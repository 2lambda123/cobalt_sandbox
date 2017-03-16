// Copyright 2016 Google Inc. All Rights Reserved.
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

#include "cobalt/renderer/backend/default_graphics_system.h"

#include "cobalt/renderer/backend/egl/graphics_system.h"
#include "cobalt/system_window/system_window.h"

namespace cobalt {
namespace renderer {
namespace backend {

scoped_ptr<GraphicsSystem> CreateDefaultGraphicsSystem() {
  return scoped_ptr<GraphicsSystem>(new GraphicsSystemEGL());
}

EGLNativeWindowType GetHandleFromSystemWindow(
    system_window::SystemWindow* system_window) {
  return (EGLNativeWindowType)(system_window->GetWindowHandle());
}

}  // namespace backend
}  // namespace renderer
}  // namespace cobalt
