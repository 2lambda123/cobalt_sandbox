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

#ifndef CSS_PARSER_PARSER_H_
#define CSS_PARSER_PARSER_H_

#include "base/callback.h"
#include "cobalt/cssom/css_parser.h"

namespace cobalt {
namespace css_parser {

class Parser : public cssom::CSSParser {
 public:
  static scoped_ptr<Parser> Create();
  ~Parser();

  scoped_refptr<cssom::CSSStyleSheet> ParseStyleSheet(
      const std::string& file_name, const std::string& input) OVERRIDE;
  scoped_refptr<cssom::CSSStyleSheet> ParseStyleSheetWithBeginLine(
      const std::string& file_name, const std::string& input,
      int begin_line) OVERRIDE;

 private:
  typedef base::Callback<void(const std::string& message)> OnMessageCallback;

  Parser(const OnMessageCallback& on_warning_callback,
         const OnMessageCallback& on_error_callback);

  const OnMessageCallback on_warning_callback_;
  const OnMessageCallback on_error_callback_;

  friend class ParserImpl;
  friend class ParserTest;
  DISALLOW_COPY_AND_ASSIGN(Parser);
};

}  // namespace css_parser
}  // namespace cobalt

#endif  // CSS_PARSER_PARSER_H_
