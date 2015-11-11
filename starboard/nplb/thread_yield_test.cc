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

#include "starboard/nplb/thread_helpers.h"
#include "starboard/thread.h"
#include "starboard/time.h"
#include "testing/gtest/include/gtest/gtest.h"

using namespace starboard::nplb;

namespace {

// Returns whether a given index is a yielder for the given trial. We alternate
// whether 0 or 1 is a Yielder to avoid the first-started advantage.
inline bool IsYielder(int trial, int index) {
  return (trial % 2 ? (index % 2 != 0) : (index % 2 == 0));
}

// This number was experimentally determined on my desktop to be close to the
// minimum number of loops for the yielders to lose very consistently. The more
// loops, the more the yielders should fall behind.
const int kLoops = 1000;

void* YieldingEntryPoint(void* context) {
  for (int i = 0; i < kLoops; ++i) {
    SbThreadYield();
  }

  SbTimeMonotonic* end_time = static_cast<SbTimeMonotonic*>(context);
  *end_time = SbTimeGetMonotonicNow();
  return NULL;
}

void* UnyieldingEntryPoint(void* context) {
  for (int i = 0; i < kLoops; ++i) {
    DoNotYield();
  }

  SbTimeMonotonic* end_time = static_cast<SbTimeMonotonic*>(context);
  *end_time = SbTimeGetMonotonicNow();
  return NULL;
}

TEST(SbThreadYieldTest, SunnyDay) {
  SbThreadYield();
  // Well, my work here is done.
}

// Okay, okay, I'm not sure how else to test this other than to try to make sure
// that a thread that yields generally gets more CPU time than one that doesn't.
//
// I did test that racing Unyielding threads against each other causes this test
// to fail regularly. By rerunning the test kTrials times, and by swapping which
// thread gets started first, I hope to make this inherently flaky test not
// flaky.
TEST(SbThreadYieldTest, SunnyDayRace) {
  const int kTrials = 30;
  for (int trial = 0; trial < kTrials; ++trial) {
    // Pin to CPU 0 to make sure the threads don't get distributed onto other
    // cores.
    SbThreadAffinity affinity = 0;
    // We want enough racers such that the threads must contend for cpu time,
    // and enough data for the averages to be consistently divergent.
    const int64_t kRacers = 16;
    SbThread threads[kRacers];
    SbTimeMonotonic end_times[kRacers] = {0};
    for (int i = 0; i < kRacers; ++i) {
      threads[i] = SbThreadCreate(
          0, kSbThreadNoPriority, affinity, true, NULL,
          (IsYielder(trial, i) ? YieldingEntryPoint : UnyieldingEntryPoint),
          &(end_times[i]));
    }

    for (int i = 0; i < kRacers; ++i) {
      EXPECT_TRUE(SbThreadIsValid(threads[i])) << "thread = " << threads[i];
    }

    for (int i = 0; i < kRacers; ++i) {
      EXPECT_TRUE(SbThreadJoin(threads[i], NULL));
    }

    // On average, Unyielders should finsh sooner than Yielders.
    SbTimeMonotonic average_yielder = 0;
    SbTimeMonotonic average_unyielder = 0;
    const int64_t kRacersPerGroup = kRacers / 2;
    for (int i = 0; i < kRacers; ++i) {
      if (IsYielder(trial, i)) {
        average_yielder += end_times[i] / kRacersPerGroup;
      } else {
        average_unyielder += end_times[i] / kRacersPerGroup;
      }
    }

    EXPECT_LT(average_unyielder, average_yielder) << "Trial " << trial;
  }
}

}  // namespace
