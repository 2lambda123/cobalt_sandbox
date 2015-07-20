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

#include "cobalt/dom/media_key_error_event.h"

#include "cobalt/dom/event_names.h"

namespace cobalt {
namespace dom {

MediaKeyErrorEvent::MediaKeyErrorEvent(const std::string& key_system,
                                       const std::string& session_id,
                                       MediaKeyError::Code error_code,
                                       uint16 system_code)
    : Event(EventNames::GetInstance()->keyerror(), kNotBubbles, kNotCancelable),
      key_system_(key_system),
      session_id_(session_id),
      error_code_(static_cast<uint16>(error_code)),
      system_code_(system_code) {}

}  // namespace dom
}  // namespace cobalt
