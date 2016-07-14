// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef MEDIA_BASE_AUDIO_RENDERER_SINK_H_
#define MEDIA_BASE_AUDIO_RENDERER_SINK_H_

#include <vector>
#include "base/basictypes.h"
#include "base/memory/ref_counted.h"
#include "media/audio/audio_parameters.h"
#include "media/base/audio_bus.h"
#include "media/base/media_export.h"

#if defined(OS_STARBOARD)
#include "starboard/configuration.h"
#endif  // defined(OS_STARBOARD)

namespace media {

// AudioRendererSink is an interface representing the end-point for
// rendered audio.  An implementation is expected to
// periodically call Render() on a callback object.

class MEDIA_EXPORT AudioRendererSink
    : public base::RefCountedThreadSafe<media::AudioRendererSink> {
 public:
  class MEDIA_EXPORT RenderCallback {
   public:
    // Attempts to completely fill all channels of |dest|, returns actual
    // number of frames filled.
    virtual int Render(AudioBus* dest, int audio_delay_milliseconds) = 0;

    // Synchronized audio I/O - see InitializeIO() below.
    virtual void RenderIO(AudioBus* /* source */,
                          AudioBus* /* dest */,
                          int /* audio_delay_milliseconds */) {}

    // Signals an error has occurred.
    virtual void OnRenderError() = 0;

#if defined(__LB_SHELL__) || defined(COBALT)
    // Callback from the sink to the renderer to indicate that it is currently
    // full and will not be requesting additional data until some is consumed.
    virtual void SinkFull() = 0;

#if defined(OS_STARBOARD)
#if SB_IS(MEDIA_UNDERFLOW_DETECTED_BY_AUDIO_SINK)
    // Callback from the sink to the renderer to indicate that it has not
    // enough data to continue playback without playing past the end of
    // buffered data.
    virtual void SinkUnderflow() = 0;
#endif  // SB_IS(MEDIA_UNDERFLOW_DETECTED_BY_AUDIO_SINK)
#endif  // defined(OS_STARBOARD)
#endif  // defined(__LB_SHELL__) || defined(COBALT)

   protected:
    virtual ~RenderCallback() {}
  };

  // Sets important information about the audio stream format.
  // It must be called before any of the other methods.
  virtual void Initialize(const AudioParameters& params,
                          RenderCallback* callback) = 0;

  // InitializeIO() may be called instead of Initialize() for clients who wish
  // to have synchronized input and output.  |input_channels| specifies the
  // number of input channels which will be at the same sample-rate
  // and buffer-size as the output as specified in |params|.
  // The callback's RenderIO() method will be called instead of Render(),
  // providing the synchronized input data at the same time as when new
  // output data is to be rendered.
  virtual void InitializeIO(const AudioParameters& /* params */,
                            int /* input_channels */,
                            RenderCallback* /* callback */) {}

  // Starts audio playback.
  virtual void Start() = 0;

  // Stops audio playback.
  virtual void Stop() = 0;

  // Pauses playback.
  virtual void Pause(bool flush) = 0;

  // Resumes playback after calling Pause().
  virtual void Play() = 0;

  // Sets the playback volume, with range [0.0, 1.0] inclusive.
  // Returns |true| on success.
  virtual bool SetVolume(double volume) = 0;

#if defined(__LB_SHELL__) || defined(COBALT)
  // To avoid duplication of audio data and additional copies our Sink
  // implementation is responsible for buffering rendered audio. As a
  // result the renderer relays the message to buffer more audio back
  // to the Sink.
  virtual void ResumeAfterUnderflow(bool buffer_more_audio) = 0;
#endif

 protected:
  friend class base::RefCountedThreadSafe<AudioRendererSink>;
  virtual ~AudioRendererSink() {}
};

}  // namespace media

#endif  // MEDIA_BASE_AUDIO_RENDERER_SINK_H_
