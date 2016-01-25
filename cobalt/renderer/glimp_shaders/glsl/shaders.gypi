# Copyright 2016 Google Inc. All Rights Reserved.
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

# This file defines 'glsl_keys', which is an input to glimp's
# map_glsl_shaders.gypi tool used to map GLSL shaders to platform-specific
# shaders, mapping them by filename.  The 'glsl_keys' variable defined here
# lists all GLSL shaders intended to be used by Cobalt.  Make sure that this
# is included before including 'glimp/map_glsl_shaders.gypi'.

{
  'variables': {
    'glsl_shaders': [
      # A simple shader allowing for full-screen quad blitting, used to enable
      # the transfer of a software-rasterized image to the display.
      '<(DEPTH)/cobalt/renderer/glimp_shaders/glsl/fragment_position_and_texcoord.glsl',
      '<(DEPTH)/cobalt/renderer/glimp_shaders/glsl/fragment_skia_alpha_texcoords_and_color.glsl',
      '<(DEPTH)/cobalt/renderer/glimp_shaders/glsl/fragment_skia_alpha_texcoords_and_color_2.glsl',
      '<(DEPTH)/cobalt/renderer/glimp_shaders/glsl/fragment_skia_antialiased_circle.glsl',
      '<(DEPTH)/cobalt/renderer/glimp_shaders/glsl/fragment_skia_antialiased_color_only.glsl',
      '<(DEPTH)/cobalt/renderer/glimp_shaders/glsl/fragment_skia_antialiased_oval.glsl',
      '<(DEPTH)/cobalt/renderer/glimp_shaders/glsl/fragment_skia_circle_masked_texture.glsl',
      '<(DEPTH)/cobalt/renderer/glimp_shaders/glsl/fragment_skia_color_only.glsl',
      '<(DEPTH)/cobalt/renderer/glimp_shaders/glsl/fragment_skia_distance_field_texture.glsl',
      '<(DEPTH)/cobalt/renderer/glimp_shaders/glsl/fragment_skia_distance_field_texture_non_similar.glsl',
      '<(DEPTH)/cobalt/renderer/glimp_shaders/glsl/fragment_skia_ellipse_masked_texture.glsl',
      '<(DEPTH)/cobalt/renderer/glimp_shaders/glsl/fragment_skia_linear_gradient_many_colors.glsl',
      '<(DEPTH)/cobalt/renderer/glimp_shaders/glsl/fragment_skia_linear_gradient_three_colors.glsl',
      '<(DEPTH)/cobalt/renderer/glimp_shaders/glsl/fragment_skia_linear_gradient_two_colors.glsl',
      '<(DEPTH)/cobalt/renderer/glimp_shaders/glsl/fragment_skia_texcoords_and_color.glsl',
      '<(DEPTH)/cobalt/renderer/glimp_shaders/glsl/fragment_skia_texcoords_and_color_with_edge_clamped_texels.glsl',
      '<(DEPTH)/cobalt/renderer/glimp_shaders/glsl/fragment_skia_texture_masked_texture.glsl',
      '<(DEPTH)/cobalt/renderer/glimp_shaders/glsl/fragment_skia_yuv.glsl',
      '<(DEPTH)/cobalt/renderer/glimp_shaders/glsl/vertex_position_and_texcoord.glsl',
      '<(DEPTH)/cobalt/renderer/glimp_shaders/glsl/vertex_skia_antialiased_circle.glsl',
      '<(DEPTH)/cobalt/renderer/glimp_shaders/glsl/vertex_skia_antialiased_color_only.glsl',
      '<(DEPTH)/cobalt/renderer/glimp_shaders/glsl/vertex_skia_antialiased_oval.glsl',
      '<(DEPTH)/cobalt/renderer/glimp_shaders/glsl/vertex_skia_color_only.glsl',
      '<(DEPTH)/cobalt/renderer/glimp_shaders/glsl/vertex_skia_texcoords_and_color.glsl',
      '<(DEPTH)/cobalt/renderer/glimp_shaders/glsl/vertex_skia_texcoords_and_color_with_texcoord_matrix.glsl',
      '<(DEPTH)/cobalt/renderer/glimp_shaders/glsl/vertex_skia_texcoords_derived_from_position.glsl',
      '<(DEPTH)/cobalt/renderer/glimp_shaders/glsl/vertex_skia_two_texcoords_and_color_with_texcoord_matrices.glsl',
      '<(DEPTH)/cobalt/renderer/glimp_shaders/glsl/vertex_skia_yuv.glsl',
    ],
  }
}
