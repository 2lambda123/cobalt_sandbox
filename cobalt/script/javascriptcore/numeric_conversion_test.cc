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

#include <limits>

#include "config.h"
#undef LOG

#include "cobalt/base/polymorphic_downcast.h"
#include "cobalt/script/global_object_proxy.h"
#include "cobalt/script/javascript_engine.h"
#include "cobalt/script/javascriptcore/conversion_helpers.h"
#include "cobalt/script/javascriptcore/jsc_global_object.h"
#include "cobalt/script/javascriptcore/jsc_global_object_proxy.h"
#include "testing/gtest/include/gtest/gtest.h"
#include "third_party/WebKit/Source/JavaScriptCore/runtime/JSFunction.h"
#include "third_party/WebKit/Source/JavaScriptCore/runtime/JSString.h"
#include "third_party/WebKit/Source/JavaScriptCore/runtime/JSValue.h"
#include "third_party/WebKit/Source/WTF/wtf/text/WTFString.h"

namespace cobalt {
namespace script {
namespace javascriptcore {

namespace {

template <int kNumber>
JSC::EncodedJSValue returnNumberFunction(JSC::ExecState* exec) {
  return JSC::JSValue::encode(JSC::jsNumber(kNumber));
}

template <typename T>
class NumericConversionTest : public ::testing::Test {
 public:
  NumericConversionTest()
      : engine_(JavaScriptEngine::CreateEngine()),
        global_object_proxy_(engine_->CreateGlobalObject()),
        jsc_global_object_(base::polymorphic_downcast<JSCGlobalObjectProxy*>(
                               global_object_proxy_.get())->global_object()),
        exec_state_(jsc_global_object_->globalExec()) {}

  void AddFunction(JSC::JSObject* object, const char* name,
                   JSC::NativeFunction function) {
    int num_arguments = 0;
    JSC::Identifier identifier(jsc_global_object_->globalExec(), name);
    object->putDirect(jsc_global_object_->globalData(), identifier,
                      JSC::JSFunction::create(jsc_global_object_->globalExec(),
                                              jsc_global_object_, num_arguments,
                                              identifier.string(), function));
  }

  const scoped_ptr<JavaScriptEngine> engine_;
  const scoped_refptr<GlobalObjectProxy> global_object_proxy_;
  JSCGlobalObject* const jsc_global_object_;
  JSC::ExecState* exec_state_;
};

typedef ::testing::Types<int8_t, uint8_t, int16_t, uint16_t, int32_t, uint32_t>
    NumericTypes;
TYPED_TEST_CASE(NumericConversionTest, NumericTypes);

}  // namespace

// Conversion between a JavaScript value and an IDL integer type is described
// here:
//     http://www.w3.org/TR/WebIDL/#es-byte
//     http://www.w3.org/TR/WebIDL/#es-octet
//     http://www.w3.org/TR/WebIDL/#es-short
//     http://www.w3.org/TR/WebIDL/#es-unsigned-short
//     http://www.w3.org/TR/WebIDL/#es-long
//     http://www.w3.org/TR/WebIDL/#es-unsigned-long
// The first step in each of these algorithms is the ToNumber operation:
//     http://es5.github.io/#x9.3
// ToNumber describes how various non-numeric types should convert to a
// number.

// ToNumber: http://es5.github.io/#x9.3
TYPED_TEST(NumericConversionTest, BooleanConversion) {
  EXPECT_EQ(
      1, JSValueToNumber<TypeParam>(this->exec_state_, JSC::jsBoolean(true)));
  EXPECT_EQ(
      0, JSValueToNumber<TypeParam>(this->exec_state_, JSC::jsBoolean(false)));
}

// ToNumber applied to the String Type: http://es5.github.io/#x9.3.1
TYPED_TEST(NumericConversionTest, StringConversion) {
  JSC::ExecState* exec = this->jsc_global_object_->globalExec();
  EXPECT_EQ(0, JSValueToNumber<TypeParam>(
                   this->exec_state_, JSC::jsString(this->exec_state_, "")));
  EXPECT_EQ(
      0, JSValueToNumber<TypeParam>(this->exec_state_,
                                    JSC::jsString(this->exec_state_, "    ")));
  EXPECT_EQ(0, JSValueToNumber<TypeParam>(
                   this->exec_state_,
                   JSC::jsString(this->exec_state_, "not_a_number")));
  EXPECT_EQ(32, JSValueToNumber<TypeParam>(
                    this->exec_state_, JSC::jsString(this->exec_state_, "32")));
  EXPECT_EQ(
      32, JSValueToNumber<TypeParam>(this->exec_state_,
                                     JSC::jsString(this->exec_state_, "0x20")));
}

// Described in the integer type conversion algorithms:
//     Set x to sign(x)*floor(abs(x))
// Also in ToUint16, ToInt32, and ToUInt64:
//     3. Let posInt be sign(number) * floor(abs(number))
TYPED_TEST(NumericConversionTest, FloatingPointToIntegerConversion) {
  EXPECT_EQ(5,
            JSValueToNumber<TypeParam>(this->exec_state_, JSC::jsNumber(5.1)));
  if (std::numeric_limits<TypeParam>::is_signed) {
    EXPECT_EQ(
        -5, JSValueToNumber<TypeParam>(this->exec_state_, JSC::jsNumber(-5.1)));
  }
}

// http://es5.github.io/#x9.3
TYPED_TEST(NumericConversionTest, OtherConversions) {
  EXPECT_EQ(0, JSValueToNumber<TypeParam>(this->exec_state_, JSC::jsNull()));
  EXPECT_EQ(0,
            JSValueToNumber<TypeParam>(this->exec_state_, JSC::jsUndefined()));
  EXPECT_EQ(0, JSValueToNumber<TypeParam>(
                   this->exec_state_,
                   JSC::jsNumber(std::numeric_limits<double>::infinity())));
  EXPECT_EQ(0, JSValueToNumber<TypeParam>(
                   this->exec_state_,
                   JSC::jsNumber(-std::numeric_limits<double>::infinity())));
  EXPECT_EQ(0, JSValueToNumber<TypeParam>(this->exec_state_, JSC::jsNaN()));
}


// ToNumber (http://es5.github.io/#x9.3) calls the ToPrimitive operation:
//     http://es5.github.io/#x9.1
// ToPrimitive calls the [[DefaultValue]] method of the object:
//     http://es5.github.io/#x8.12.8
TYPED_TEST(NumericConversionTest, ObjectConversion) {
  JSC::JSLockHolder lock(this->jsc_global_object_->globalData());
  JSC::Structure* structure =
      JSC::createEmptyObjectStructure(this->jsc_global_object_->globalData(),
                                      this->jsc_global_object_, JSC::jsNull());
  {
    JSC::JSObject* object =
        JSC::constructEmptyObject(this->exec_state_, structure);
    this->AddFunction(object, "valueOf", &(returnNumberFunction<5>));
    EXPECT_EQ(5,
              JSValueToNumber<TypeParam>(this->jsc_global_object_->globalExec(),
                                         JSC::JSValue(object)));
  }
  {
    JSC::JSObject* object =
        JSC::constructEmptyObject(this->exec_state_, structure);
    // The conversion algorithm uses the value of toString() if it is
    // a primitive value, which is not necessarily a string.
    this->AddFunction(object, "toString", &(returnNumberFunction<5>));
    EXPECT_EQ(
        5, JSValueToNumber<TypeParam>(this->exec_state_, JSC::JSValue(object)));
  }
  {
    JSC::JSObject* object =
        JSC::constructEmptyObject(this->exec_state_, structure);
    EXPECT_EQ(
        0, JSValueToNumber<TypeParam>(this->exec_state_, JSC::JSValue(object)));
  }
}

}  // namespace javascriptcore
}  // namespace script
}  // namespace cobalt
