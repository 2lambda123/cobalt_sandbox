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

#include "cobalt/dom/html_span_element.h"

namespace cobalt {
namespace dom {

// static
const char* HTMLSpanElement::kTagName = "span";

HTMLSpanElement::HTMLSpanElement(Document* document,
                                 HTMLElementContext* html_element_context)
    : HTMLElement(document, html_element_context) {}

std::string HTMLSpanElement::tag_name() const {
  return kTagName;
}

HTMLSpanElement::~HTMLSpanElement() {}

}  // namespace dom
}  // namespace cobalt
