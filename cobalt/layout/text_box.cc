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

#include "cobalt/layout/used_style.h"
#include "cobalt/math/transform_2d.h"
#include "cobalt/render_tree/text_node.h"

namespace cobalt {
namespace layout {

TextBox::TextBox(
    const scoped_refptr<const cssom::CSSStyleDeclarationData>& computed_style,
    const cssom::TransitionSet* transitions, const std::string& text,
    bool has_leading_white_space, bool has_trailing_white_space,
    const scoped_refptr<render_tree::Font>& used_font)
    : Box(computed_style, transitions),
      text_(text),
      has_leading_white_space_(has_leading_white_space),
      has_trailing_white_space_(has_trailing_white_space),
      used_font_(used_font) {}

Box::Level TextBox::GetLevel() const { return kInlineLevel; }

void TextBox::Layout(const LayoutParams& /*layout_params*/) {
  // TODO(***REMOVED***): If this method is called on a box after the split, no layout
  //               recalculation is necessary.

  render_tree::FontMetrics font_metrics = used_font_->GetFontMetrics();

  // Since Skia returns the bounding rectangle of a text, the width of any white
  // space cannot be measured on its own.
  space_width_ = used_font_->GetBounds("_ _").width() -
                 used_font_->GetBounds("__").width();

  // TODO(***REMOVED***): Skia knows how to measure the bounding box of the text,
  //               while we need a layout box. For example, the bounding box
  //               for the letter "i" is smaller than the layout box
  //               (the layout box includes a thin white space around
  //               the letter). Consider using Pango, Harfbuzz, or FreeType
  //               instead.
  math::RectF text_bounds = used_font_->GetBounds(text_);
  used_frame().set_width(GetLeadingWhiteSpaceWidth() + text_bounds.width() +
                         GetTrailingWhiteSpaceWidth());
  text_x_ = -text_bounds.x();

  // Below is calculated based on
  // http://www.w3.org/TR/CSS21/visudet.html#leading.

  UsedLineHeightProvider used_line_height_provider(font_metrics);
  computed_style()->line_height()->Accept(&used_line_height_provider);

  // Determine the leading L, where L = "line-height" - AD,
  // AD = A (ascent) + D (descent).
  float leading = used_line_height_provider.used_line_height() -
                  (font_metrics.ascent + font_metrics.descent);

  // The height of the inline box encloses all glyphs and their half-leading
  // on each side and is thus exactly "line-height".
  used_frame().set_height(used_line_height_provider.used_line_height());

  // Half the leading is added above ascent (A) and the other half below
  // descent (D), giving the glyph and its leading (L) a total height above
  // the baseline of A' = A + L/2 and a total depth of D' = D + L/2.
  height_above_baseline_ = font_metrics.ascent + leading / 2;
}

scoped_ptr<Box> TextBox::TrySplitAt(float /*available_width*/) {
  // TODO(***REMOVED***): Split the text box at soft wrap opportunity.
  //               http://www.w3.org/TR/css-text-3/#soft-wrap-opportunity
  // TODO(***REMOVED***): Implement "white-space: nowrap".
  //               http://www.w3.org/TR/css3-text/#white-space
  NOTIMPLEMENTED();

  // TODO(***REMOVED***): Update the text bounds in the both parts of the original box
  //               after the successful split.

  return scoped_ptr<Box>();
}

bool TextBox::IsCollapsed() const {
  return !has_leading_white_space_ && !has_trailing_white_space_ &&
         text_.empty();
}

bool TextBox::HasLeadingWhiteSpace() const { return has_leading_white_space_; }

bool TextBox::HasTrailingWhiteSpace() const {
  return has_trailing_white_space_;
}

void TextBox::CollapseLeadingWhiteSpace() {
  if (has_leading_white_space_) {
    used_frame().set_width(used_frame().width() - GetLeadingWhiteSpaceWidth());
    has_leading_white_space_ = false;

    if (has_trailing_white_space_ && text_.empty()) {
      CollapseTrailingWhiteSpace();
    }
  }
}

void TextBox::CollapseTrailingWhiteSpace() {
  if (has_trailing_white_space_) {
    used_frame().set_width(used_frame().width() - GetTrailingWhiteSpaceWidth());
    has_trailing_white_space_ = false;

    if (has_leading_white_space_ && text_.empty()) {
      CollapseLeadingWhiteSpace();
    }
  }
}

bool TextBox::JustifiesLineExistence() const { return !text_.empty(); }

bool TextBox::AffectsBaselineInBlockFormattingContext() const {
  NOTREACHED() << "Should only be called in a block formatting context.";
  return true;
}

float TextBox::GetHeightAboveBaseline() const { return height_above_baseline_; }

void TextBox::AddContentToRenderTree(
    render_tree::CompositionNode::Builder* composition_node_builder,
    render_tree::animations::NodeAnimationsMap::Builder*
    /* node_animations_map_builder */) const {
  render_tree::ColorRGBA used_color = GetUsedColor(computed_style()->color());

  // The render tree API considers text coordinates to be a position of
  // a baseline, offset the text node accordingly.
  composition_node_builder->AddChild(
      new render_tree::TextNode(text_, used_font_, used_color),
      math::TranslateMatrix(GetLeadingWhiteSpaceWidth() + text_x_,
                            height_above_baseline_));
}

bool TextBox::IsTransformable() const { return false; }

void TextBox::DumpClassName(std::ostream* stream) const {
  *stream << "TextBox ";
}

void TextBox::DumpProperties(std::ostream* stream) const {
  Box::DumpProperties(stream);

  *stream << std::boolalpha
          << "has_leading_white_space=" << has_leading_white_space_ << " "
          << "has_trailing_white_space=" << has_trailing_white_space_ << " "
          << std::noboolalpha << "text_x=" << text_x_ << " ";
}

void TextBox::DumpChildrenWithIndent(std::ostream* stream, int indent) const {
  Box::DumpChildrenWithIndent(stream, indent);

  DumpIndent(stream, indent);
  *stream << "\"" << text_ << "\"\n";
}

float TextBox::GetLeadingWhiteSpaceWidth() const {
  return has_leading_white_space_ ? space_width_ : 0;
}

float TextBox::GetTrailingWhiteSpaceWidth() const {
  return has_trailing_white_space_ && !text_.empty() ? space_width_ : 0;
}

}  // namespace layout
}  // namespace cobalt
