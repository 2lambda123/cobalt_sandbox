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

#ifndef BINDINGS_TESTING_DERIVED_GETTER_SETTER_INTERFACE_H_
#define BINDINGS_TESTING_DERIVED_GETTER_SETTER_INTERFACE_H_

#include "cobalt/bindings/testing/getter_setter_interface.h"
#include "testing/gmock/include/gmock/gmock.h"

namespace cobalt {
namespace bindings {
namespace testing {

class DerivedGetterSetterInterface : public GetterSetterInterface {
 public:
  MOCK_METHOD0(length, uint32_t());
  MOCK_METHOD1(DerivedIndexedGetter, uint32_t(uint32_t));
  MOCK_METHOD2(DerivedIndexedSetter, void(uint32_t, uint32_t));

  DEFINE_WRAPPABLE_TYPE(DerivedGetterSetterInterface);
};

}  // namespace testing
}  // namespace bindings
}  // namespace cobalt

#endif  // BINDINGS_TESTING_DERIVED_GETTER_SETTER_INTERFACE_H_
