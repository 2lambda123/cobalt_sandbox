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

#include "cobalt/dom/html_video_element.h"

#include "base/logging.h"
#include "base/string_number_conversions.h"

namespace cobalt {
namespace dom {

using ::media::ShellVideoFrameProvider;

const char HTMLVideoElement::kTagName[] = "video";

HTMLVideoElement::HTMLVideoElement(Document* document)
    : HTMLMediaElement(document) {}

std::string HTMLVideoElement::tag_name() const { return kTagName; }

uint32 HTMLVideoElement::width() const {
  uint32 result = 0;
  std::string value_in_string = GetAttribute("width").value_or("0");
  if (!base::StringToUint32(value_in_string, &result)) {
    LOG(WARNING) << "Invalid width attribute: \'" << value_in_string << "\'";
  }
  return result;
}

uint32 HTMLVideoElement::height() const {
  uint32 result = 0;
  std::string value_in_string = GetAttribute("height").value_or("0");
  if (!base::StringToUint32(value_in_string, &result)) {
    LOG(WARNING) << "Invalid height attribute: \'" << value_in_string << "\'";
  }
  return result;
}

void HTMLVideoElement::set_width(uint32 width) {
  SetAttribute("width", base::Uint32ToString(width));
}

void HTMLVideoElement::set_height(uint32 height) {
  SetAttribute("height", base::Uint32ToString(height));
}

uint32 HTMLVideoElement::video_width() const {
  if (!player()) {
    return 0u;
  }
  DCHECK_GE(player()->GetNaturalSize().width(), 0);
  return static_cast<uint32>(player()->GetNaturalSize().width());
}

uint32 HTMLVideoElement::video_height() const {
  if (!player()) {
    return 0u;
  }
  DCHECK_GE(player()->GetNaturalSize().height(), 0);
  return static_cast<uint32>(player()->GetNaturalSize().height());
}

scoped_refptr<ShellVideoFrameProvider>
HTMLVideoElement::GetVideoFrameProvider() {
  DCHECK(thread_checker_.CalledOnValidThread());
  return player() ? player()->GetVideoFrameProvider() : NULL;
}

}  // namespace dom
}  // namespace cobalt
