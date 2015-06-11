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

#ifndef DOM_HTML_ELEMENT_H_
#define DOM_HTML_ELEMENT_H_

#include "base/memory/ref_counted.h"
#include "base/memory/scoped_ptr.h"
#include "base/string_piece.h"
#include "cobalt/base/source_location.h"
#include "cobalt/cssom/css_parser.h"
#include "cobalt/cssom/css_style_declaration.h"
#include "cobalt/cssom/css_style_rule.h"
#include "cobalt/cssom/css_transition_set.h"
#include "cobalt/dom/element.h"

namespace cobalt {
namespace dom {

class HTMLBodyElement;
class HTMLDivElement;
class HTMLElementFactory;
class HTMLHeadElement;
class HTMLHtmlElement;
class HTMLLinkElement;
class HTMLMediaElement;
class HTMLScriptElement;
class HTMLSpanElement;
class HTMLStyleElement;

// The basic interface, from which all the HTML elements' interfaces inherit,
// and which must be used by elements that have no additional requirements.
//   http://www.w3.org/TR/html5/dom.html#htmlelement
class HTMLElement : public Element {
 public:
  // Web API: ElementCSSInlineStyle
  // Extended in CSSOM specification.
  //   http://www.w3.org/TR/2013/WD-cssom-20131205/#elementcssinlinestyle
  const scoped_refptr<cssom::CSSStyleDeclaration>& style() { return style_; }

  // Custom, not in any spec: Element.
  scoped_refptr<HTMLElement> AsHTMLElement() OVERRIDE { return this; }

  // Custom, not in any spec.
  // Safe type conversion methods that will downcast to the required type if
  // possible or return NULL otherwise.
  virtual scoped_refptr<HTMLBodyElement> AsHTMLBodyElement();
  virtual scoped_refptr<HTMLDivElement> AsHTMLDivElement();
  virtual scoped_refptr<HTMLHeadElement> AsHTMLHeadElement();
  virtual scoped_refptr<HTMLHtmlElement> AsHTMLHtmlElement();
  virtual scoped_refptr<HTMLLinkElement> AsHTMLLinkElement();
  virtual scoped_refptr<HTMLMediaElement> AsHTMLMediaElement();
  virtual scoped_refptr<HTMLScriptElement> AsHTMLScriptElement();
  virtual scoped_refptr<HTMLSpanElement> AsHTMLSpanElement();
  virtual scoped_refptr<HTMLStyleElement> AsHTMLStyleElement();

  // Points to ">" of opening tag.
  virtual void SetOpeningTagLocation(
      const base::SourceLocation& opening_tag_location);

  // Used by layout engine to cache the computed values.
  // See http://www.w3.org/TR/css-cascade-3/#computed for the definition of
  // computed value.
  scoped_refptr<const cssom::CSSStyleDeclarationData> computed_style() {
    return computed_style_;
  }
  // TODO(***REMOVED***): Get rid of this setter by moving the update of a computed
  // style into HTMLElement class.
  void set_computed_style(
      const scoped_refptr<cssom::CSSStyleDeclarationData>& computed_style) {
    computed_style_ = computed_style;
  }

  // Used by layout engine to cache the rule matching results.
  cssom::RulesWithCascadePriority* matching_rules() {
    return matching_rules_.get();
  }
  // TODO(***REMOVED***): Get rid of this setter by moving the update of matching rules
  // into HTMLElement class.
  void set_matching_rules(
      scoped_ptr<cssom::RulesWithCascadePriority> matching_rules) {
    matching_rules_.reset(matching_rules.release());
  }

  cssom::TransitionSet* transitions() { return &transitions_; }

  DEFINE_WRAPPABLE_TYPE(HTMLElement);

 protected:
  HTMLElement(HTMLElementFactory* html_element_factory,
              cssom::CSSParser* css_parser);
  ~HTMLElement() OVERRIDE;

  void AttachToDocument(Document* document) OVERRIDE;

  cssom::CSSParser* const css_parser_;

 private:
  void UpdateTransitions(const cssom::CSSStyleDeclarationData* source,
                         const cssom::CSSStyleDeclarationData* destination);

  scoped_refptr<cssom::CSSStyleDeclaration> style_;
  scoped_refptr<cssom::CSSStyleDeclarationData> computed_style_;
  scoped_ptr<cssom::RulesWithCascadePriority> matching_rules_;

  cssom::TransitionSet transitions_;
};

}  // namespace dom
}  // namespace cobalt

#endif  // DOM_HTML_ELEMENT_H_
