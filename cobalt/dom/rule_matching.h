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

#ifndef COBALT_DOM_RULE_MATCHING_H_
#define COBALT_DOM_RULE_MATCHING_H_

#include "base/memory/ref_counted.h"
#include "cobalt/cssom/cascade_priority.h"
#include "cobalt/cssom/css_style_declaration_data.h"
#include "cobalt/cssom/css_style_rule.h"
#include "cobalt/cssom/css_style_sheet.h"
#include "cobalt/cssom/selector_tree.h"
#include "cobalt/cssom/style_sheet_list.h"

namespace cobalt {
namespace dom {

class Element;
class HTMLElement;

// Returns whether a rule matches an element.
bool MatchRuleAndElement(cssom::CSSStyleRule* rule, Element* element);

// Updates the matching rules on an element and its children, using the given
// selector tree.
void UpdateMatchingRulesUsingSelectorTree(
    HTMLElement* dom_root, const cssom::SelectorTree* selector_tree);

}  // namespace dom
}  // namespace cobalt

#endif  // COBALT_DOM_RULE_MATCHING_H_
