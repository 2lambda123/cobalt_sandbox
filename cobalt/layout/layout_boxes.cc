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

#include "cobalt/layout/layout_boxes.h"

#include "cobalt/layout/container_box.h"

namespace cobalt {
namespace layout {

LayoutBoxes::LayoutBoxes() {}

LayoutBoxes::~LayoutBoxes() {}

LayoutBoxes::Type LayoutBoxes::type() const { return kLayoutLayoutBoxes; }

// Algorithm for GetClientRects:
//   https://www.w3.org/TR/2013/WD-cssom-view-20131217/#dom-element-getclientrects
scoped_refptr<dom::DOMRectList> LayoutBoxes::GetClientRects() const {
  // 1. If the element on which it was invoked does not have an associated
  // layout box return an empty DOMRectList object and stop this algorithm.

  // 2. If the element has an associated SVG layout box return a DOMRectList
  // object containing a single DOMRect object that describes the bounding box
  // of the element as defined by the SVG specification, applying the transforms
  // that apply to the element and its ancestors.

  // 3. Return a DOMRectList object containing a list of DOMRect objects in
  // content order describing the bounding border boxes (including those with a
  // height or width of zero) with the following constraints:
  //  . Apply the transforms that apply to the element and its ancestors.
  //  . If the element on which the method was invoked has a computed value for
  //    the 'display' property of 'table' or 'inline-table' include both the
  //    table box and the caption box, if any, but not the anonymous container
  //    box.
  //  . Replace each anonymous block box with its child box(es) and repeat this
  //    until no anonymous block boxes are left in the final list.

  scoped_refptr<dom::DOMRectList> dom_rect_list(new dom::DOMRectList());
  for (Boxes::const_iterator box_iterator = boxes_.begin();
       box_iterator != boxes_.end(); ++box_iterator) {
    Box* box = *box_iterator;
    scoped_refptr<dom::DOMRect> dom_rect(new dom::DOMRect());

    // TODO(***REMOVED***): Take transforms into account and recurse into anonymous
    // block boxes. ***REMOVED*** currently doesn't rely on GetClientRects() to do
    // that. Tracked in b/25983085.

    dom_rect->set_x(box->GetBorderBoxLeftEdge());
    dom_rect->set_y(box->GetBorderBoxTopEdge());
    math::SizeF box_size = box->GetBorderBoxSize();
    dom_rect->set_width(box_size.width());
    dom_rect->set_height(box_size.height());
    dom_rect_list->AppendDOMRect(dom_rect);
  }

  return dom_rect_list;
}

bool LayoutBoxes::IsInlineLevel() const {
  DCHECK(!boxes_.empty());
  return boxes_.front()->GetLevel() == Box::kInlineLevel;
}

float LayoutBoxes::GetBorderEdgeLeft() const {
  return GetBoundingBorderRectangle().x();
}

float LayoutBoxes::GetBorderEdgeTop() const {
  return GetBoundingBorderRectangle().y();
}

float LayoutBoxes::GetBorderEdgeWidth() const {
  return GetBoundingBorderRectangle().width();
}

float LayoutBoxes::GetBorderEdgeHeight() const {
  return GetBoundingBorderRectangle().height();
}

float LayoutBoxes::GetBorderLeftWidth() const {
  DCHECK(!boxes_.empty());
  return boxes_.front()->border_left_width();
}

float LayoutBoxes::GetBorderTopWidth() const {
  DCHECK(!boxes_.empty());
  return boxes_.front()->border_top_width();
}

float LayoutBoxes::GetMarginEdgeWidth() const {
  DCHECK(!boxes_.empty());
  return boxes_.front()->GetMarginBoxWidth();
}

float LayoutBoxes::GetMarginEdgeHeight() const {
  DCHECK(!boxes_.empty());
  return boxes_.front()->GetMarginBoxHeight();
}

float LayoutBoxes::GetPaddingEdgeLeft() const {
  DCHECK(!boxes_.empty());
  return boxes_.front()->GetPaddingBoxLeftEdge();
}

float LayoutBoxes::GetPaddingEdgeTop() const {
  DCHECK(!boxes_.empty());
  return boxes_.front()->GetPaddingBoxTopEdge();
}

float LayoutBoxes::GetPaddingEdgeWidth() const {
  DCHECK(!boxes_.empty());
  return boxes_.front()->GetPaddingBoxWidth();
}

float LayoutBoxes::GetPaddingEdgeHeight() const {
  DCHECK(!boxes_.empty());
  return boxes_.front()->GetPaddingBoxHeight();
}

math::RectF LayoutBoxes::GetBoundingBorderRectangle() const {
  // In the CSSOM View extensions to the HTMLElement interface, at
  // https://www.w3.org/TR/2013/WD-cssom-view-20131217/#extensions-to-the-htmlelement-interface,
  // the standard mentions the 'first CSS layout box associated with the
  // element' and links to a definition 'The term CSS layout box refers to the
  // same term in CSS', which is followed by a note 'ISSUE 2' that mentions 'The
  // terms CSS layout box and SVG layout box are not currently defined by CSS or
  // SVG', at https://www.w3.org/TR/2013/WD-cssom-view-20131217/#css-layout-box.
  // This function calculates the bounding box of the border boxes of the layout
  // boxes, mirroring behavior of most other browsers for the 'first CSS layout
  // box associated with the element'.
  if (boxes_.empty()) return math::RectF();
  Boxes::const_iterator box_iterator = boxes_.begin();
  const Box* box = *box_iterator;
  math::RectF bounding_rectangle(box->GetBorderBox());
  ++box_iterator;
  for (; box_iterator != boxes_.end(); ++box_iterator) {
    box = *box_iterator;
    bounding_rectangle.Union(box->GetBorderBox());
  }
  return bounding_rectangle;
}

}  // namespace layout
}  // namespace cobalt
