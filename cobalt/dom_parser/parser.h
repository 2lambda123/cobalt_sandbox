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

#ifndef DOM_PARSER_PARSER_H_
#define DOM_PARSER_PARSER_H_

#include <string>

#include "base/callback.h"
#include "base/memory/ref_counted.h"
#include "cobalt/dom/document.h"
#include "cobalt/dom/parser.h"
#include "cobalt/dom/xml_document.h"
#include "cobalt/dom_parser/document_builder.h"

namespace cobalt {
namespace dom_parser {

class Parser : public dom::Parser {
 public:
  Parser() {}
  explicit Parser(
      const base::Callback<void(const std::string&)>& error_callback)
      : error_callback_(error_callback) {}
  ~Parser() OVERRIDE {}

  // From dom::Parser.
  //
  scoped_refptr<dom::Document> ParseDocument(
      const std::string& input, dom::HTMLElementContext* html_element_context,
      const base::SourceLocation& input_location) OVERRIDE;

  scoped_refptr<dom::XMLDocument> ParseXMLDocument(
      const std::string& input, dom::HTMLElementContext* html_element_context,
      const base::SourceLocation& input_location) OVERRIDE;

  void ParseDocumentFragment(
      const std::string& input, const scoped_refptr<dom::Document>& document,
      const scoped_refptr<dom::Node>& parent_node,
      const scoped_refptr<dom::Node>& reference_node,
      const base::SourceLocation& input_location) OVERRIDE;

  void ParseXMLDocumentFragment(
      const std::string& input,
      const scoped_refptr<dom::XMLDocument>& xml_document,
      const scoped_refptr<dom::Node>& parent_node,
      const scoped_refptr<dom::Node>& reference_node,
      const base::SourceLocation& input_location) OVERRIDE;

  // This function starts an asynchronous process that loads a document from the
  // given url. Note it requires a message loop to function, i.e. the loading
  // will happen during the message loop. If the parser is destroyed before
  // the document building finishes, the process is cancelled.
  void BuildDocument(const GURL& url,
                     scoped_refptr<dom::Document> document) OVERRIDE;

 private:
  void ErrorCallback(const std::string& error);

  const base::Callback<void(const std::string&)> error_callback_;
  scoped_ptr<DocumentBuilder> document_builder_;

  DISALLOW_COPY_AND_ASSIGN(Parser);
};

}  // namespace dom_parser
}  // namespace cobalt

#endif  // DOM_PARSER_PARSER_H_
