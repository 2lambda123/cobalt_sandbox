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

#include "cobalt/webdriver/element_driver.h"

#include "cobalt/cssom/property_value.h"
#include "cobalt/cssom/string_value.h"
#include "cobalt/dom/document.h"
#include "cobalt/dom/html_body_element.h"
#include "cobalt/dom/html_element.h"
#include "cobalt/webdriver/util/call_on_message_loop.h"

namespace cobalt {
namespace webdriver {
namespace {
std::string GetTagName(dom::Element* element) {
  DCHECK(element);
  return element->tag_name();
}

bool IsHidden(dom::Element* element) {
  base::optional<std::string> display_property =
      element->GetAttribute("display");
  return display_property && display_property.value() == "hidden";
}

bool DisplayStyleIsNone(dom::Element* element) {
  scoped_refptr<dom::HTMLElement> html_element = element->AsHTMLElement();
  DCHECK(html_element);
  DCHECK(html_element->computed_style());
  scoped_refptr<cssom::StringValue> none(new cssom::StringValue("none"));
  scoped_refptr<cssom::PropertyValue> display =
      html_element->computed_style()->display();
  return display && display->Equals(*none.get());
}

bool IsTransparentDocumentRoot(dom::Element* element) {
  scoped_refptr<dom::Document> element_as_document = element->AsDocument();
  scoped_refptr<dom::HTMLElement> html_element = element->AsHTMLElement();
  if (element_as_document && html_element && html_element->computed_style()) {
    scoped_refptr<cssom::StringValue> transparent_property(
        new cssom::StringValue("transparent"));
    return html_element->computed_style() &&
           html_element->computed_style()->background_color() &&
           html_element->computed_style()->background_color()->Equals(
               *transparent_property.get());
  }
  return false;
}

bool HasTransparentBody(dom::Element* element) {
  scoped_refptr<dom::Document> element_as_document = element->AsDocument();
  DCHECK(element_as_document);
  scoped_refptr<cssom::StringValue> transparent_property(
      new cssom::StringValue("transparent"));
  return element_as_document->body() &&
      element_as_document->body()->computed_style() &&
      element_as_document->body()->computed_style()->background_color() &&
      element_as_document->body()
          ->computed_style()
          ->background_color()
          ->Equals(*transparent_property.get());
}

// https://w3c.github.io/webdriver/webdriver-spec.html#element-displayedness
bool IsDisplayed(dom::Element* element) {
  DCHECK(element);
  // 1. If the attribute hidden is set, return false.
  if (IsHidden(element)) {
    return false;
  }
  // 2. If the computed value of the display style property is "none", return
  //    false.
  if (DisplayStyleIsNone(element)) {
    return false;
  }
  // 4. If element is the document's root element...
  // 4.1. If the computed value of the background-color property is
  //      "transparent".
  if (IsTransparentDocumentRoot(element)) {
    // 4.1.1. If the body is also transparent.
    return HasTransparentBody(element);
  }
  // 8. If element is a [DOM] text node, return true.
  if (element->IsText()) {
    return true;
  }
  // TODO(***REMOVED***): Check for visible children and whether this element
  // is hidden by a parent's overflow behaviour once getBoundingRect and
  // getClientRects are implemented.
  return true;
}
}  // namespace

ElementDriver::ElementDriver(
    const protocol::ElementId& element_id,
    const base::WeakPtr<dom::Element>& element,
    const scoped_refptr<base::MessageLoopProxy>& message_loop)
    : element_id_(element_id),
      element_(element),
      element_message_loop_(message_loop) {}

util::CommandResult<std::string> ElementDriver::GetTagName() {
  return util::CallWeakOnMessageLoopAndReturnResult(
      element_message_loop_,
      base::Bind(&ElementDriver::GetWeakElement, base::Unretained(this)),
      base::Bind(&::cobalt::webdriver::GetTagName),
      protocol::Response::kStaleElementReference);
}

dom::Element* ElementDriver::GetWeakElement() {
  DCHECK_EQ(base::MessageLoopProxy::current(), element_message_loop_);
  return element_.get();
}

util::CommandResult<bool> ElementDriver::IsDisplayed() {
  return util::CallWeakOnMessageLoopAndReturnResult(
      element_message_loop_,
      base::Bind(&ElementDriver::GetWeakElement, base::Unretained(this)),
      base::Bind(&::cobalt::webdriver::IsDisplayed),
      protocol::Response::kStaleElementReference);
}

}  // namespace webdriver
}  // namespace cobalt
