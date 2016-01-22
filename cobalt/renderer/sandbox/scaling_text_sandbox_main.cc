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

#include "base/at_exit.h"
#include "base/debug/trace_event.h"
#include "base/logging.h"
#include "base/memory/ref_counted.h"
#include "cobalt/base/init_cobalt.h"
#include "cobalt/renderer/renderer_module.h"
#include "cobalt/renderer/test/scenes/scaling_text_scene.h"
#include "cobalt/system_window/system_window.h"
#include "cobalt/trace_event/scoped_trace_to_file.h"

using cobalt::render_tree::ResourceProvider;
using cobalt::renderer::test::scenes::AddBlankBackgroundToScene;
using cobalt::renderer::test::scenes::CreateScalingTextScene;
using cobalt::renderer::test::scenes::RenderTreeWithAnimations;
using cobalt::system_window::SystemWindow;

int main(int argc, char** argv) {
  base::AtExitManager at_exit;
  cobalt::InitCobalt(argc, argv);

  cobalt::trace_event::ScopedTraceToFile trace_to_file(
      FilePath(FILE_PATH_LITERAL("scaling_text_sandbox_trace.json")));

  base::EventDispatcher event_dispatcher;
  // Create a system window to use as a render target.
  scoped_ptr<SystemWindow> system_window =
      cobalt::system_window::CreateSystemWindow(&event_dispatcher);

  // Construct a renderer module using default options.
  cobalt::renderer::RendererModule::Options renderer_module_options;
  cobalt::renderer::RendererModule renderer_module(system_window.get(),
                                                   renderer_module_options);

  cobalt::math::SizeF output_dimensions(
      renderer_module.render_target()->GetSurfaceInfo().size);

  // Construct our render tree and associated animations to be passed into
  // the renderer pipeline for display.
  base::TimeDelta start_time = base::Time::Now() - base::Time::UnixEpoch();
  RenderTreeWithAnimations scene = AddBlankBackgroundToScene(
      CreateScalingTextScene(renderer_module.pipeline()->GetResourceProvider(),
                             output_dimensions, start_time),
      output_dimensions);

  // Pass the render tree along with associated animations into the renderer
  // module to be displayed.
  renderer_module.pipeline()->Submit(cobalt::renderer::Pipeline::Submission(
      scene.render_tree, scene.animations, start_time));

  base::PlatformThread::Sleep(base::TimeDelta::FromSeconds(30));

  return 0;
}
