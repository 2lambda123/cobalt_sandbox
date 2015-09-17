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

#include "starboard/memory.h"
#include "testing/gtest/include/gtest/gtest.h"

namespace {

const size_t kSize = 1024 * 128;

TEST(SbMemoryFreeAlignedTest, FreesAligned) {
  const size_t kMaxAlign = 4096 + 1;
  for (size_t align = 2; align < kMaxAlign; align <<= 1) {
    void *memory = SbMemoryAllocateAligned(align, kSize);
    SbMemoryFreeAligned(memory);
  }
}

TEST(SbMemoryFreeAlignedTest, FreesNull) {
  SbMemoryFreeAligned(NULL);
}

}  // namespace
