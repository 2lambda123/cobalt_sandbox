// Copyright 2016 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "media/remoting/proto_enum_utils.h"

namespace media {
namespace remoting {

#define CASE_RETURN_OTHER(x) \
  case OriginType::x:        \
    return OtherType::x

#define CASE_RETURN_ORIGIN_TO_OTHER(x, y) \
  case OriginType::x:                     \
    return OtherType::y

absl::optional<AudioCodec> ToMediaAudioCodec(
    openscreen::cast::AudioDecoderConfig::Codec value) {
  using OriginType = openscreen::cast::AudioDecoderConfig;
  using OtherType = AudioCodec;
  switch (value) {
    CASE_RETURN_ORIGIN_TO_OTHER(kUnknownAudioCodec, kUnknown);
    CASE_RETURN_ORIGIN_TO_OTHER(kCodecAAC, kAAC);
    CASE_RETURN_ORIGIN_TO_OTHER(kCodecMP3, kMP3);
    CASE_RETURN_ORIGIN_TO_OTHER(kCodecPCM, kPCM);
    CASE_RETURN_ORIGIN_TO_OTHER(kCodecVorbis, kVorbis);
    CASE_RETURN_ORIGIN_TO_OTHER(kCodecFLAC, kFLAC);
    CASE_RETURN_ORIGIN_TO_OTHER(kCodecAMR_NB, kAMR_NB);
    CASE_RETURN_ORIGIN_TO_OTHER(kCodecAMR_WB, kAMR_WB);
    CASE_RETURN_ORIGIN_TO_OTHER(kCodecPCM_MULAW, kPCM_MULAW);
    CASE_RETURN_ORIGIN_TO_OTHER(kCodecGSM_MS, kGSM_MS);
    CASE_RETURN_ORIGIN_TO_OTHER(kCodecPCM_S16BE, kPCM_S16BE);
    CASE_RETURN_ORIGIN_TO_OTHER(kCodecPCM_S24BE, kPCM_S24BE);
    CASE_RETURN_ORIGIN_TO_OTHER(kCodecOpus, kOpus);
    CASE_RETURN_ORIGIN_TO_OTHER(kCodecEAC3, kEAC3);
    CASE_RETURN_ORIGIN_TO_OTHER(kCodecPCM_ALAW, kPCM_ALAW);
    CASE_RETURN_ORIGIN_TO_OTHER(kCodecALAC, kALAC);
    CASE_RETURN_ORIGIN_TO_OTHER(kCodecAC3, kAC3);
    CASE_RETURN_ORIGIN_TO_OTHER(kCodecMpegHAudio, kMpegHAudio);
    default:
      return absl::nullopt;
  }
}

absl::optional<openscreen::cast::AudioDecoderConfig::Codec>
ToProtoAudioDecoderConfigCodec(AudioCodec value) {
  using OriginType = AudioCodec;
  using OtherType = openscreen::cast::AudioDecoderConfig;
  switch (value) {
    CASE_RETURN_ORIGIN_TO_OTHER(kUnknown, kUnknownAudioCodec);
    CASE_RETURN_ORIGIN_TO_OTHER(kAAC, kCodecAAC);
    CASE_RETURN_ORIGIN_TO_OTHER(kMP3, kCodecMP3);
    CASE_RETURN_ORIGIN_TO_OTHER(kPCM, kCodecPCM);
    CASE_RETURN_ORIGIN_TO_OTHER(kVorbis, kCodecVorbis);
    CASE_RETURN_ORIGIN_TO_OTHER(kFLAC, kCodecFLAC);
    CASE_RETURN_ORIGIN_TO_OTHER(kAMR_NB, kCodecAMR_NB);
    CASE_RETURN_ORIGIN_TO_OTHER(kAMR_WB, kCodecAMR_WB);
    CASE_RETURN_ORIGIN_TO_OTHER(kPCM_MULAW, kCodecPCM_MULAW);
    CASE_RETURN_ORIGIN_TO_OTHER(kGSM_MS, kCodecGSM_MS);
    CASE_RETURN_ORIGIN_TO_OTHER(kPCM_S16BE, kCodecPCM_S16BE);
    CASE_RETURN_ORIGIN_TO_OTHER(kPCM_S24BE, kCodecPCM_S24BE);
    CASE_RETURN_ORIGIN_TO_OTHER(kOpus, kCodecOpus);
    CASE_RETURN_ORIGIN_TO_OTHER(kEAC3, kCodecEAC3);
    CASE_RETURN_ORIGIN_TO_OTHER(kPCM_ALAW, kCodecPCM_ALAW);
    CASE_RETURN_ORIGIN_TO_OTHER(kALAC, kCodecALAC);
    CASE_RETURN_ORIGIN_TO_OTHER(kAC3, kCodecAC3);
    CASE_RETURN_ORIGIN_TO_OTHER(kMpegHAudio, kCodecMpegHAudio);
    default:
      return absl::nullopt;
  }
}

absl::optional<SampleFormat> ToMediaSampleFormat(
    openscreen::cast::AudioDecoderConfig::SampleFormat value) {
  using OriginType = openscreen::cast::AudioDecoderConfig;
  using OtherType = SampleFormat;
  switch (value) {
    CASE_RETURN_OTHER(kUnknownSampleFormat);
    CASE_RETURN_OTHER(kSampleFormatU8);
    CASE_RETURN_OTHER(kSampleFormatS16);
    CASE_RETURN_OTHER(kSampleFormatS32);
    CASE_RETURN_OTHER(kSampleFormatF32);
    CASE_RETURN_OTHER(kSampleFormatPlanarU8);
    CASE_RETURN_OTHER(kSampleFormatPlanarS16);
    CASE_RETURN_OTHER(kSampleFormatPlanarF32);
    CASE_RETURN_OTHER(kSampleFormatPlanarS32);
    CASE_RETURN_OTHER(kSampleFormatS24);
    CASE_RETURN_OTHER(kSampleFormatAc3);
    CASE_RETURN_OTHER(kSampleFormatEac3);
    CASE_RETURN_OTHER(kSampleFormatMpegHAudio);
    default:
      return absl::nullopt;
  }
}

absl::optional<openscreen::cast::AudioDecoderConfig::SampleFormat>
ToProtoAudioDecoderConfigSampleFormat(SampleFormat value) {
  using OriginType = SampleFormat;
  using OtherType = openscreen::cast::AudioDecoderConfig;
  switch (value) {
    CASE_RETURN_OTHER(kUnknownSampleFormat);
    CASE_RETURN_OTHER(kSampleFormatU8);
    CASE_RETURN_OTHER(kSampleFormatS16);
    CASE_RETURN_OTHER(kSampleFormatS32);
    CASE_RETURN_OTHER(kSampleFormatF32);
    CASE_RETURN_OTHER(kSampleFormatPlanarU8);
    CASE_RETURN_OTHER(kSampleFormatPlanarS16);
    CASE_RETURN_OTHER(kSampleFormatPlanarF32);
    CASE_RETURN_OTHER(kSampleFormatPlanarS32);
    CASE_RETURN_OTHER(kSampleFormatS24);
    CASE_RETURN_OTHER(kSampleFormatAc3);
    CASE_RETURN_OTHER(kSampleFormatEac3);
    CASE_RETURN_OTHER(kSampleFormatMpegHAudio);
    default:
      return absl::nullopt;
  }
}

absl::optional<ChannelLayout> ToMediaChannelLayout(
    openscreen::cast::AudioDecoderConfig::ChannelLayout value) {
  using OriginType = openscreen::cast::AudioDecoderConfig;
  using OtherType = ChannelLayout;
  switch (value) {
    CASE_RETURN_OTHER(CHANNEL_LAYOUT_NONE);
    CASE_RETURN_OTHER(CHANNEL_LAYOUT_UNSUPPORTED);
    CASE_RETURN_OTHER(CHANNEL_LAYOUT_MONO);
    CASE_RETURN_OTHER(CHANNEL_LAYOUT_STEREO);
    CASE_RETURN_OTHER(CHANNEL_LAYOUT_2_1);
    CASE_RETURN_OTHER(CHANNEL_LAYOUT_SURROUND);
    CASE_RETURN_OTHER(CHANNEL_LAYOUT_4_0);
    CASE_RETURN_OTHER(CHANNEL_LAYOUT_2_2);
    CASE_RETURN_OTHER(CHANNEL_LAYOUT_QUAD);
    CASE_RETURN_OTHER(CHANNEL_LAYOUT_5_0);
    CASE_RETURN_OTHER(CHANNEL_LAYOUT_5_1);
    CASE_RETURN_OTHER(CHANNEL_LAYOUT_5_0_BACK);
    CASE_RETURN_OTHER(CHANNEL_LAYOUT_5_1_BACK);
    CASE_RETURN_OTHER(CHANNEL_LAYOUT_7_0);
    CASE_RETURN_OTHER(CHANNEL_LAYOUT_7_1);
    CASE_RETURN_OTHER(CHANNEL_LAYOUT_7_1_WIDE);
    CASE_RETURN_OTHER(CHANNEL_LAYOUT_STEREO_DOWNMIX);
    CASE_RETURN_OTHER(CHANNEL_LAYOUT_2POINT1);
    CASE_RETURN_OTHER(CHANNEL_LAYOUT_3_1);
    CASE_RETURN_OTHER(CHANNEL_LAYOUT_4_1);
    CASE_RETURN_OTHER(CHANNEL_LAYOUT_6_0);
    CASE_RETURN_OTHER(CHANNEL_LAYOUT_6_0_FRONT);
    CASE_RETURN_OTHER(CHANNEL_LAYOUT_HEXAGONAL);
    CASE_RETURN_OTHER(CHANNEL_LAYOUT_6_1);
    CASE_RETURN_OTHER(CHANNEL_LAYOUT_6_1_BACK);
    CASE_RETURN_OTHER(CHANNEL_LAYOUT_6_1_FRONT);
    CASE_RETURN_OTHER(CHANNEL_LAYOUT_7_0_FRONT);
    CASE_RETURN_OTHER(CHANNEL_LAYOUT_7_1_WIDE_BACK);
    CASE_RETURN_OTHER(CHANNEL_LAYOUT_OCTAGONAL);
    CASE_RETURN_OTHER(CHANNEL_LAYOUT_DISCRETE);
    CASE_RETURN_OTHER(CHANNEL_LAYOUT_STEREO_AND_KEYBOARD_MIC);
    CASE_RETURN_OTHER(CHANNEL_LAYOUT_4_1_QUAD_SIDE);
    CASE_RETURN_OTHER(CHANNEL_LAYOUT_BITSTREAM);
    default:
      return absl::nullopt;
  }
}

absl::optional<openscreen::cast::AudioDecoderConfig::ChannelLayout>
ToProtoAudioDecoderConfigChannelLayout(ChannelLayout value) {
  using OriginType = ChannelLayout;
  using OtherType = openscreen::cast::AudioDecoderConfig;
  switch (value) {
    CASE_RETURN_OTHER(CHANNEL_LAYOUT_NONE);
    CASE_RETURN_OTHER(CHANNEL_LAYOUT_UNSUPPORTED);
    CASE_RETURN_OTHER(CHANNEL_LAYOUT_MONO);
    CASE_RETURN_OTHER(CHANNEL_LAYOUT_STEREO);
    CASE_RETURN_OTHER(CHANNEL_LAYOUT_2_1);
    CASE_RETURN_OTHER(CHANNEL_LAYOUT_SURROUND);
    CASE_RETURN_OTHER(CHANNEL_LAYOUT_4_0);
    CASE_RETURN_OTHER(CHANNEL_LAYOUT_2_2);
    CASE_RETURN_OTHER(CHANNEL_LAYOUT_QUAD);
    CASE_RETURN_OTHER(CHANNEL_LAYOUT_5_0);
    CASE_RETURN_OTHER(CHANNEL_LAYOUT_5_1);
    CASE_RETURN_OTHER(CHANNEL_LAYOUT_5_0_BACK);
    CASE_RETURN_OTHER(CHANNEL_LAYOUT_5_1_BACK);
    CASE_RETURN_OTHER(CHANNEL_LAYOUT_7_0);
    CASE_RETURN_OTHER(CHANNEL_LAYOUT_7_1);
    CASE_RETURN_OTHER(CHANNEL_LAYOUT_7_1_WIDE);
    CASE_RETURN_OTHER(CHANNEL_LAYOUT_STEREO_DOWNMIX);
    CASE_RETURN_OTHER(CHANNEL_LAYOUT_2POINT1);
    CASE_RETURN_OTHER(CHANNEL_LAYOUT_3_1);
    CASE_RETURN_OTHER(CHANNEL_LAYOUT_4_1);
    CASE_RETURN_OTHER(CHANNEL_LAYOUT_6_0);
    CASE_RETURN_OTHER(CHANNEL_LAYOUT_6_0_FRONT);
    CASE_RETURN_OTHER(CHANNEL_LAYOUT_HEXAGONAL);
    CASE_RETURN_OTHER(CHANNEL_LAYOUT_6_1);
    CASE_RETURN_OTHER(CHANNEL_LAYOUT_6_1_BACK);
    CASE_RETURN_OTHER(CHANNEL_LAYOUT_6_1_FRONT);
    CASE_RETURN_OTHER(CHANNEL_LAYOUT_7_0_FRONT);
    CASE_RETURN_OTHER(CHANNEL_LAYOUT_7_1_WIDE_BACK);
    CASE_RETURN_OTHER(CHANNEL_LAYOUT_OCTAGONAL);
    CASE_RETURN_OTHER(CHANNEL_LAYOUT_DISCRETE);
    CASE_RETURN_OTHER(CHANNEL_LAYOUT_STEREO_AND_KEYBOARD_MIC);
    CASE_RETURN_OTHER(CHANNEL_LAYOUT_4_1_QUAD_SIDE);
    CASE_RETURN_OTHER(CHANNEL_LAYOUT_BITSTREAM);
    default:
      return absl::nullopt;
  }
}

absl::optional<VideoCodec> ToMediaVideoCodec(
    openscreen::cast::VideoDecoderConfig::Codec value) {
  using OriginType = openscreen::cast::VideoDecoderConfig;
  using OtherType = VideoCodec;
  switch (value) {
    CASE_RETURN_ORIGIN_TO_OTHER(kUnknownVideoCodec, kUnknown);
    CASE_RETURN_ORIGIN_TO_OTHER(kCodecH264, kH264);
    CASE_RETURN_ORIGIN_TO_OTHER(kCodecVC1, kVC1);
    CASE_RETURN_ORIGIN_TO_OTHER(kCodecMPEG2, kMPEG2);
    CASE_RETURN_ORIGIN_TO_OTHER(kCodecMPEG4, kMPEG4);
    CASE_RETURN_ORIGIN_TO_OTHER(kCodecTheora, kTheora);
    CASE_RETURN_ORIGIN_TO_OTHER(kCodecVP8, kVP8);
    CASE_RETURN_ORIGIN_TO_OTHER(kCodecVP9, kVP9);
    CASE_RETURN_ORIGIN_TO_OTHER(kCodecHEVC, kHEVC);
    CASE_RETURN_ORIGIN_TO_OTHER(kCodecDolbyVision, kDolbyVision);
    CASE_RETURN_ORIGIN_TO_OTHER(kCodecAV1, kAV1);
    default:
      return absl::nullopt;
  }
}

absl::optional<openscreen::cast::VideoDecoderConfig::Codec>
ToProtoVideoDecoderConfigCodec(VideoCodec value) {
  using OriginType = VideoCodec;
  using OtherType = openscreen::cast::VideoDecoderConfig;
  switch (value) {
    CASE_RETURN_ORIGIN_TO_OTHER(kUnknown, kUnknownVideoCodec);
    CASE_RETURN_ORIGIN_TO_OTHER(kH264, kCodecH264);
    CASE_RETURN_ORIGIN_TO_OTHER(kVC1, kCodecVC1);
    CASE_RETURN_ORIGIN_TO_OTHER(kMPEG2, kCodecMPEG2);
    CASE_RETURN_ORIGIN_TO_OTHER(kMPEG4, kCodecMPEG4);
    CASE_RETURN_ORIGIN_TO_OTHER(kTheora, kCodecTheora);
    CASE_RETURN_ORIGIN_TO_OTHER(kVP8, kCodecVP8);
    CASE_RETURN_ORIGIN_TO_OTHER(kVP9, kCodecVP9);
    CASE_RETURN_ORIGIN_TO_OTHER(kHEVC, kCodecHEVC);
    CASE_RETURN_ORIGIN_TO_OTHER(kDolbyVision, kCodecDolbyVision);
    CASE_RETURN_ORIGIN_TO_OTHER(kAV1, kCodecAV1);
    default:
      return absl::nullopt;
  }
}

absl::optional<VideoCodecProfile> ToMediaVideoCodecProfile(
    openscreen::cast::VideoDecoderConfig::Profile value) {
  using OriginType = openscreen::cast::VideoDecoderConfig;
  using OtherType = VideoCodecProfile;
  switch (value) {
    CASE_RETURN_OTHER(VIDEO_CODEC_PROFILE_UNKNOWN);
    CASE_RETURN_OTHER(H264PROFILE_BASELINE);
    CASE_RETURN_OTHER(H264PROFILE_MAIN);
    CASE_RETURN_OTHER(H264PROFILE_EXTENDED);
    CASE_RETURN_OTHER(H264PROFILE_HIGH);
    CASE_RETURN_OTHER(H264PROFILE_HIGH10PROFILE);
    CASE_RETURN_OTHER(H264PROFILE_HIGH422PROFILE);
    CASE_RETURN_OTHER(H264PROFILE_HIGH444PREDICTIVEPROFILE);
    CASE_RETURN_OTHER(H264PROFILE_SCALABLEBASELINE);
    CASE_RETURN_OTHER(H264PROFILE_SCALABLEHIGH);
    CASE_RETURN_OTHER(H264PROFILE_STEREOHIGH);
    CASE_RETURN_OTHER(H264PROFILE_MULTIVIEWHIGH);
    CASE_RETURN_OTHER(VP8PROFILE_ANY);
    CASE_RETURN_OTHER(VP9PROFILE_PROFILE0);
    CASE_RETURN_OTHER(VP9PROFILE_PROFILE1);
    CASE_RETURN_OTHER(VP9PROFILE_PROFILE2);
    CASE_RETURN_OTHER(VP9PROFILE_PROFILE3);
    CASE_RETURN_OTHER(HEVCPROFILE_MAIN);
    CASE_RETURN_OTHER(HEVCPROFILE_MAIN10);
    CASE_RETURN_OTHER(HEVCPROFILE_MAIN_STILL_PICTURE);
    CASE_RETURN_OTHER(DOLBYVISION_PROFILE0);
    CASE_RETURN_OTHER(DOLBYVISION_PROFILE4);
    CASE_RETURN_OTHER(DOLBYVISION_PROFILE5);
    CASE_RETURN_OTHER(DOLBYVISION_PROFILE7);
    CASE_RETURN_OTHER(DOLBYVISION_PROFILE8);
    CASE_RETURN_OTHER(DOLBYVISION_PROFILE9);
    CASE_RETURN_OTHER(THEORAPROFILE_ANY);
    CASE_RETURN_OTHER(AV1PROFILE_PROFILE_MAIN);
    CASE_RETURN_OTHER(AV1PROFILE_PROFILE_HIGH);
    CASE_RETURN_OTHER(AV1PROFILE_PROFILE_PRO);
    default:
      return absl::nullopt;
  }
}

absl::optional<openscreen::cast::VideoDecoderConfig::Profile>
ToProtoVideoDecoderConfigProfile(VideoCodecProfile value) {
  using OriginType = VideoCodecProfile;
  using OtherType = openscreen::cast::VideoDecoderConfig;
  switch (value) {
    CASE_RETURN_OTHER(VIDEO_CODEC_PROFILE_UNKNOWN);
    CASE_RETURN_OTHER(H264PROFILE_BASELINE);
    CASE_RETURN_OTHER(H264PROFILE_MAIN);
    CASE_RETURN_OTHER(H264PROFILE_EXTENDED);
    CASE_RETURN_OTHER(H264PROFILE_HIGH);
    CASE_RETURN_OTHER(H264PROFILE_HIGH10PROFILE);
    CASE_RETURN_OTHER(H264PROFILE_HIGH422PROFILE);
    CASE_RETURN_OTHER(H264PROFILE_HIGH444PREDICTIVEPROFILE);
    CASE_RETURN_OTHER(H264PROFILE_SCALABLEBASELINE);
    CASE_RETURN_OTHER(H264PROFILE_SCALABLEHIGH);
    CASE_RETURN_OTHER(H264PROFILE_STEREOHIGH);
    CASE_RETURN_OTHER(H264PROFILE_MULTIVIEWHIGH);
    CASE_RETURN_OTHER(VP8PROFILE_ANY);
    CASE_RETURN_OTHER(VP9PROFILE_PROFILE0);
    CASE_RETURN_OTHER(VP9PROFILE_PROFILE1);
    CASE_RETURN_OTHER(VP9PROFILE_PROFILE2);
    CASE_RETURN_OTHER(VP9PROFILE_PROFILE3);
    CASE_RETURN_OTHER(HEVCPROFILE_MAIN);
    CASE_RETURN_OTHER(HEVCPROFILE_MAIN10);
    CASE_RETURN_OTHER(HEVCPROFILE_MAIN_STILL_PICTURE);
    CASE_RETURN_OTHER(DOLBYVISION_PROFILE0);
    CASE_RETURN_OTHER(DOLBYVISION_PROFILE4);
    CASE_RETURN_OTHER(DOLBYVISION_PROFILE5);
    CASE_RETURN_OTHER(DOLBYVISION_PROFILE7);
    CASE_RETURN_OTHER(DOLBYVISION_PROFILE8);
    CASE_RETURN_OTHER(DOLBYVISION_PROFILE9);
    CASE_RETURN_OTHER(THEORAPROFILE_ANY);
    CASE_RETURN_OTHER(AV1PROFILE_PROFILE_MAIN);
    CASE_RETURN_OTHER(AV1PROFILE_PROFILE_HIGH);
    CASE_RETURN_OTHER(AV1PROFILE_PROFILE_PRO);
    default:
      return absl::nullopt;
  }
}

absl::optional<VideoPixelFormat> ToMediaVideoPixelFormat(
    openscreen::cast::VideoDecoderConfig::Format value) {
  using OriginType = openscreen::cast::VideoDecoderConfig;
  using OtherType = VideoPixelFormat;
  switch (value) {
    CASE_RETURN_OTHER(PIXEL_FORMAT_UNKNOWN);
    CASE_RETURN_OTHER(PIXEL_FORMAT_I420);
    CASE_RETURN_OTHER(PIXEL_FORMAT_YV12);
    CASE_RETURN_OTHER(PIXEL_FORMAT_I422);
    CASE_RETURN_OTHER(PIXEL_FORMAT_I420A);
    CASE_RETURN_OTHER(PIXEL_FORMAT_I444);
    CASE_RETURN_OTHER(PIXEL_FORMAT_NV12);
    CASE_RETURN_OTHER(PIXEL_FORMAT_NV21);
    CASE_RETURN_OTHER(PIXEL_FORMAT_YUY2);
    CASE_RETURN_OTHER(PIXEL_FORMAT_ARGB);
    CASE_RETURN_OTHER(PIXEL_FORMAT_XRGB);
    CASE_RETURN_OTHER(PIXEL_FORMAT_RGB24);
    CASE_RETURN_OTHER(PIXEL_FORMAT_MJPEG);
    CASE_RETURN_OTHER(PIXEL_FORMAT_YUV420P9);
    CASE_RETURN_OTHER(PIXEL_FORMAT_YUV420P10);
    CASE_RETURN_OTHER(PIXEL_FORMAT_YUV422P9);
    CASE_RETURN_OTHER(PIXEL_FORMAT_YUV422P10);
    CASE_RETURN_OTHER(PIXEL_FORMAT_YUV444P9);
    CASE_RETURN_OTHER(PIXEL_FORMAT_YUV444P10);
    CASE_RETURN_OTHER(PIXEL_FORMAT_YUV420P12);
    CASE_RETURN_OTHER(PIXEL_FORMAT_YUV422P12);
    CASE_RETURN_OTHER(PIXEL_FORMAT_YUV444P12);
    CASE_RETURN_OTHER(PIXEL_FORMAT_Y16);
    CASE_RETURN_OTHER(PIXEL_FORMAT_ABGR);
    CASE_RETURN_OTHER(PIXEL_FORMAT_XBGR);
    CASE_RETURN_OTHER(PIXEL_FORMAT_P016LE);
    CASE_RETURN_OTHER(PIXEL_FORMAT_XR30);
    CASE_RETURN_OTHER(PIXEL_FORMAT_XB30);
    // PIXEL_FORMAT_UYVY, PIXEL_FORMAT_RGB32 and PIXEL_FORMAT_Y8 are deprecated.
    case openscreen::cast::VideoDecoderConfig_Format_PIXEL_FORMAT_RGB32:
      return absl::nullopt;
    default:
      return absl::nullopt;
  }
}

absl::optional<BufferingState> ToMediaBufferingState(
    openscreen::cast::RendererClientOnBufferingStateChange::State value) {
  using OriginType = openscreen::cast::RendererClientOnBufferingStateChange;
  using OtherType = BufferingState;
  switch (value) {
    CASE_RETURN_OTHER(BUFFERING_HAVE_NOTHING);
    CASE_RETURN_OTHER(BUFFERING_HAVE_ENOUGH);
    default:
      return absl::nullopt;
  }
}

absl::optional<openscreen::cast::RendererClientOnBufferingStateChange::State>
ToProtoMediaBufferingState(BufferingState value) {
  using OriginType = BufferingState;
  using OtherType = openscreen::cast::RendererClientOnBufferingStateChange;
  switch (value) {
    CASE_RETURN_OTHER(BUFFERING_HAVE_NOTHING);
    CASE_RETURN_OTHER(BUFFERING_HAVE_ENOUGH);
    default:
      return absl::nullopt;
  }
}

absl::optional<DemuxerStream::Status> ToDemuxerStreamStatus(
    openscreen::cast::DemuxerStreamReadUntilCallback::Status value) {
  using OriginType = openscreen::cast::DemuxerStreamReadUntilCallback;
  using OtherType = DemuxerStream;
  switch (value) {
    CASE_RETURN_OTHER(kOk);
    CASE_RETURN_OTHER(kAborted);
    CASE_RETURN_OTHER(kConfigChanged);
    CASE_RETURN_OTHER(kError);
    default:
      return absl::nullopt;
  }
}

absl::optional<openscreen::cast::DemuxerStreamReadUntilCallback::Status>
ToProtoDemuxerStreamStatus(DemuxerStream::Status value) {
  using OriginType = DemuxerStream;
  using OtherType = openscreen::cast::DemuxerStreamReadUntilCallback;
  switch (value) {
    CASE_RETURN_OTHER(kOk);
    CASE_RETURN_OTHER(kAborted);
    CASE_RETURN_OTHER(kConfigChanged);
    CASE_RETURN_OTHER(kError);
    default:
      return absl::nullopt;
  }
}

}  // namespace remoting
}  // namespace media
