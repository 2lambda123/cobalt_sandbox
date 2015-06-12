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

#ifndef DOM_SOURCE_BUFFER_H_
#define DOM_SOURCE_BUFFER_H_

#include <string>

#include "base/memory/ref_counted.h"
#include "cobalt/dom/time_ranges.h"
#include "cobalt/dom/uint8_array.h"
#include "cobalt/script/wrappable.h"

namespace cobalt {
namespace dom {

class MediaSource;

// The SourceBuffer interface exposes the media source buffer so its user can
// append media data to.
//   https://rawgit.com/w3c/media-source/6747ed7a3206f646963d760100b0f37e2fc7e47e/media-source.html#sourcebuffer
class SourceBuffer : public script::Wrappable {
 public:
  // Custom, not in any spec.
  //
  SourceBuffer(const scoped_refptr<MediaSource>& media_source,
               const std::string& id);

  // Web API: SourceBuffer
  //
  scoped_refptr<TimeRanges> buffered() const;
  double timestamp_offset() const;
  void set_timestamp_offset(double offset);
  void Append(const scoped_refptr<Uint8Array>& data);
  void Abort();

  // Custom, not in any spec.
  //
  const std::string& id() const { return id_; }
  // Detach it from the MediaSource object.
  void Close();

  DEFINE_WRAPPABLE_TYPE(SourceBuffer);

 private:
  scoped_refptr<MediaSource> media_source_;
  const std::string id_;
  double timestamp_offset_;
};

}  // namespace dom
}  // namespace cobalt

#endif  // DOM_SOURCE_BUFFER_H_
