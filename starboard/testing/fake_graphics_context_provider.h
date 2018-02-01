// Copyright 2018 Google Inc. All Rights Reserved.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#ifndef STARBOARD_TESTING_FAKE_GRAPHICS_CONTEXT_PROVIDER_H_
#define STARBOARD_TESTING_FAKE_GRAPHICS_CONTEXT_PROVIDER_H_

#include <functional>

#include "starboard/condition_variable.h"
#include "starboard/decode_target.h"
#include "starboard/mutex.h"
#include "starboard/queue.h"
#include "starboard/thread.h"

namespace starboard {
namespace testing {

// This class provides a SbDecodeTargetGraphicsContextProvider implementation
// used by SbPlayer related tests.  It creates a thread and forwards decode
// target creation/destruction to the thread.
class FakeGraphicsContextProvider {
 public:
  FakeGraphicsContextProvider();
  ~FakeGraphicsContextProvider();

  SbDecodeTargetGraphicsContextProvider* decoder_target_provider() {
#if SB_HAS(BLITTER) || SB_HAS(GLES2)
    return &decoder_target_provider_;
#else   // SB_HAS(BLITTER) || SB_HAS(GLES2)
    return NULL;
#endif  // SB_HAS(BLITTER) || SB_HAS(GLES2)
  }

#if SB_HAS(BLITTER) || SB_HAS(GLES2)
  void ReleaseDecodeTarget(SbDecodeTarget decode_target);
#endif  // SB_HAS(BLITTER) || SB_HAS(GLES2)

 private:
  static void* ThreadEntryPoint(void* context);
  void RunLoop();

#if SB_HAS(GLES2)
  void ReleaseDecodeTargetOnGlesContextThread(
      Mutex* mutex,
      ConditionVariable* condition_variable,
      SbDecodeTarget decode_target);
  void RunDecodeTargetFunctionOnGlesContextThread(
      Mutex* mutex,
      ConditionVariable* condition_variable,
      SbDecodeTargetGlesContextRunnerTarget target_function,
      void* target_function_context);

  void OnDecodeTargetGlesContextRunner(
      SbDecodeTargetGlesContextRunnerTarget target_function,
      void* target_function_context);

  static void DecodeTargetGlesContextRunner(
      SbDecodeTargetGraphicsContextProvider* graphics_context_provider,
      SbDecodeTargetGlesContextRunnerTarget target_function,
      void* target_function_context);
#endif  // SB_HAS(GLES2)

#if SB_HAS(BLITTER) || SB_HAS(GLES2)
  Queue<std::function<void()>> functor_queue_;
  SbThread decode_target_context_thread_;
  SbDecodeTargetGraphicsContextProvider decoder_target_provider_;
#endif  // SB_HAS(BLITTER) || SB_HAS(GLES2)
};

}  // namespace testing
}  // namespace starboard

#endif  // STARBOARD_TESTING_FAKE_GRAPHICS_CONTEXT_PROVIDER_H_
