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

#ifndef RENDERER_BACKEND_GRAPHICS_SYSTEM_H_
#define RENDERER_BACKEND_GRAPHICS_SYSTEM_H_

#include "base/memory/ref_counted.h"
#include "base/memory/scoped_ptr.h"

#include "cobalt/renderer/backend/display.h"
#include "cobalt/renderer/backend/graphics_context.h"
#include "cobalt/renderer/backend/render_target.h"

namespace cobalt {
namespace renderer {
namespace backend {

// The GraphicsSystem is the entry point for the Cobalt graphics API.
// The first step for doing any graphics-related activity on a Cobalt platform
// is to construct a GraphicsSystem object, which acts not only as a factory
// for other graphics system objects the user may wish to create, but also as a
// central home for any platform-specific global support objects.
// Typically a GraphicsSystem will be constructed via a call to
// CreateDefaultGraphicsSystem() which should be implemented on every Cobalt
// platform, even if it simply returns a GraphicsSystemNull object.
// Most often, there will be one GraphicsSystem per platform.
class GraphicsSystem {
 public:
  virtual ~GraphicsSystem() {}

  // Creates whatever the default display is for this graphics system.
  // Typically this would construct a Display associated with the TV or monitor
  // a device is connected to, but for desktop operating systems this may also
  // create a Window and refer to that.
  virtual scoped_ptr<Display> CreateDefaultDisplay() = 0;

  // Creates a graphics context that can be used to issue graphics commands to
  // the hardware.
  virtual scoped_ptr<GraphicsContext> CreateGraphicsContext(
      const scoped_refptr<RenderTarget>& render_target) = 0;
};

}  // namespace backend
}  // namespace renderer
}  // namespace cobalt

#endif  // RENDERER_BACKEND_GRAPHICS_SYSTEM_H_
