/*
 * Copyright 2017 Google Inc. All Rights Reserved.
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

// clang-format off

// This file has been auto-generated by bindings/code_generator_cobalt.py. DO NOT MODIFY!
// Auto-generated from template: bindings/mozjs/templates/dictionary-conversion.cc.template

#include "mozjs_gen_type_conversion.h"

#include "cobalt/bindings/testing/test_dictionary.h"

#include "cobalt/script/exception_state.h"
#include "third_party/mozjs/js/src/jsapi.h"
#include "cobalt/bindings/testing/arbitrary_interface.h"
#include "cobalt/bindings/testing/mozjs_arbitrary_interface.h"

using cobalt::bindings::testing::TestDictionary;
using cobalt::bindings::testing::ArbitraryInterface;
using cobalt::bindings::testing::MozjsArbitraryInterface;

namespace cobalt {
namespace script {
namespace mozjs {

void ToJSValue(
    JSContext* context,
    const TestDictionary& in_dictionary,
    JS::MutableHandleValue out_value) {
  // Create a new object that will hold the dictionary values.
  JS::RootedObject dictionary_object(
      context, JS_NewObject(context, NULL, NULL, NULL));
  const int kPropertyAttributes = JSPROP_ENUMERATE;
  if (in_dictionary.has_boolean_member()) {
    JS::RootedValue member_value(context);
    ToJSValue(context, in_dictionary.boolean_member(), &member_value);
    if (!JS_DefineProperty(context, dictionary_object,
                           "booleanMember",
                           member_value, NULL, NULL, kPropertyAttributes)) {
      // Some internal error occurred.
      NOTREACHED();
      return;
    }
  }
  if (in_dictionary.has_short_clamp_member()) {
    JS::RootedValue member_value(context);
    ToJSValue(context, in_dictionary.short_clamp_member(), &member_value);
    if (!JS_DefineProperty(context, dictionary_object,
                           "shortClampMember",
                           member_value, NULL, NULL, kPropertyAttributes)) {
      // Some internal error occurred.
      NOTREACHED();
      return;
    }
  }
  if (in_dictionary.has_long_member()) {
    JS::RootedValue member_value(context);
    ToJSValue(context, in_dictionary.long_member(), &member_value);
    if (!JS_DefineProperty(context, dictionary_object,
                           "longMember",
                           member_value, NULL, NULL, kPropertyAttributes)) {
      // Some internal error occurred.
      NOTREACHED();
      return;
    }
  }
  if (in_dictionary.has_double_member()) {
    JS::RootedValue member_value(context);
    ToJSValue(context, in_dictionary.double_member(), &member_value);
    if (!JS_DefineProperty(context, dictionary_object,
                           "doubleMember",
                           member_value, NULL, NULL, kPropertyAttributes)) {
      // Some internal error occurred.
      NOTREACHED();
      return;
    }
  }
  if (in_dictionary.has_string_member()) {
    JS::RootedValue member_value(context);
    ToJSValue(context, in_dictionary.string_member(), &member_value);
    if (!JS_DefineProperty(context, dictionary_object,
                           "stringMember",
                           member_value, NULL, NULL, kPropertyAttributes)) {
      // Some internal error occurred.
      NOTREACHED();
      return;
    }
  }
  if (in_dictionary.has_interface_member()) {
    JS::RootedValue member_value(context);
    ToJSValue(context, in_dictionary.interface_member(), &member_value);
    if (!JS_DefineProperty(context, dictionary_object,
                           "interfaceMember",
                           member_value, NULL, NULL, kPropertyAttributes)) {
      // Some internal error occurred.
      NOTREACHED();
      return;
    }
  }
  if (in_dictionary.has_member_with_default()) {
    JS::RootedValue member_value(context);
    ToJSValue(context, in_dictionary.member_with_default(), &member_value);
    if (!JS_DefineProperty(context, dictionary_object,
                           "memberWithDefault",
                           member_value, NULL, NULL, kPropertyAttributes)) {
      // Some internal error occurred.
      NOTREACHED();
      return;
    }
  }
  if (in_dictionary.has_non_default_member()) {
    JS::RootedValue member_value(context);
    ToJSValue(context, in_dictionary.non_default_member(), &member_value);
    if (!JS_DefineProperty(context, dictionary_object,
                           "nonDefaultMember",
                           member_value, NULL, NULL, kPropertyAttributes)) {
      // Some internal error occurred.
      NOTREACHED();
      return;
    }
  }
  if (in_dictionary.has_any_member_with_default()) {
    JS::RootedValue member_value(context);
    ToJSValue(context, in_dictionary.any_member_with_default(), &member_value);
    if (!JS_DefineProperty(context, dictionary_object,
                           "anyMemberWithDefault",
                           member_value, NULL, NULL, kPropertyAttributes)) {
      // Some internal error occurred.
      NOTREACHED();
      return;
    }
  }
  if (in_dictionary.has_any_member()) {
    JS::RootedValue member_value(context);
    ToJSValue(context, in_dictionary.any_member(), &member_value);
    if (!JS_DefineProperty(context, dictionary_object,
                           "anyMember",
                           member_value, NULL, NULL, kPropertyAttributes)) {
      // Some internal error occurred.
      NOTREACHED();
      return;
    }
  }
  out_value.set(OBJECT_TO_JSVAL(dictionary_object));
}

void FromJSValue(JSContext* context, JS::HandleValue value,
                 int conversion_flags, ExceptionState* exception_state,
                 TestDictionary* out_dictionary) {
  DCHECK_EQ(0, conversion_flags) << "Unexpected conversion flags.";
  // https://heycam.github.io/webidl/#es-dictionary

  if (value.isUndefined() || value.isNull()) {
    // The default constructor will assign appropriate values to dictionary
    // members with default values and leave the others unset.
    *out_dictionary = TestDictionary();
    return;
  }
  if (!value.isObject()) {
    // 1. If Type(V) is not Undefined, Null or Object, then throw a TypeError.
    exception_state->SetSimpleException(kNotObjectType);
    return;
  }
  JS::RootedObject dictionary_object(context, JSVAL_TO_OBJECT(value));
  JS::RootedValue boolean_member(context);
  if (!JS_GetProperty(context, dictionary_object,
                      "booleanMember",
                      boolean_member.address())) {
    exception_state->SetSimpleException(kSimpleError);
    return;
  }
  if (!boolean_member.isUndefined()) {
    bool converted_value;
    FromJSValue(context,
                boolean_member,
                kNoConversionFlags,
                exception_state,
                &converted_value);
    if (context->isExceptionPending()) {
      return;
    }
    out_dictionary->set_boolean_member(converted_value);
  }
  JS::RootedValue short_clamp_member(context);
  if (!JS_GetProperty(context, dictionary_object,
                      "shortClampMember",
                      short_clamp_member.address())) {
    exception_state->SetSimpleException(kSimpleError);
    return;
  }
  if (!short_clamp_member.isUndefined()) {
    int16_t converted_value;
    FromJSValue(context,
                short_clamp_member,
                (kConversionFlagClamped),
                exception_state,
                &converted_value);
    if (context->isExceptionPending()) {
      return;
    }
    out_dictionary->set_short_clamp_member(converted_value);
  }
  JS::RootedValue long_member(context);
  if (!JS_GetProperty(context, dictionary_object,
                      "longMember",
                      long_member.address())) {
    exception_state->SetSimpleException(kSimpleError);
    return;
  }
  if (!long_member.isUndefined()) {
    int32_t converted_value;
    FromJSValue(context,
                long_member,
                kNoConversionFlags,
                exception_state,
                &converted_value);
    if (context->isExceptionPending()) {
      return;
    }
    out_dictionary->set_long_member(converted_value);
  }
  JS::RootedValue double_member(context);
  if (!JS_GetProperty(context, dictionary_object,
                      "doubleMember",
                      double_member.address())) {
    exception_state->SetSimpleException(kSimpleError);
    return;
  }
  if (!double_member.isUndefined()) {
    double converted_value;
    FromJSValue(context,
                double_member,
                (kConversionFlagRestricted),
                exception_state,
                &converted_value);
    if (context->isExceptionPending()) {
      return;
    }
    out_dictionary->set_double_member(converted_value);
  }
  JS::RootedValue string_member(context);
  if (!JS_GetProperty(context, dictionary_object,
                      "stringMember",
                      string_member.address())) {
    exception_state->SetSimpleException(kSimpleError);
    return;
  }
  if (!string_member.isUndefined()) {
    std::string converted_value;
    FromJSValue(context,
                string_member,
                kNoConversionFlags,
                exception_state,
                &converted_value);
    if (context->isExceptionPending()) {
      return;
    }
    out_dictionary->set_string_member(converted_value);
  }
  JS::RootedValue interface_member(context);
  if (!JS_GetProperty(context, dictionary_object,
                      "interfaceMember",
                      interface_member.address())) {
    exception_state->SetSimpleException(kSimpleError);
    return;
  }
  if (!interface_member.isUndefined()) {
    scoped_refptr<ArbitraryInterface> converted_value;
    FromJSValue(context,
                interface_member,
                kNoConversionFlags,
                exception_state,
                &converted_value);
    if (context->isExceptionPending()) {
      return;
    }
    out_dictionary->set_interface_member(converted_value);
  }
  JS::RootedValue member_with_default(context);
  if (!JS_GetProperty(context, dictionary_object,
                      "memberWithDefault",
                      member_with_default.address())) {
    exception_state->SetSimpleException(kSimpleError);
    return;
  }
  if (!member_with_default.isUndefined()) {
    int32_t converted_value;
    FromJSValue(context,
                member_with_default,
                kNoConversionFlags,
                exception_state,
                &converted_value);
    if (context->isExceptionPending()) {
      return;
    }
    out_dictionary->set_member_with_default(converted_value);
  }
  JS::RootedValue non_default_member(context);
  if (!JS_GetProperty(context, dictionary_object,
                      "nonDefaultMember",
                      non_default_member.address())) {
    exception_state->SetSimpleException(kSimpleError);
    return;
  }
  if (!non_default_member.isUndefined()) {
    int32_t converted_value;
    FromJSValue(context,
                non_default_member,
                kNoConversionFlags,
                exception_state,
                &converted_value);
    if (context->isExceptionPending()) {
      return;
    }
    out_dictionary->set_non_default_member(converted_value);
  }
  JS::RootedValue any_member_with_default(context);
  if (!JS_GetProperty(context, dictionary_object,
                      "anyMemberWithDefault",
                      any_member_with_default.address())) {
    exception_state->SetSimpleException(kSimpleError);
    return;
  }
  if (!any_member_with_default.isUndefined()) {
    TypeTraits<::cobalt::script::ValueHandle >::ConversionType converted_value;
    FromJSValue(context,
                any_member_with_default,
                kNoConversionFlags,
                exception_state,
                &converted_value);
    if (context->isExceptionPending()) {
      return;
    }
    out_dictionary->set_any_member_with_default(&converted_value);
  }
  JS::RootedValue any_member(context);
  if (!JS_GetProperty(context, dictionary_object,
                      "anyMember",
                      any_member.address())) {
    exception_state->SetSimpleException(kSimpleError);
    return;
  }
  if (!any_member.isUndefined()) {
    TypeTraits<::cobalt::script::ValueHandle >::ConversionType converted_value;
    FromJSValue(context,
                any_member,
                kNoConversionFlags,
                exception_state,
                &converted_value);
    if (context->isExceptionPending()) {
      return;
    }
    out_dictionary->set_any_member(&converted_value);
  }
}

}  // namespace mozjs
}  // namespace script
}  // namespace cobalt

