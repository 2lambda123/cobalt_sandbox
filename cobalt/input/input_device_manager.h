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

#ifndef INPUT_INPUT_DEVICE_MANAGER_H_
#define INPUT_INPUT_DEVICE_MANAGER_H_

#include "base/callback.h"
#include "base/memory/scoped_ptr.h"
#include "cobalt/dom/keyboard_event.h"

namespace cobalt {

// Forward-declaring SystemWindow here, as some implementations (e.g. consoles)
// will never need to know about the class.
namespace system_window {
class SystemWindow;
}

namespace input {

typedef base::Callback<void(const scoped_refptr<dom::KeyboardEvent>&)>
    KeyboardEventCallback;

// InputDeviceManager listens to events from platform-specific input devices,
// maps them to platform-independent keyboard key events and sends them to the
// client via a callback provided upon construction.
class InputDeviceManager {
 public:
  // Creates an instance using a SystemWindow parameter.
  // This allows us to hook up keyboard events on desktop systems.
  static scoped_ptr<InputDeviceManager> CreateFromWindow(
      const KeyboardEventCallback& callback,
      system_window::SystemWindow* system_window);

  virtual ~InputDeviceManager() {}

 protected:
  explicit InputDeviceManager(const KeyboardEventCallback& callback)
      : keyboard_event_callback_(callback) {}

  KeyboardEventCallback keyboard_event_callback_;
};

}  // namespace input
}  // namespace cobalt

#endif  // INPUT_INPUT_DEVICE_MANAGER_H_
