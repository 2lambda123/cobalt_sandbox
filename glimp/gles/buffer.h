/*
 * Copyright 2015 Google Inc. All Rights Reserved.
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

#ifndef GLIMP_GLES_BUFFER_H_
#define GLIMP_GLES_BUFFER_H_

#include <GLES3/gl3.h>

#include "glimp/gles/buffer_impl.h"
#include "glimp/nb/ref_counted.h"
#include "glimp/nb/scoped_ptr.h"

namespace glimp {
namespace gles {

class Buffer : public nb::RefCountedThreadSafe<Buffer> {
 public:
  explicit Buffer(nb::scoped_ptr<BufferImpl> impl);

  void SetTarget(GLenum target);

  // Implements support for glBufferData() on this buffer object.
  void BufferData(GLsizeiptr size, const GLvoid* data, GLenum usage);

  // Returns true if the target has been set (e.g. via glBindBuffer()).
  bool target_valid() const { return target_valid_; }

  // Returns the target (set via glBindBuffer()).  Must be called only if
  // target_valid() is true.
  GLenum target() const {
    SB_DCHECK(target_valid_);
    return target_;
  }

 private:
  friend class nb::RefCountedThreadSafe<Buffer>;
  ~Buffer() {}

  nb::scoped_ptr<BufferImpl> impl_;

  // The target type this buffer was last bound as, set through a call to
  // glBindBuffer().
  GLenum target_;

  // Represents whether or not target_ as been initialized yet.
  bool target_valid_;
};

}  // namespace gles
}  // namespace glimp

#endif  // GLIMP_GLES_BUFFER_H_
