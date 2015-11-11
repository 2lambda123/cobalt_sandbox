// Copyright 2015 Google Inc. All Rights Reserved.
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

#include "starboard/condition_variable.h"

#include <errno.h>
#include <pthread.h>
#include <time.h>

#include "starboard/shared/posix/time_internal.h"
#include "starboard/shared/pthread/is_success.h"
#include "starboard/time.h"

SbConditionVariableResult SbConditionVariableWaitTimed(
    SbConditionVariable* condition,
    SbMutex* mutex,
    SbTime timeout) {
  if (!condition || !mutex) {
    return kSbConditionVariableFailed;
  }

  if (timeout < 0) {
    timeout = 0;
  }

  struct timespec timeout_ts;
  ToTimespec(&timeout_ts, SbTimeToPosix(SbTimeGetNow() + timeout));

  int result = pthread_cond_timedwait(condition, mutex, &timeout_ts);
  if (IsSuccess(result)) {
    return kSbConditionVariableSignaled;
  }

  if (result == ETIMEDOUT) {
    return kSbConditionVariableTimedOut;
  }

  return kSbConditionVariableFailed;
}
