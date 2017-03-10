// Copyright 2017 Google Inc. All Rights Reserved.
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

#ifndef COBALT_MEDIA_DECODER_BUFFER_ALLOCATOR_H_
#define COBALT_MEDIA_DECODER_BUFFER_ALLOCATOR_H_

#include "base/compiler_specific.h"
#include "cobalt/media/base/decoder_buffer.h"
#include "nb/memory_pool.h"

namespace cobalt {
namespace media {

class DecoderBufferAllocator : public DecoderBuffer::Allocator {
 public:
  DecoderBufferAllocator();
  ~DecoderBufferAllocator() OVERRIDE;

  void* Allocate(Type type, size_t size, size_t alignment) OVERRIDE;
  void Free(Type type, void* ptr) OVERRIDE;

 private:
  void* memory_block_;
  nb::MemoryPool memory_pool_;
};

}  // namespace media
}  // namespace cobalt

#endif  // COBALT_MEDIA_DECODER_BUFFER_ALLOCATOR_H_
