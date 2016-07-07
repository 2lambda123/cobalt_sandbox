/*
 * Copyright 2014 Google Inc. All Rights Reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "cobalt/base/fixed_no_free_allocator.h"
#include "cobalt/base/pointer_arithmetic.h"
#include "testing/gtest/include/gtest/gtest.h"

class FixedNoFreeAllocatorTest : public ::testing::Test {
 public:
  FixedNoFreeAllocatorTest();

 protected:
  static const size_t kAllocationSize = 8;
  static const size_t kAllocationAlignment = 8;
  static const size_t kMaxAllocations = 64;
  static const size_t kBufferSize = kAllocationSize * kMaxAllocations;

  char buffer_[kBufferSize];
  base::FixedNoFreeAllocator allocator_;
};

FixedNoFreeAllocatorTest::FixedNoFreeAllocatorTest()
    : allocator_(buffer_, kBufferSize) {}

TEST_F(FixedNoFreeAllocatorTest, CanDoSimpleAllocations) {
  void* allocation = allocator_.Allocate(kAllocationSize);

  EXPECT_GE(allocation, buffer_);
  EXPECT_LE(
      reinterpret_cast<uintptr_t>(allocation),
      reinterpret_cast<uintptr_t>(buffer_) + kBufferSize - kAllocationSize);
}

TEST_F(FixedNoFreeAllocatorTest, CanDoMultipleAllocationsProperly) {
  void* buffers[kMaxAllocations];
  for (int i = 0; i < kMaxAllocations; ++i) {
    buffers[i] = allocator_.Allocate(kAllocationSize);
    EXPECT_GE(buffers[i], buffer_);
    EXPECT_LE(
        reinterpret_cast<uintptr_t>(buffers[i]),
        reinterpret_cast<uintptr_t>(buffer_) + kBufferSize - kAllocationSize);

    // Make sure this allocation doesn't overlap with any previous ones.
    for (int j = 0; j < i; ++j) {
      EXPECT_NE(buffers[j], buffers[i]);
      if (buffers[j] < buffers[i]) {
        EXPECT_LE(base::AsInteger(buffers[j]) + kAllocationSize,
                  base::AsInteger(buffers[i]));
      } else {
        EXPECT_LE(base::AsInteger(buffers[i]) + kAllocationSize,
                  base::AsInteger(buffers[j]));
      }
    }
  }
}

TEST_F(FixedNoFreeAllocatorTest, CanDoMultipleAllocationsAndFreesProperly) {
  for (int i = 0; i < kMaxAllocations; ++i) {
    void* current_allocation = allocator_.Allocate(kAllocationSize);

    EXPECT_GE(current_allocation, buffer_);
    EXPECT_LE(
        reinterpret_cast<uintptr_t>(current_allocation),
        reinterpret_cast<uintptr_t>(buffer_) + kBufferSize - kAllocationSize);

    allocator_.Free(current_allocation);
  }
}

TEST_F(FixedNoFreeAllocatorTest, CanHandleOutOfMemory) {
  for (int i = 0; i < kMaxAllocations; ++i) {
    void* current_allocation = allocator_.Allocate(kAllocationSize);

    EXPECT_GE(current_allocation, buffer_);
    EXPECT_LE(
        reinterpret_cast<uintptr_t>(current_allocation),
        reinterpret_cast<uintptr_t>(buffer_) + kBufferSize - kAllocationSize);

    allocator_.Free(current_allocation);
  }

  // We should have exhausted our memory supply now, check that our next
  // allocation returns null.
  void* final_allocation = allocator_.Allocate(kAllocationSize);
  EXPECT_EQ(final_allocation, reinterpret_cast<void*>(NULL));
}

TEST_F(FixedNoFreeAllocatorTest, CanHandleAlignedMemory) {
  const int kMinimumAlignedMemoryAllocations =
      kBufferSize / (kAllocationSize + kAllocationAlignment);

  for (int i = 0; i < kMinimumAlignedMemoryAllocations; ++i) {
    void* current_allocation =
        allocator_.Allocate(kAllocationSize, kAllocationAlignment);
    EXPECT_EQ(0, reinterpret_cast<uintptr_t>(current_allocation) %
                     kAllocationAlignment);

    EXPECT_GE(current_allocation, buffer_);
    EXPECT_LE(
        reinterpret_cast<uintptr_t>(current_allocation),
        reinterpret_cast<uintptr_t>(buffer_) + kBufferSize - kAllocationSize);

    allocator_.Free(current_allocation);
  }
}
