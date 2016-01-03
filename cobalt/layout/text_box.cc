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

#include "cobalt/layout/text_box.h"

#include <algorithm>
#include <limits>

#include "cobalt/cssom/keyword_value.h"
#include "cobalt/layout/math.h"
#include "cobalt/layout/used_style.h"
#include "cobalt/math/transform_2d.h"
#include "cobalt/render_tree/text_node.h"

namespace cobalt {
namespace layout {

TextBox::TextBox(
    const scoped_refptr<cssom::ComputedStyleState>& computed_style_state,
    const scoped_refptr<Paragraph>& paragraph, int32 text_start_position,
    int32 text_end_position, bool has_trailing_line_break,
    UsedStyleProvider* used_style_provider)
    : Box(computed_style_state, used_style_provider),
      paragraph_(paragraph),
      text_start_position_(text_start_position),
      text_end_position_(text_end_position),
      truncated_text_end_position_(text_end_position),
      used_font_(used_style_provider->GetUsedFontList(
          computed_style_state->style()->font_family(),
          computed_style_state->style()->font_size(),
          computed_style_state->style()->font_style(),
          computed_style_state->style()->font_weight())),
      text_has_leading_white_space_(false),
      text_has_trailing_white_space_(false),
      should_collapse_leading_white_space_(false),
      should_collapse_trailing_white_space_(false),
      has_trailing_line_break_(has_trailing_line_break),
      update_size_results_valid_(false) {
  DCHECK(text_start_position_ <= text_end_position_);

  UpdateTextHasLeadingWhiteSpace();
  UpdateTextHasTrailingWhiteSpace();
}

Box::Level TextBox::GetLevel() const { return kInlineLevel; }

bool TextBox::ValidateUpdateSizeInputs(const LayoutParams& params) {
  // Also take into account mutable local state about (at least) whether white
  // space should be collapsed or not.
  if (Box::ValidateUpdateSizeInputs(params) && update_size_results_valid_) {
    return true;
  } else {
    update_size_results_valid_ = true;
    return false;
  }
}

void TextBox::UpdateContentSizeAndMargins(const LayoutParams& layout_params) {
  // Anonymous boxes do not have margins.
  DCHECK_EQ(0.0f, GetUsedMarginLeftIfNotAuto(
                      computed_style(), layout_params.containing_block_size));
  DCHECK_EQ(0.0f, GetUsedMarginTopIfNotAuto(
                      computed_style(), layout_params.containing_block_size));
  DCHECK_EQ(0.0f, GetUsedMarginRightIfNotAuto(
                      computed_style(), layout_params.containing_block_size));
  DCHECK_EQ(0.0f, GetUsedMarginBottomIfNotAuto(
                      computed_style(), layout_params.containing_block_size));

  set_margin_left(0);
  set_margin_top(0);
  set_margin_right(0);
  set_margin_bottom(0);

  float non_collapsible_text_width =
      HasNonCollapsibleText()
          ? RoundToFixedPointPrecision(
                used_font_->GetBounds(GetNonCollapsibleText()).width())
          : 0;
  set_width(GetLeadingWhiteSpaceWidth() + non_collapsible_text_width +
            GetTrailingWhiteSpaceWidth());

  if (!baseline_offset_from_top_) {
    UsedLineHeightProvider used_line_height_provider(
        used_font_->GetFontMetrics());
    computed_style()->line_height()->Accept(&used_line_height_provider);
    set_height(used_line_height_provider.used_line_height());
    baseline_offset_from_top_ =
        used_line_height_provider.baseline_offset_from_top();
  }
}

scoped_refptr<Box> TextBox::TrySplitAt(float available_width,
                                       bool allow_overflow) {
  if (!WhiteSpaceStyleAllowsWrapping()) {
    return scoped_refptr<Box>();
  }

  // Start from the text position when searching for the split position. We do
  // not want to split on leading whitespace. Additionally, as a result of
  // skipping over it, the width of the leading whitespace will need to be
  // removed from the available width.
  available_width -= GetLeadingWhiteSpaceWidth();
  int32 start_position = GetNonCollapsibleTextStartPosition();
  int32 split_position;
  float split_width;

  Paragraph::BreakPolicy break_policy;
  if (computed_style()->overflow_wrap() ==
      cssom::KeywordValue::GetBreakWord()) {
    break_policy = Paragraph::kSoftWrapWithBreakWordOnOverflow;
  } else {
    break_policy = Paragraph::kSoftWrap;
  }

  if (paragraph_->FindBreakPosition(
          used_font_, start_position, text_end_position_, available_width,
          allow_overflow, break_policy, &split_position, &split_width)) {
    return SplitAtPosition(split_position);
  }

  return scoped_refptr<Box>();
}

bool TextBox::DoesFulfillEllipsisPlacementRequirement() const {
  // This box has non-collapsed text and fulfills the requirement that the first
  // character or inline-level element must appear on the line before ellipsing
  // can occur if it has non-collapsed characters.
  //   http://www.w3.org/TR/css3-ui/#propdef-text-overflow
  return GetNonCollapsedTextStartPosition() < GetNonCollapsedTextEndPosition();
}

void TextBox::ResetEllipses() {
  truncated_text_end_position_ = text_end_position_;
}

void TextBox::SplitBidiLevelRuns() {}

scoped_refptr<Box> TextBox::TrySplitAtSecondBidiLevelRun() {
  int32 split_position;
  if (paragraph_->GetNextRunPosition(text_start_position_, &split_position) &&
      split_position < text_end_position_) {
    return SplitAtPosition(split_position);
  } else {
    return scoped_refptr<Box>();
  }
}

base::optional<int> TextBox::GetBidiLevel() const {
  return paragraph_->GetBidiLevel(text_start_position_);
}

void TextBox::SetShouldCollapseLeadingWhiteSpace(
    bool should_collapse_leading_white_space) {
  if (should_collapse_leading_white_space_ !=
      should_collapse_leading_white_space) {
    should_collapse_leading_white_space_ = should_collapse_leading_white_space;
    update_size_results_valid_ = false;
  }
}

void TextBox::SetShouldCollapseTrailingWhiteSpace(
    bool should_collapse_trailing_white_space) {
  if (should_collapse_trailing_white_space_ !=
      should_collapse_trailing_white_space) {
    should_collapse_trailing_white_space_ =
        should_collapse_trailing_white_space;
    update_size_results_valid_ = false;
  }
}

bool TextBox::IsCollapsed() const {
  return !HasLeadingWhiteSpace() && !HasTrailingWhiteSpace() &&
         !HasNonCollapsibleText() && !has_trailing_line_break_;
}

bool TextBox::HasLeadingWhiteSpace() const {
  return text_has_leading_white_space_ &&
         !should_collapse_leading_white_space_ &&
         (HasNonCollapsibleText() || !should_collapse_trailing_white_space_);
}

bool TextBox::HasTrailingWhiteSpace() const {
  return text_has_trailing_white_space_ &&
         !should_collapse_trailing_white_space_ &&
         (HasNonCollapsibleText() || !should_collapse_leading_white_space_);
}

bool TextBox::JustifiesLineExistence() const {
  return HasNonCollapsibleText() || has_trailing_line_break_;
}

bool TextBox::HasTrailingLineBreak() const { return has_trailing_line_break_; }

bool TextBox::AffectsBaselineInBlockFormattingContext() const {
  NOTREACHED() << "Should only be called in a block formatting context.";
  return true;
}

float TextBox::GetBaselineOffsetFromTopMarginEdge() const {
  return *baseline_offset_from_top_;
}

void TextBox::RenderAndAnimateContent(
    render_tree::CompositionNode::Builder* border_node_builder,
    render_tree::animations::NodeAnimationsMap::Builder*
        node_animations_map_builder) const {
  UNREFERENCED_PARAMETER(node_animations_map_builder);

  if (computed_style()->visibility() != cssom::KeywordValue::GetVisible()) {
    return;
  }

  DCHECK_EQ(0, border_left_width() + padding_left());
  DCHECK_EQ(0, border_top_width() + padding_top());

  // Only add the text node to the render tree if it actually has content and
  // a font isn't loading. The font is treated as transparent if a font is
  // currently being downloaded and hasn't timed out: "In cases where textual
  // content is loaded before downloadable fonts are available, user agents
  // may... render text transparently with fallback fonts to avoid a flash of
  // text using a fallback font. In cases where the font download fails user
  // agents must display text, simply leaving transparent text is considered
  // non-conformant behavior."
  //   http://www.w3.org/TR/css3-fonts/#font-face-loading
  if (HasVisibleText() && used_font_->IsVisible()) {
    render_tree::ColorRGBA used_color = GetUsedColor(computed_style()->color());

    // Only render the text if it is not completely transparent.
    if (used_color.a() > 0.0f) {
      std::string text = GetVisibleText();
      dom::FontRunList font_run_list;
      used_font_->GenerateFontRunList(text, &font_run_list);

      float leading_width = GetLeadingWhiteSpaceWidth();

      for (size_t i = 0; i < font_run_list.size(); ++i) {
        const dom::FontRun& font_run = font_run_list[i];

        std::string font_string =
            text.substr(font_run.start_position, font_run.length);

        // The render tree API considers text coordinates to be a position of
        // a baseline, offset the text node accordingly.
        border_node_builder->AddChild(
            new render_tree::TextNode(font_string, font_run.font, used_color),
            math::TranslateMatrix(leading_width, *baseline_offset_from_top_));

        if (i < font_run_list.size() - 1) {
          leading_width += font_run.font->GetBounds(font_string).width();
        }
      }
    }
  }
}

bool TextBox::IsTransformable() const { return false; }

#ifdef COBALT_BOX_DUMP_ENABLED

void TextBox::DumpClassName(std::ostream* stream) const {
  *stream << "TextBox ";
}

void TextBox::DumpProperties(std::ostream* stream) const {
  Box::DumpProperties(stream);

  *stream << "text_start=" << text_start_position_ << " "
          << "text_end=" << text_end_position_ << " ";

  *stream << std::boolalpha
          << "has_leading_white_space=" << HasLeadingWhiteSpace() << " "
          << "has_trailing_white_space=" << HasTrailingWhiteSpace() << " "
          << std::noboolalpha;

  *stream << "bidi_level=" << paragraph_->GetBidiLevel(text_start_position_)
          << " ";
}

void TextBox::DumpChildrenWithIndent(std::ostream* stream, int indent) const {
  Box::DumpChildrenWithIndent(stream, indent);

  DumpIndent(stream, indent);

  *stream << "\"" << GetNonCollapsibleText() << "\"\n";
}

#endif  // COBALT_BOX_DUMP_ENABLED

void TextBox::DoPlaceEllipsisOrProcessPlacedEllipsis(
    float desired_offset, bool* is_placement_requirement_met, bool* is_placed,
    float* placed_offset) {
  // If the ellipsis has already been placed, then the text is fully truncated
  // by the ellipsis.
  if (*is_placed) {
    truncated_text_end_position_ = text_start_position_;
    return;
  }

  // Otherwise, the ellipsis is being placed somewhere within this text box.
  *is_placed = true;

  // Initially subtract the box's offset from the containing block from the
  // ellipsis's desired offset. Paragraph::FindBreakPosition() searches for
  // a break position from the start of the non-collapsed portion of the the
  // box's text, and not from the start of the containing block.
  float content_box_offset = GetContentBoxLeftEdgeOffsetFromContainingBlock();
  desired_offset -= content_box_offset;

  int32 start_position = GetNonCollapsedTextStartPosition();
  int32 end_position = GetNonCollapsedTextEndPosition();
  int32 found_position;
  float found_offset;

  // Attempt to find a break position allowing breaks anywhere within the text,
  // and not simply at soft wrap locations. If the placement requirement has
  // already been satisfied, then the ellipsis can appear anywhere within the
  // text box. Otherwise, it can only appear after the first character
  // (http://www.w3.org/TR/css3-ui/#propdef-text-overflow).
  if (paragraph_->FindBreakPosition(
          used_font_, start_position, end_position, desired_offset,
          !(*is_placement_requirement_met), Paragraph::kBreakWord,
          &found_position, &found_offset)) {
    *placed_offset = found_offset + content_box_offset;
    truncated_text_end_position_ = found_position;
    // An acceptable break position was not found. If the placement requirement
    // was already met prior to this box, then the ellipsis doesn't  require a
    // character from this box to appear prior to its position, so simply place
    // the ellipsis at the left edge of the box and fully truncate the text.
  } else if (is_placement_requirement_met) {
    *placed_offset = left();
    truncated_text_end_position_ = text_start_position_;
    // The placement requirement has not already been met. Given that an
    // acceptable break position was not found within the text, the ellipsis can
    // only be placed at the right edge of the box.
  } else {
    *placed_offset = GetMarginBoxRightEdgeOffsetFromContainingBlock();
  }
}

bool TextBox::WhiteSpaceStyleAllowsCollapsing() {
  return computed_style()->white_space() != cssom::KeywordValue::GetPre();
}

bool TextBox::WhiteSpaceStyleAllowsWrapping() {
  return computed_style()->white_space() != cssom::KeywordValue::GetPre() &&
         computed_style()->white_space() != cssom::KeywordValue::GetNoWrap();
}

void TextBox::UpdateTextHasLeadingWhiteSpace() {
  text_has_leading_white_space_ = text_start_position_ != text_end_position_ &&
                                  paragraph_->IsSpace(text_start_position_) &&
                                  WhiteSpaceStyleAllowsCollapsing();
}

void TextBox::UpdateTextHasTrailingWhiteSpace() {
  text_has_trailing_white_space_ =
      !has_trailing_line_break_ && text_start_position_ != text_end_position_ &&
      paragraph_->IsSpace(text_end_position_ - 1) &&
      WhiteSpaceStyleAllowsCollapsing();
}

scoped_refptr<Box> TextBox::SplitAtPosition(int32 split_start_position) {
  int32 split_end_position = text_end_position_;
  DCHECK_LT(split_start_position, split_end_position);

  text_end_position_ = split_start_position;

  // The size results are no longer valid for this box now that it has been
  // split in two.
  update_size_results_valid_ = false;

  scoped_refptr<Box> box_after_split(new TextBox(
      computed_style_state(), paragraph_, split_start_position,
      split_end_position, has_trailing_line_break_, used_style_provider()));

  // TODO(***REMOVED***): Set the text width of the box after split to
  //               |text_width_ - pre_split_width| to save a call
  //               to Skia/HarfBuzz.

  // Pass the line break trigger on to the sibling that retains the trailing
  // portion of the text and reset the value for this text box.
  has_trailing_line_break_ = false;

  // Update the paragraph end position white space now that this text box has
  // a new end position. The start position white space does not need to be
  // updated as it has not changed.
  UpdateTextHasTrailingWhiteSpace();

  return box_after_split;
}

float TextBox::GetLeadingWhiteSpaceWidth() const {
  return HasLeadingWhiteSpace()
             ? RoundToFixedPointPrecision(used_font_->GetSpaceWidth())
             : 0;
}

float TextBox::GetTrailingWhiteSpaceWidth() const {
  return HasTrailingWhiteSpace() && HasNonCollapsibleText()
             ? RoundToFixedPointPrecision(used_font_->GetSpaceWidth())
             : 0;
}

int32 TextBox::GetNonCollapsedTextStartPosition() const {
  return should_collapse_leading_white_space_
             ? GetNonCollapsibleTextStartPosition()
             : text_start_position_;
}

int32 TextBox::GetNonCollapsedTextEndPosition() const {
  return should_collapse_trailing_white_space_
             ? GetNonCollapsibleTextEndPosition()
             : text_end_position_;
}

int32 TextBox::GetNonCollapsibleTextStartPosition() const {
  return text_has_leading_white_space_ ? text_start_position_ + 1
                                       : text_start_position_;
}

int32 TextBox::GetNonCollapsibleTextEndPosition() const {
  return text_has_trailing_white_space_ ? text_end_position_ - 1
                                        : text_end_position_;
}

bool TextBox::HasNonCollapsibleText() const {
  return GetNonCollapsibleTextStartPosition() <
         GetNonCollapsibleTextEndPosition();
}

std::string TextBox::GetNonCollapsibleText() const {
  return paragraph_->RetrieveUtf8SubString(GetNonCollapsibleTextStartPosition(),
                                           GetNonCollapsibleTextEndPosition(),
                                           Paragraph::kVisualTextOrder);
}

int32 TextBox::GetVisibleTextEndPosition() const {
  return std::min(GetNonCollapsibleTextEndPosition(),
                  truncated_text_end_position_);
}

bool TextBox::HasVisibleText() const {
  return GetNonCollapsibleTextStartPosition() < GetVisibleTextEndPosition();
}

std::string TextBox::GetVisibleText() const {
  return paragraph_->RetrieveUtf8SubString(GetNonCollapsibleTextStartPosition(),
                                           GetVisibleTextEndPosition(),
                                           Paragraph::kVisualTextOrder);
}

}  // namespace layout
}  // namespace cobalt
