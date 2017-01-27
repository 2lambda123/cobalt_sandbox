// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "media2/filters/decrypting_audio_decoder.h"

#include <stdint.h>

#include <cstdlib>

#include "base/bind.h"
#include "base/callback_helpers.h"
#include "base/location.h"
#include "base/logging.h"
#include "base/single_thread_task_runner.h"
#include "base/string_number_conversions.h"
#include "media2/base/audio_buffer.h"
#include "media2/base/audio_decoder_config.h"
#include "media2/base/audio_timestamp_helper.h"
#include "media2/base/bind_to_current_loop.h"
#include "media2/base/cdm_context.h"
#include "media2/base/decoder_buffer.h"
#include "media2/base/media_log.h"
#include "media2/base/timestamp_constants.h"

namespace media {

static inline bool IsOutOfSync(const base::TimeDelta& timestamp_1,
                               const base::TimeDelta& timestamp_2) {
  // Out of sync of 100ms would be pretty noticeable and we should keep any
  // drift below that.
  const int64_t kOutOfSyncThresholdInMilliseconds = 100;
  return std::abs(timestamp_1.InMilliseconds() - timestamp_2.InMilliseconds()) >
         kOutOfSyncThresholdInMilliseconds;
}

DecryptingAudioDecoder::DecryptingAudioDecoder(
    const scoped_refptr<base::SingleThreadTaskRunner>& task_runner,
    const scoped_refptr<MediaLog>& media_log,
    const base::Closure& waiting_for_decryption_key_cb)
    : task_runner_(task_runner),
      media_log_(media_log),
      state_(kUninitialized),
      waiting_for_decryption_key_cb_(waiting_for_decryption_key_cb),
      decryptor_(NULL),
      key_added_while_decode_pending_(false),
      weak_factory_(this) {}

std::string DecryptingAudioDecoder::GetDisplayName() const {
  return "DecryptingAudioDecoder";
}

void DecryptingAudioDecoder::Initialize(const AudioDecoderConfig& config,
                                        CdmContext* cdm_context,
                                        const InitCB& init_cb,
                                        const OutputCB& output_cb) {
  DVLOG(2) << "Initialize()";
  DCHECK(task_runner_->BelongsToCurrentThread());
  DCHECK(decode_cb_.is_null());
  DCHECK(reset_cb_.is_null());

  weak_this_ = weak_factory_.GetWeakPtr();
  init_cb_ = BindToCurrentLoop(init_cb);
  output_cb_ = BindToCurrentLoop(output_cb);

  // TODO(xhwang): We should be able to DCHECK config.IsValidConfig() and
  // config.is_encrypted().
  if (!config.IsValidConfig()) {
    DLOG(ERROR) << "Invalid audio stream config.";
    base::ResetAndReturn(&init_cb_).Run(false);
    return;
  }

  // DecryptingAudioDecoder only accepts potentially encrypted stream.
  if (!config.is_encrypted()) {
    base::ResetAndReturn(&init_cb_).Run(false);
    return;
  }

  config_ = config;

  if (state_ == kUninitialized) {
    DCHECK(cdm_context);
    if (!cdm_context->GetDecryptor()) {
      MEDIA_LOG(DEBUG, media_log_) << GetDisplayName() << ": no decryptor";
      base::ResetAndReturn(&init_cb_).Run(false);
      return;
    }

    decryptor_ = cdm_context->GetDecryptor();
  } else {
    // Reinitialization (i.e. upon a config change)
    decryptor_->DeinitializeDecoder(Decryptor::kAudio);
  }

  InitializeDecoder();
}

void DecryptingAudioDecoder::Decode(const scoped_refptr<DecoderBuffer>& buffer,
                                    const DecodeCB& decode_cb) {
  DVLOG(3) << "Decode()";
  DCHECK(task_runner_->BelongsToCurrentThread());
  DCHECK(state_ == kIdle || state_ == kDecodeFinished) << state_;
  DCHECK(!decode_cb.is_null());
  CHECK(decode_cb_.is_null()) << "Overlapping decodes are not supported.";

  decode_cb_ = BindToCurrentLoop(decode_cb);

  // Return empty (end-of-stream) frames if decoding has finished.
  if (state_ == kDecodeFinished) {
    output_cb_.Run(AudioBuffer::CreateEOSBuffer());
    base::ResetAndReturn(&decode_cb_).Run(DecodeStatus::OK);
    return;
  }

  // Initialize the |next_output_timestamp_| to be the timestamp of the first
  // non-EOS buffer.
  if (timestamp_helper_->base_timestamp() == kNoTimestamp &&
      !buffer->end_of_stream()) {
    timestamp_helper_->SetBaseTimestamp(buffer->timestamp());
  }

  pending_buffer_to_decode_ = buffer;
  state_ = kPendingDecode;
  DecodePendingBuffer();
}

void DecryptingAudioDecoder::Reset(const base::Closure& closure) {
  DVLOG(2) << "Reset() - state: " << state_;
  DCHECK(task_runner_->BelongsToCurrentThread());
  DCHECK(state_ == kIdle ||
         state_ == kPendingDecode ||
         state_ == kWaitingForKey ||
         state_ == kDecodeFinished) << state_;
  DCHECK(init_cb_.is_null());  // No Reset() during pending initialization.
  DCHECK(reset_cb_.is_null());

  reset_cb_ = BindToCurrentLoop(closure);

  decryptor_->ResetDecoder(Decryptor::kAudio);

  // Reset() cannot complete if the read callback is still pending.
  // Defer the resetting process in this case. The |reset_cb_| will be fired
  // after the read callback is fired - see DecryptAndDecodeBuffer() and
  // DeliverFrame().
  if (state_ == kPendingDecode) {
    DCHECK(!decode_cb_.is_null());
    return;
  }

  if (state_ == kWaitingForKey) {
    DCHECK(!decode_cb_.is_null());
    pending_buffer_to_decode_ = NULL;
    base::ResetAndReturn(&decode_cb_).Run(DecodeStatus::ABORTED);
  }

  DCHECK(decode_cb_.is_null());
  DoReset();
}

DecryptingAudioDecoder::~DecryptingAudioDecoder() {
  DVLOG(2) << __func__;
  DCHECK(task_runner_->BelongsToCurrentThread());

  if (state_ == kUninitialized)
    return;

  if (decryptor_) {
    decryptor_->DeinitializeDecoder(Decryptor::kAudio);
    decryptor_ = NULL;
  }
  pending_buffer_to_decode_ = NULL;
  if (!init_cb_.is_null())
    base::ResetAndReturn(&init_cb_).Run(false);
  if (!decode_cb_.is_null())
    base::ResetAndReturn(&decode_cb_).Run(DecodeStatus::ABORTED);
  if (!reset_cb_.is_null())
    base::ResetAndReturn(&reset_cb_).Run();
}

void DecryptingAudioDecoder::InitializeDecoder() {
  state_ = kPendingDecoderInit;
  decryptor_->InitializeAudioDecoder(
      config_,
      BindToCurrentLoop(base::Bind(
          &DecryptingAudioDecoder::FinishInitialization, weak_this_)));
}

void DecryptingAudioDecoder::FinishInitialization(bool success) {
  DVLOG(2) << "FinishInitialization()";
  DCHECK(task_runner_->BelongsToCurrentThread());
  DCHECK(state_ == kPendingDecoderInit) << state_;
  DCHECK(!init_cb_.is_null());
  DCHECK(reset_cb_.is_null());  // No Reset() before initialization finished.
  DCHECK(decode_cb_.is_null());  // No Decode() before initialization finished.

  if (!success) {
    MEDIA_LOG(DEBUG, media_log_) << GetDisplayName()
                                 << ": failed to init decoder on decryptor";
    base::ResetAndReturn(&init_cb_).Run(false);
    decryptor_ = NULL;
    state_ = kError;
    return;
  }

  // Success!
  timestamp_helper_.reset(
      new AudioTimestampHelper(config_.samples_per_second()));

  decryptor_->RegisterNewKeyCB(
      Decryptor::kAudio,
      BindToCurrentLoop(
          base::Bind(&DecryptingAudioDecoder::OnKeyAdded, weak_this_)));

  state_ = kIdle;
  base::ResetAndReturn(&init_cb_).Run(true);
}

void DecryptingAudioDecoder::DecodePendingBuffer() {
  DCHECK(task_runner_->BelongsToCurrentThread());
  DCHECK_EQ(state_, kPendingDecode) << state_;

  int buffer_size = 0;
  if (!pending_buffer_to_decode_->end_of_stream()) {
    buffer_size = pending_buffer_to_decode_->data_size();
  }

  decryptor_->DecryptAndDecodeAudio(
      pending_buffer_to_decode_,
      BindToCurrentLoop(base::Bind(
          &DecryptingAudioDecoder::DeliverFrame, weak_this_, buffer_size)));
}

void DecryptingAudioDecoder::DeliverFrame(
    int buffer_size,
    Decryptor::Status status,
    const Decryptor::AudioFrames& frames) {
  DVLOG(3) << "DeliverFrame() - status: " << status;
  DCHECK(task_runner_->BelongsToCurrentThread());
  DCHECK_EQ(state_, kPendingDecode) << state_;
  DCHECK(!decode_cb_.is_null());
  DCHECK(pending_buffer_to_decode_.get());

  bool need_to_try_again_if_nokey_is_returned = key_added_while_decode_pending_;
  key_added_while_decode_pending_ = false;

  scoped_refptr<DecoderBuffer> scoped_pending_buffer_to_decode =
      pending_buffer_to_decode_;
  pending_buffer_to_decode_ = NULL;

  if (!reset_cb_.is_null()) {
    base::ResetAndReturn(&decode_cb_).Run(DecodeStatus::ABORTED);
    DoReset();
    return;
  }

  DCHECK_EQ(status == Decryptor::kSuccess, !frames.empty());

  if (status == Decryptor::kError) {
    DVLOG(2) << "DeliverFrame() - kError";
    MEDIA_LOG(ERROR, media_log_) << GetDisplayName() << ": decode error";
    state_ = kDecodeFinished; // TODO add kError state
    base::ResetAndReturn(&decode_cb_).Run(DecodeStatus::DECODE_ERROR);
    return;
  }

  if (status == Decryptor::kNoKey) {
    std::string key_id =
        scoped_pending_buffer_to_decode->decrypt_config()->key_id();
    std::string missing_key_id = base::HexEncode(key_id.data(), key_id.size());
    DVLOG(1) << "DeliverFrame() - no key for key ID " << missing_key_id;
    MEDIA_LOG(DEBUG, media_log_) << GetDisplayName() << ": no key for key ID "
                                 << missing_key_id;

    // Set |pending_buffer_to_decode_| back as we need to try decoding the
    // pending buffer again when new key is added to the decryptor.
    pending_buffer_to_decode_ = scoped_pending_buffer_to_decode;

    if (need_to_try_again_if_nokey_is_returned) {
      // The |state_| is still kPendingDecode.
      MEDIA_LOG(INFO, media_log_) << GetDisplayName()
                                  << ": key was added, resuming decode";
      DecodePendingBuffer();
      return;
    }

    state_ = kWaitingForKey;
    waiting_for_decryption_key_cb_.Run();
    return;
  }

  if (status == Decryptor::kNeedMoreData) {
    DVLOG(2) << "DeliverFrame() - kNeedMoreData";
    state_ = scoped_pending_buffer_to_decode->end_of_stream() ? kDecodeFinished
                                                              : kIdle;
    base::ResetAndReturn(&decode_cb_).Run(DecodeStatus::OK);
    return;
  }

  DCHECK_EQ(status, Decryptor::kSuccess);
  DCHECK(!frames.empty());
  ProcessDecodedFrames(frames);

  if (scoped_pending_buffer_to_decode->end_of_stream()) {
    // Set |pending_buffer_to_decode_| back as we need to keep flushing the
    // decryptor until kNeedMoreData is returned.
    pending_buffer_to_decode_ = scoped_pending_buffer_to_decode;
    DecodePendingBuffer();
    return;
  }

  state_ = kIdle;
  base::ResetAndReturn(&decode_cb_).Run(DecodeStatus::OK);
}

void DecryptingAudioDecoder::OnKeyAdded() {
  DCHECK(task_runner_->BelongsToCurrentThread());

  if (state_ == kPendingDecode) {
    key_added_while_decode_pending_ = true;
    return;
  }

  if (state_ == kWaitingForKey) {
    MEDIA_LOG(INFO, media_log_) << GetDisplayName()
                                << ": key added, resuming decode";
    state_ = kPendingDecode;
    DecodePendingBuffer();
  }
}

void DecryptingAudioDecoder::DoReset() {
  DCHECK(init_cb_.is_null());
  DCHECK(decode_cb_.is_null());
  timestamp_helper_->SetBaseTimestamp(kNoTimestamp);
  state_ = kIdle;
  base::ResetAndReturn(&reset_cb_).Run();
}

void DecryptingAudioDecoder::ProcessDecodedFrames(
    const Decryptor::AudioFrames& frames) {
  for (Decryptor::AudioFrames::const_iterator iter = frames.begin();
       iter != frames.end();
       ++iter) {
    scoped_refptr<AudioBuffer> frame = *iter;

    DCHECK(!frame->end_of_stream()) << "EOS frame returned.";
    DCHECK_GT(frame->frame_count(), 0) << "Empty frame returned.";

    base::TimeDelta current_time = timestamp_helper_->GetTimestamp();
    if (IsOutOfSync(current_time, frame->timestamp())) {
      DVLOG(1) << "Timestamp returned by the decoder ("
               << frame->timestamp().InMilliseconds() << " ms)"
               << " does not match the input timestamp and number of samples"
               << " decoded (" << current_time.InMilliseconds() << " ms).";
    }

    frame->set_timestamp(current_time);
    timestamp_helper_->AddFrames(frame->frame_count());

    output_cb_.Run(frame);
  }
}

}  // namespace media
