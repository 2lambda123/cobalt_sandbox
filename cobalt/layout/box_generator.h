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
 * limitations under the License.`
 */

#ifndef LAYOUT_BOX_GENERATOR_H_
#define LAYOUT_BOX_GENERATOR_H_

#include "base/memory/scoped_vector.h"
#include "cobalt/cssom/css_style_declaration_data.h"
#include "cobalt/cssom/string_value.h"
#include "cobalt/dom/html_element.h"
#include "cobalt/dom/node.h"
#include "cobalt/layout/box.h"
#include "third_party/icu/public/common/unicode/brkiter.h"

namespace cobalt {

namespace dom {
class HTMLVideoElement;
}  // namespace dom

namespace layout {

class Box;
class Paragraph;
class UsedStyleProvider;

// In the visual formatting model, each element in the document tree generates
// zero or more boxes.
//   http://www.w3.org/TR/CSS21/visuren.html#box-gen
//
// A box generator recursively visits an HTML subtree that starts with a given
// element, creates a matching forest of boxes, and returns zero or more root
// boxes.
//
// As a side-effect, computed styles of visited HTML elements are updated.
class BoxGenerator : public dom::NodeVisitor {
 public:
  BoxGenerator(const scoped_refptr<const cssom::CSSStyleDeclarationData>&
                   parent_computed_style,
               UsedStyleProvider* used_style_provider,
               icu::BreakIterator* line_break_iterator,
               scoped_refptr<Paragraph>* paragraph);
  ~BoxGenerator();

  void Visit(dom::CDATASection* cdata_section) OVERRIDE;
  void Visit(dom::Comment* comment) OVERRIDE;
  void Visit(dom::Document* document) OVERRIDE;
  void Visit(dom::DocumentType* document_type) OVERRIDE;
  void Visit(dom::Element* element) OVERRIDE;
  void Visit(dom::Text* text) OVERRIDE;

  const Boxes& boxes() const { return boxes_; }

 private:
  void VisitVideoElement(dom::HTMLVideoElement* video_element);
  void VisitBrElement(dom::HTMLBRElement* br_element);
  void VisitNonReplacedElement(dom::HTMLElement* html_element);

  void AppendChildBoxToLine(const scoped_refptr<Box>& child_box);
  void AppendPseudoElementToLine(dom::HTMLElement* html_element,
                                 dom::PseudoElementType pseudo_element_type);

  const scoped_refptr<const cssom::CSSStyleDeclarationData>
      parent_computed_style_;
  UsedStyleProvider* const used_style_provider_;
  icu::BreakIterator* const line_break_iterator_;
  scoped_refptr<Paragraph>* paragraph_;
  scoped_refptr<dom::HTMLElement> generating_html_element_;

  // The result of a box generator is zero or more root boxes.
  Boxes boxes_;

  DISALLOW_COPY_AND_ASSIGN(BoxGenerator);
};

}  // namespace layout
}  // namespace cobalt

#endif  // LAYOUT_BOX_GENERATOR_H_
