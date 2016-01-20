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

#include "cobalt/dom/rule_matching.h"

#include <algorithm>

#include "base/bind.h"
#include "base/callback.h"
#include "base/debug/trace_event.h"
#include "base/string_util.h"
#include "cobalt/base/unused.h"
#include "cobalt/cssom/after_pseudo_element.h"
#include "cobalt/cssom/before_pseudo_element.h"
#include "cobalt/cssom/cascade_priority.h"
#include "cobalt/cssom/child_combinator.h"
#include "cobalt/cssom/class_selector.h"
#include "cobalt/cssom/combinator.h"
#include "cobalt/cssom/combinator_visitor.h"
#include "cobalt/cssom/complex_selector.h"
#include "cobalt/cssom/compound_selector.h"
#include "cobalt/cssom/css_style_rule.h"
#include "cobalt/cssom/css_style_sheet.h"
#include "cobalt/cssom/descendant_combinator.h"
#include "cobalt/cssom/empty_pseudo_class.h"
#include "cobalt/cssom/following_sibling_combinator.h"
#include "cobalt/cssom/id_selector.h"
#include "cobalt/cssom/next_sibling_combinator.h"
#include "cobalt/cssom/not_pseudo_class.h"
#include "cobalt/cssom/property_value.h"
#include "cobalt/cssom/pseudo_element.h"
#include "cobalt/cssom/selector_visitor.h"
#include "cobalt/cssom/simple_selector.h"
#include "cobalt/cssom/type_selector.h"
#include "cobalt/dom/dom_token_list.h"
#include "cobalt/dom/element.h"
#include "cobalt/dom/html_element.h"
#include "cobalt/dom/html_html_element.h"
#include "cobalt/dom/node_descendants_iterator.h"
#include "cobalt/dom/node_list.h"
#include "cobalt/dom/pseudo_element.h"
#include "cobalt/math/safe_integer_conversions.h"

namespace cobalt {
namespace dom {
namespace {

using cssom::SelectorTree;
const size_t kRuleMatchingNodeSetSize = 60;

//////////////////////////////////////////////////////////////////////////
// Helper functions
//////////////////////////////////////////////////////////////////////////

// Matches an element against a selector. If the element doesn't match, returns
// NULL, otherwise returns the result of advancing the pointer to the element
// according to the combinators.
Element* MatchSelectorAndElement(cssom::Selector* selector, Element* element,
                                 bool matching_combinators);

//////////////////////////////////////////////////////////////////////////
// Combinator matcher
//////////////////////////////////////////////////////////////////////////

class CombinatorMatcher : public cssom::CombinatorVisitor {
 public:
  explicit CombinatorMatcher(Element* element) : element_(element) {
    DCHECK(element);
  }

  // Child combinator describes a childhood relationship between two elements.
  //   https://www.w3.org/TR/selectors4/#child-combinators
  void VisitChildCombinator(cssom::ChildCombinator* child_combinator) OVERRIDE {
    element_ = MatchSelectorAndElement(child_combinator->left_selector(),
                                       element_->parent_element(), true);
  }

  // Next-sibling combinator describes that the elements represented by the two
  // compound selectors share the same parent in the document tree and the
  // element represented by the first compound selector immediately precedes the
  // element represented by the second one.
  //   https://www.w3.org/TR/selectors4/#adjacent-sibling-combinators
  void VisitNextSiblingCombinator(
      cssom::NextSiblingCombinator* next_sibling_combinator) OVERRIDE {
    element_ =
        MatchSelectorAndElement(next_sibling_combinator->left_selector(),
                                element_->previous_element_sibling(), true);
  }

  // Descendant combinator describes an element that is the descendant of
  // another element in the document tree.
  //   https://www.w3.org/TR/selectors4/#descendant-combinators
  void VisitDescendantCombinator(
      cssom::DescendantCombinator* descendant_combinator) OVERRIDE {
    do {
      element_ = element_->parent_element();
      Element* element = MatchSelectorAndElement(
          descendant_combinator->left_selector(), element_, true);
      if (element) {
        element_ = element;
        return;
      }
    } while (element_);
  }

  // Following-sibling combinator describes that the elements represented by the
  // two compound selectors share the same parent in the document tree and the
  // element represented by the first compound selector precedes (not
  // necessarily immediately) the element represented by the second one.
  //   https://www.w3.org/TR/selectors4/#general-sibling-combinators
  void VisitFollowingSiblingCombinator(
      cssom::FollowingSiblingCombinator* following_sibling_combinator)
      OVERRIDE {
    do {
      element_ = element_->previous_element_sibling();
      Element* element = MatchSelectorAndElement(
          following_sibling_combinator->left_selector(), element_, true);
      if (element) {
        element_ = element;
        return;
      }
    } while (element_);
  }

  Element* element() { return element_; }

 private:
  Element* element_;
  DISALLOW_COPY_AND_ASSIGN(CombinatorMatcher);
};

//////////////////////////////////////////////////////////////////////////
// Selector matcher
//////////////////////////////////////////////////////////////////////////

class SelectorMatcher : public cssom::SelectorVisitor {
 public:
  explicit SelectorMatcher(Element* element)
      : element_(element), matching_combinators_(false) {
    DCHECK(element);
  }
  SelectorMatcher(Element* element, bool matching_combinators)
      : element_(element), matching_combinators_(matching_combinators) {
    DCHECK(element);
  }

  // A type selector represents an instance of the element type in the document
  // tree.
  //   https://www.w3.org/TR/selectors4/#type-selector
  void VisitTypeSelector(cssom::TypeSelector* type_selector) OVERRIDE {
    if (!LowerCaseEqualsASCII(type_selector->element_name().c_str(),
                              element_->tag_name().c_str())) {
      element_ = NULL;
    }
  }

  // The class selector represents an element belonging to the class identified
  // by the identifier.
  //   https://www.w3.org/TR/selectors4/#class-selector
  void VisitClassSelector(cssom::ClassSelector* class_selector) OVERRIDE {
    if (!element_->class_list()->ContainsValid(class_selector->class_name())) {
      element_ = NULL;
    }
  }

  // An ID selector represents an element instance that has an identifier that
  // matches the identifier in the ID selector.
  //   https://www.w3.org/TR/selectors4/#id-selector
  void VisitIdSelector(cssom::IdSelector* id_selector) OVERRIDE {
    if (id_selector->id() != element_->id()) {
      element_ = NULL;
    }
  }

  // The :active pseudo-class applies while an element is being activated by the
  // user. For example, between the times the user presses the mouse button and
  // releases it. On systems with more than one mouse button, :active applies
  // only to the primary or primary activation button (typically the "left"
  // mouse button), and any aliases thereof.
  //   https://www.w3.org/TR/selectors4/#active-pseudo
  void VisitActivePseudoClass(cssom::ActivePseudoClass*) OVERRIDE {
    NOTIMPLEMENTED();
    element_ = NULL;
  }

  // The :empty pseudo-class represents an element that has no content children.
  //   https://www.w3.org/TR/selectors4/#empty-pseudo
  void VisitEmptyPseudoClass(cssom::EmptyPseudoClass*) OVERRIDE {
    if (!element_->IsEmpty()) {
      element_ = NULL;
    }
  }

  // The :focus pseudo-class applies while an element has the focus (accepts
  // keyboard or mouse events, or other forms of input).
  //   https://www.w3.org/TR/selectors4/#focus-pseudo
  void VisitFocusPseudoClass(cssom::FocusPseudoClass*) OVERRIDE {
    if (!element_->HasFocus()) {
      element_ = NULL;
    }
  }

  // The :hover pseudo-class applies while the user designates an element with a
  // pointing device, but does not necessarily activate it. For example, a
  // visual user agent could apply this pseudo-class when the cursor (mouse
  // pointer) hovers over a box generated by the element. Interactive user
  // agents that cannot detect hovering due to hardware limitations (e.g., a pen
  // device that does not detect hovering) are still conforming.
  //   https://www.w3.org/TR/selectors4/#hover-pseudo
  void VisitHoverPseudoClass(cssom::HoverPseudoClass*) OVERRIDE {
    NOTIMPLEMENTED();
    element_ = NULL;
  }

  // The negation pseudo-class, :not(), is a functional pseudo-class taking a
  // selector list as an argument. It represents an element that is not
  // represented by its argument.
  //   https://www.w3.org/TR/selectors4/#negation-pseudo
  void VisitNotPseudoClass(cssom::NotPseudoClass* not_pseudo_class) OVERRIDE {
    if (MatchSelectorAndElement(not_pseudo_class->selector(), element_, true)) {
      element_ = NULL;
    }
  }

  // Pseudo elements doesn't affect whether the selector matches or not.

  // The :after pseudo-element represents a generated element.
  //   https://www.w3.org/TR/CSS21/generate.html#before-after-content
  void VisitAfterPseudoElement(cssom::AfterPseudoElement*) OVERRIDE {}

  // The :before pseudo-element represents a generated element.
  //   https://www.w3.org/TR/CSS21/generate.html#before-after-content
  void VisitBeforePseudoElement(cssom::BeforePseudoElement*) OVERRIDE {}

  // A compound selector is a chain of simple selectors that are not separated
  // by a combinator.
  //   https://www.w3.org/TR/selectors4/#compound
  void VisitCompoundSelector(
      cssom::CompoundSelector* compound_selector) OVERRIDE {
    DCHECK_GT(compound_selector->simple_selectors().size(), 0U);
    // Iterate through all the simple selectors. If any of the simple selectors
    // doesn't match, the compound selector doesn't match.
    for (cssom::CompoundSelector::SimpleSelectors::const_iterator
             selector_iterator = compound_selector->simple_selectors().begin();
         selector_iterator != compound_selector->simple_selectors().end();
         ++selector_iterator) {
      element_ = MatchSelectorAndElement(*selector_iterator, element_, false);
      if (!element_) {
        return;
      }
    }

    // Check combinator.
    if (matching_combinators_ && compound_selector->left_combinator()) {
      CombinatorMatcher combinator_matcher(element_);
      compound_selector->left_combinator()->Accept(&combinator_matcher);
      element_ = combinator_matcher.element();
    }
  }

  // A complex selector is a chain of one or more compound selectors separated
  // by combinators.
  //   https://www.w3.org/TR/selectors4/#complex
  void VisitComplexSelector(cssom::ComplexSelector* complex_selector) OVERRIDE {
    element_ = MatchSelectorAndElement(complex_selector->last_selector(),
                                       element_, true);
  }

  Element* element() const { return element_; }

 private:
  Element* element_;
  bool matching_combinators_;
  DISALLOW_COPY_AND_ASSIGN(SelectorMatcher);
};

//////////////////////////////////////////////////////////////////////////
// Helper functions
//////////////////////////////////////////////////////////////////////////

Element* MatchSelectorAndElement(cssom::Selector* selector, Element* element,
                                 bool matching_combinators) {
  DCHECK(selector);
  if (!element) {
    return NULL;
  }
  SelectorMatcher selector_matcher(element, matching_combinators);
  selector->Accept(&selector_matcher);
  return selector_matcher.element();
}

void AddRulesOnNodeToElement(HTMLElement* element,
                             const SelectorTree::Node* node) {
  cssom::PseudoElement* pseudo_element =
      node->compound_selector()->pseudo_element();

  // Where to add matching rules.
  cssom::RulesWithCascadePriority* target_matching_rules;

  if (!pseudo_element) {
    target_matching_rules = element->matching_rules();
  } else {
    PseudoElementType pseudo_element_type = kNotPseudoElementType;

    if (pseudo_element->AsAfterPseudoElement()) {
      pseudo_element_type = kAfterPseudoElementType;
    } else if (pseudo_element->AsBeforePseudoElement()) {
      pseudo_element_type = kBeforePseudoElementType;
    } else {
      NOTREACHED();
    }

    // Make sure the pseudo element exists under element.
    if (!element->pseudo_element(pseudo_element_type)) {
      element->set_pseudo_element(
          pseudo_element_type,
          make_scoped_ptr(new dom::PseudoElement(element)));
    }

    target_matching_rules =
        element->pseudo_element(pseudo_element_type)->matching_rules();
  }

  element->rule_matching_state()->matching_nodes.insert(node);

  for (SelectorTree::Rules::const_iterator rule_iterator =
           node->rules().begin();
       rule_iterator != node->rules().end(); ++rule_iterator) {
    base::WeakPtr<cssom::CSSStyleRule> rule = *rule_iterator;
    if (!rule) {
      continue;
    }
    DCHECK(rule->parent_style_sheet());
    cssom::CascadePriority precedence(
        pseudo_element ? cssom::kNormalOverride
                       : rule->parent_style_sheet()->origin(),
        node->cumulative_specificity(),
        cssom::Appearance(rule->parent_style_sheet()->index(), rule->index()));
    target_matching_rules->push_back(std::make_pair(rule.get(), precedence));
  }
}

void GatherCandidateNodesFromMap(
    cssom::SimpleSelectorType simple_selector_type,
    cssom::CombinatorType combinator_type,
    const SelectorTree::SelectorTextToNodesMap& map, base::Token key,
    SelectorTree::NodeSet<kRuleMatchingNodeSetSize>* candidate_nodes) {
  // TODO(***REMOVED***): The hit rate here is only ~20%, use a Token specific map
  // to pre-check if the key is in the map.
  SelectorTree::SelectorTextToNodesMap::const_iterator it = map.find(key);
  if (it != map.end()) {
    const SelectorTree::SimpleSelectorNodes& nodes = it->second;
    for (SelectorTree::SimpleSelectorNodes::const_iterator nodes_iterator =
             nodes.begin();
         nodes_iterator != nodes.end(); ++nodes_iterator) {
      if (nodes_iterator->simple_selector_type == simple_selector_type &&
          nodes_iterator->combinator_type == combinator_type) {
        candidate_nodes->insert(nodes_iterator->node);
      }
    }
  }
}

void GatherCandidateNodesFromSet(
    cssom::PseudoClassType pseudo_class_type,
    cssom::CombinatorType combinator_type,
    const SelectorTree::PseudoClassNodes& pseudo_class_nodes,
    SelectorTree::NodeSet<kRuleMatchingNodeSetSize>* candidate_nodes) {
  for (SelectorTree::PseudoClassNodes::const_iterator nodes_iterator =
           pseudo_class_nodes.begin();
       nodes_iterator != pseudo_class_nodes.end(); ++nodes_iterator) {
    if (nodes_iterator->pseudo_class_type == pseudo_class_type &&
        nodes_iterator->combinator_type == combinator_type) {
      candidate_nodes->insert(nodes_iterator->node);
    }
  }
}

template <class NodeSet>
void ForEachChildOnNodes(
    const NodeSet& node_set, cssom::CombinatorType combinator_type,
    HTMLElement* element,
    base::Callback<void(HTMLElement* element, const SelectorTree::Node*)>
        callback) {
  // Iterate through all nodes in node_set.
  for (typename NodeSet::const_iterator node_iterator = node_set.begin();
       node_iterator != node_set.end(); ++node_iterator) {
    const SelectorTree::Node* node = *node_iterator;

    if (!node->HasCombinator(combinator_type)) {
      continue;
    }

    SelectorTree::NodeSet<kRuleMatchingNodeSetSize> candidate_nodes;

    // Gather candidate sets in node's children under the given combinator.

    // Type selector.
    if (node->HasSimpleSelector(cssom::kTypeSelector, combinator_type)) {
      GatherCandidateNodesFromMap(cssom::kTypeSelector, combinator_type,
                                  node->selector_nodes_map(),
                                  element->tag_name(), &candidate_nodes);
    }

    // Class selector.
    if (node->HasSimpleSelector(cssom::kClassSelector, combinator_type)) {
      scoped_refptr<DOMTokenList> class_list = element->class_list();
      for (unsigned int index = 0; index < class_list->length(); ++index) {
        GatherCandidateNodesFromMap(
            cssom::kClassSelector, combinator_type, node->selector_nodes_map(),
            class_list->NonNullItem(index), &candidate_nodes);
      }
    }

    // Id selector.
    if (node->HasSimpleSelector(cssom::kIdSelector, combinator_type)) {
      GatherCandidateNodesFromMap(cssom::kIdSelector, combinator_type,
                                  node->selector_nodes_map(), element->id(),
                                  &candidate_nodes);
    }

    // Empty pseudo class.
    if (node->HasPseudoClass(cssom::kEmptyPseudoClass, combinator_type) &&
        element->IsEmpty()) {
      GatherCandidateNodesFromSet(cssom::kEmptyPseudoClass, combinator_type,
                                  node->pseudo_class_nodes(), &candidate_nodes);
    }

    // Focus pseudo class.
    if (node->HasPseudoClass(cssom::kFocusPseudoClass, combinator_type) &&
        element->HasFocus()) {
      GatherCandidateNodesFromSet(cssom::kFocusPseudoClass, combinator_type,
                                  node->pseudo_class_nodes(), &candidate_nodes);
    }

    // Not pseudo class.
    if (node->HasPseudoClass(cssom::kNotPseudoClass, combinator_type)) {
      GatherCandidateNodesFromSet(cssom::kNotPseudoClass, combinator_type,
                                  node->pseudo_class_nodes(), &candidate_nodes);
    }

    // Check all candidate nodes and run callback for matching nodes.
    for (SelectorTree::NodeSet<kRuleMatchingNodeSetSize>::const_iterator
             candidate_node_iterator = candidate_nodes.begin();
         candidate_node_iterator != candidate_nodes.end();
         ++candidate_node_iterator) {
      const SelectorTree::Node* candidate_node = *candidate_node_iterator;

      if (MatchSelectorAndElement(candidate_node->compound_selector(), element,
                                  false)) {
        callback.Run(element, candidate_node);
      }
    }
  }
}

void CalculateMatchingRules(HTMLElement* current_element,
                            const SelectorTree* selector_tree) {
  // Get parent and previous sibling of the current element.
  HTMLElement* parent = current_element->parent_element()
                            ? current_element->parent_element()->AsHTMLElement()
                            : NULL;
  HTMLElement* previous_sibling =
      current_element->previous_element_sibling()
          ? current_element->previous_element_sibling()->AsHTMLElement()
          : NULL;

  // Calculate current element's matching nodes.

  if (parent) {
    // Child combinator.
    ForEachChildOnNodes(parent->rule_matching_state()->matching_nodes,
                        cssom::kChildCombinator, current_element,
                        base::Bind(&AddRulesOnNodeToElement));

    // Descendant combinator.
    ForEachChildOnNodes(
        parent->rule_matching_state()->descendant_potential_nodes,
        cssom::kDescendantCombinator, current_element,
        base::Bind(&AddRulesOnNodeToElement));
  } else {
    // Descendant combinator from root.
    ForEachChildOnNodes(selector_tree->root_set(), cssom::kDescendantCombinator,
                        current_element, base::Bind(&AddRulesOnNodeToElement));
  }

  if (selector_tree->has_sibling_combinators()) {
    if (previous_sibling) {
      // Next sibling combinator.
      ForEachChildOnNodes(
          previous_sibling->rule_matching_state()->matching_nodes,
          cssom::kNextSiblingCombinator, current_element,
          base::Bind(&AddRulesOnNodeToElement));

      // Following sibling combinator.
      ForEachChildOnNodes(previous_sibling->rule_matching_state()
                              ->following_sibling_potential_nodes,
                          cssom::kFollowingSiblingCombinator, current_element,
                          base::Bind(&AddRulesOnNodeToElement));
    }
  }

  // Calculate current element's descendant potential nodes.
  if (parent) {
    current_element->rule_matching_state()->descendant_potential_nodes.insert(
        parent->rule_matching_state()->descendant_potential_nodes.begin(),
        parent->rule_matching_state()->descendant_potential_nodes.end());
  } else {
    current_element->rule_matching_state()->descendant_potential_nodes.insert(
        selector_tree->root());
  }
  current_element->rule_matching_state()->descendant_potential_nodes.insert(
      current_element->rule_matching_state()->matching_nodes.begin(),
      current_element->rule_matching_state()->matching_nodes.end());

  // Calculate current element's following sibling potential nodes.
  if (selector_tree->has_sibling_combinators()) {
    if (previous_sibling) {
      current_element->rule_matching_state()
          ->following_sibling_potential_nodes.insert(
              previous_sibling->rule_matching_state()
                  ->following_sibling_potential_nodes.begin(),
              previous_sibling->rule_matching_state()
                  ->following_sibling_potential_nodes.end());
    }
    current_element->rule_matching_state()
        ->following_sibling_potential_nodes.insert(
            current_element->rule_matching_state()->matching_nodes.begin(),
            current_element->rule_matching_state()->matching_nodes.end());
  }

  // Set the valid flag on the element.
  current_element->SetMatchingRulesValid();
}

void CalculateMatchingRulesRecursively(HTMLElement* current_element,
                                       const SelectorTree* selector_tree) {
  if (!current_element->matching_rules_valid()) {
    CalculateMatchingRules(current_element, selector_tree);
  }

  // Calculate matching rules for current element's children.
  for (Element* element = current_element->first_element_child(); element;
       element = element->next_element_sibling()) {
    HTMLElement* html_element = element->AsHTMLElement();
    DCHECK(html_element);
    CalculateMatchingRulesRecursively(html_element, selector_tree);
  }
}

}  // namespace

//////////////////////////////////////////////////////////////////////////
// Public APIs
//////////////////////////////////////////////////////////////////////////

bool MatchRuleAndElement(cssom::CSSStyleRule* rule, Element* element) {
  for (cssom::Selectors::const_iterator selector_iterator =
           rule->selectors().begin();
       selector_iterator != rule->selectors().end(); ++selector_iterator) {
    DCHECK(*selector_iterator);
    if (MatchSelectorAndElement(*selector_iterator, element, true)) {
      return true;
    }
  }
  return false;
}

void UpdateMatchingRulesUsingSelectorTree(HTMLElement* dom_root,
                                          const SelectorTree* selector_tree) {
  CalculateMatchingRulesRecursively(dom_root, selector_tree);
}

scoped_refptr<Element> QuerySelector(Node* node, const std::string& selectors,
                                     cssom::CSSParser* css_parser) {
  DCHECK(css_parser);

  // Generate a rule with the given selectors and no style.
  scoped_refptr<cssom::CSSRule> css_rule =
      css_parser->ParseRule(selectors + " {}", node->GetInlineSourceLocation());
  if (!css_rule) {
    return NULL;
  }
  scoped_refptr<cssom::CSSStyleRule> css_style_rule =
      css_rule->AsCSSStyleRule();
  if (!css_style_rule) {
    return NULL;
  }

  // Iterate through the descendants of the node and find the first matching
  // element if any.
  NodeDescendantsIterator iterator(node);
  Node* child = iterator.First();
  while (child) {
    if (child->IsElement()) {
      scoped_refptr<Element> element = child->AsElement();
      if (MatchRuleAndElement(css_style_rule, element)) {
        return element;
      }
    }
    child = iterator.Next();
  }
  return NULL;
}

scoped_refptr<NodeList> QuerySelectorAll(Node* node,
                                         const std::string& selectors,
                                         cssom::CSSParser* css_parser) {
  DCHECK(css_parser);

  scoped_refptr<NodeList> node_list = new NodeList();

  // Generate a rule with the given selectors and no style.
  scoped_refptr<cssom::CSSRule> css_rule =
      css_parser->ParseRule(selectors + " {}", node->GetInlineSourceLocation());
  if (!css_rule) {
    return node_list;
  }
  scoped_refptr<cssom::CSSStyleRule> css_style_rule =
      css_rule->AsCSSStyleRule();
  if (!css_style_rule) {
    return node_list;
  }

  // Iterate through the descendants of the node and find the matching elements.
  NodeDescendantsIterator iterator(node);
  Node* child = iterator.First();
  while (child) {
    if (child->IsElement()) {
      scoped_refptr<Element> element = child->AsElement();
      if (MatchRuleAndElement(css_style_rule, element)) {
        node_list->AppendNode(element);
      }
    }
    child = iterator.Next();
  }
  return node_list;
}

}  // namespace dom
}  // namespace cobalt
