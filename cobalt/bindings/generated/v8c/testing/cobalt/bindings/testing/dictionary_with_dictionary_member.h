// Copyright 2018 Google Inc. All Rights Reserved.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

// clang-format off

// This file has been auto-generated by bindings/code_generator_cobalt.py. DO NOT MODIFY!
// Auto-generated from template: bindings/templates/dictionary.h.template

#ifndef DictionaryWithDictionaryMember_h
#define DictionaryWithDictionaryMember_h

#include <string>

#include "base/optional.h"
#include "cobalt/script/array_buffer.h"
#include "cobalt/script/array_buffer_view.h"
#include "cobalt/script/script_value.h"
#include "cobalt/script/sequence.h"
#include "cobalt/script/typed_arrays.h"
#include "cobalt/script/value_handle.h"
#include "cobalt/bindings/testing/test_dictionary.h"

using cobalt::bindings::testing::TestDictionary;

namespace cobalt {
namespace bindings {
namespace testing {

class DictionaryWithDictionaryMember {
 public:
  DictionaryWithDictionaryMember() {
    has_nested_dictionary_ = false;
    nested_dictionary_ = TestDictionary();
  }

  DictionaryWithDictionaryMember(const DictionaryWithDictionaryMember& other) {
    has_nested_dictionary_ = other.has_nested_dictionary_;
    nested_dictionary_ = other.nested_dictionary_;
  }

  DictionaryWithDictionaryMember& operator=(const DictionaryWithDictionaryMember& other) {
    has_nested_dictionary_ = other.has_nested_dictionary_;
    nested_dictionary_ = other.nested_dictionary_;
    return *this;
  }

  bool has_nested_dictionary() const {
    return has_nested_dictionary_;
  }
  TestDictionary nested_dictionary() const {
    DCHECK(has_nested_dictionary_);
    return nested_dictionary_;
  }
  void set_nested_dictionary(TestDictionary value) {
    has_nested_dictionary_ = true;
    nested_dictionary_ = value;
  }


 private:
  bool has_nested_dictionary_;
  TestDictionary nested_dictionary_;
};

// This ostream override is necessary for MOCK_METHODs commonly used
// in idl test code
inline std::ostream& operator<<(
    std::ostream& stream, const cobalt::bindings::testing::DictionaryWithDictionaryMember& in) {
  UNREFERENCED_PARAMETER(in);
  stream << "[DictionaryWithDictionaryMember]";
  return stream;
}

}  // namespace cobalt
}  // namespace bindings
}  // namespace testing

#endif  // DictionaryWithDictionaryMember_h
