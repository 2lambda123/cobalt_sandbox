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

#ifndef CSSOM_STRING_VALUE_H_
#define CSSOM_STRING_VALUE_H_

#include <iostream>
#include <string>

#include "base/basictypes.h"
#include "base/compiler_specific.h"
#include "cobalt/cssom/property_value.h"

namespace cobalt {
namespace cssom {

// Represents a sequence of characters delimited by single or double quotes.
// Applies to properties like font-family.
// See http://www.w3.org/TR/css3-values/#strings for details.
class StringValue : public PropertyValue {
 public:
  explicit StringValue(const std::string& value) : value_(value) {}

  virtual void Accept(PropertyValueVisitor* visitor) OVERRIDE;

  const std::string& value() const { return value_; }

 private:
  ~StringValue() OVERRIDE {}

  const std::string value_;

  DISALLOW_COPY_AND_ASSIGN(StringValue);
};

inline bool operator==(const StringValue& lhs, const StringValue& rhs) {
  return lhs.value() == rhs.value();
}

// Used by tests.
inline std::ostream& operator<<(std::ostream& stream,
                                const StringValue& string) {
  stream << "\"" << string.value() << "\"";
  return stream;
}

}  // namespace cssom
}  // namespace cobalt

#endif  // CSSOM_STRING_VALUE_H_
