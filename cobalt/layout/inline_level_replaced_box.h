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

#ifndef LAYOUT_INLINE_LEVEL_REPLACED_BOX_H_
#define LAYOUT_INLINE_LEVEL_REPLACED_BOX_H_

#include "base/basictypes.h"
#include "base/memory/ref_counted.h"
#include "base/optional.h"
#include "cobalt/cssom/css_style_declaration_data.h"
#include "cobalt/cssom/css_transition_set.h"
#include "cobalt/layout/box.h"
#include "cobalt/layout/paragraph.h"
#include "cobalt/layout/replaced_box.h"

namespace cobalt {
namespace layout {

class UsedStyleProvider;

class InlineLevelReplacedBox : public ReplacedBox {
 public:
  InlineLevelReplacedBox(
      const scoped_refptr<cssom::ComputedStyleState>& computed_style_state,
      const ReplaceImageCB& replace_image_cb,
      const scoped_refptr<Paragraph>& paragraph, int32 text_position,
      const base::optional<float>& maybe_intrinsic_width,
      const base::optional<float>& maybe_intrinsic_height,
      const base::optional<float>& maybe_intrinsic_ratio,
      UsedStyleProvider* used_style_provider);

  // From |Box|.
  Level GetLevel() const OVERRIDE;

  bool DoesFulfillEllipsisPlacementRequirement() const OVERRIDE;
  void ResetEllipses() OVERRIDE;
  bool IsHiddenByEllipsis() const OVERRIDE;

 protected:
  // From |Box|.
#ifdef COBALT_BOX_DUMP_ENABLED
  void DumpClassName(std::ostream* stream) const OVERRIDE;
#endif  // COBALT_BOX_DUMP_ENABLED

  // From |ReplacedBox|.
  void UpdateHorizontalMargins(
      float containing_block_width, float border_box_width,
      const base::optional<float>& maybe_margin_left,
      const base::optional<float>& maybe_margin_right) OVERRIDE;

 private:
  // From |Box|.
  void DoPlaceEllipsisOrProcessPlacedEllipsis(
      float desired_offset, bool* is_placement_requirement_met, bool* is_placed,
      float* placed_offset) OVERRIDE;

  // This flag indicates that the box is fully hidden by the ellipsis and it,
  // along with its contents will not be visible.
  // "Implementations must hide characters and atomic inline-level elements at
  // the applicable edge(s) of the line as necessary to fit the ellipsis."
  //   https://www.w3.org/TR/css3-ui/#propdef-text-overflow
  bool is_hidden_by_ellipsis_;
};

}  // namespace layout
}  // namespace cobalt

#endif  // LAYOUT_INLINE_LEVEL_REPLACED_BOX_H_
