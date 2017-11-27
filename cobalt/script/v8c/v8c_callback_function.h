// This file was GENERATED by command:
//     pump.py v8c_callback_function.h.pump
// DO NOT EDIT BY HAND!!!


// Copyright 2017 Google Inc. All Rights Reserved.
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

#ifndef COBALT_SCRIPT_V8C_V8C_CALLBACK_FUNCTION_H_
#define COBALT_SCRIPT_V8C_V8C_CALLBACK_FUNCTION_H_

#include "base/logging.h"
#include "cobalt/script/callback_function.h"
#include "cobalt/script/v8c/conversion_helpers.h"
#include "cobalt/script/v8c/weak_heap_object.h"
#include "nb/memory_scope.h"
#include "v8/include/v8.h"

namespace cobalt {
namespace script {
namespace v8c {

// First, we forward declare the Callback class template. This informs the
// compiler that the template only has 1 type parameter which is the base
// CallbackFunction template class with parameters.
//
// See base/callback.h.pump for further discussion on this pattern.
template <typename Sig>
class V8cCallbackFunction;

template <typename R>
class V8cCallbackFunction<R(void)>
    : public CallbackFunction<R(void)> {
 public:
  typedef CallbackFunction<R()> BaseType;

  V8cCallbackFunction(V8cGlobalEnvironment* env, v8::Local<v8::Value> function)
    : env_(env), weak_function_(env, function) {
      DCHECK(env_);
      DCHECK(function->IsFunction());
  }

  CallbackResult<R> Run()
      const override {
    NOTIMPLEMENTED();
    CallbackResult<R> callback_result;
    return callback_result;
  }

  v8::Local<v8::Value> value() const { return weak_function_.GetValue(); }

 private:
  V8cGlobalEnvironment* env_;
  WeakHeapObject weak_function_;
};

template <typename R, typename A1>
class V8cCallbackFunction<R(A1)>
    : public CallbackFunction<R(A1)> {
 public:
  typedef CallbackFunction<R(A1)> BaseType;

  V8cCallbackFunction(V8cGlobalEnvironment* env, v8::Local<v8::Value> function)
    : env_(env), weak_function_(env, function) {
      DCHECK(env_);
      DCHECK(function->IsFunction());
  }

  CallbackResult<R> Run(
      typename base::internal::CallbackParamTraits<A1>::ForwardType a1)
      const override {
    NOTIMPLEMENTED();
    CallbackResult<R> callback_result;
    return callback_result;
  }

  v8::Local<v8::Value> value() const { return weak_function_.GetValue(); }

 private:
  V8cGlobalEnvironment* env_;
  WeakHeapObject weak_function_;
};

template <typename R, typename A1, typename A2>
class V8cCallbackFunction<R(A1, A2)>
    : public CallbackFunction<R(A1, A2)> {
 public:
  typedef CallbackFunction<R(A1, A2)> BaseType;

  V8cCallbackFunction(V8cGlobalEnvironment* env, v8::Local<v8::Value> function)
    : env_(env), weak_function_(env, function) {
      DCHECK(env_);
      DCHECK(function->IsFunction());
  }

  CallbackResult<R> Run(
      typename base::internal::CallbackParamTraits<A1>::ForwardType a1,
      typename base::internal::CallbackParamTraits<A2>::ForwardType a2)
      const override {
    NOTIMPLEMENTED();
    CallbackResult<R> callback_result;
    return callback_result;
  }

  v8::Local<v8::Value> value() const { return weak_function_.GetValue(); }

 private:
  V8cGlobalEnvironment* env_;
  WeakHeapObject weak_function_;
};

template <typename R, typename A1, typename A2, typename A3>
class V8cCallbackFunction<R(A1, A2, A3)>
    : public CallbackFunction<R(A1, A2, A3)> {
 public:
  typedef CallbackFunction<R(A1, A2, A3)> BaseType;

  V8cCallbackFunction(V8cGlobalEnvironment* env, v8::Local<v8::Value> function)
    : env_(env), weak_function_(env, function) {
      DCHECK(env_);
      DCHECK(function->IsFunction());
  }

  CallbackResult<R> Run(
      typename base::internal::CallbackParamTraits<A1>::ForwardType a1,
      typename base::internal::CallbackParamTraits<A2>::ForwardType a2,
      typename base::internal::CallbackParamTraits<A3>::ForwardType a3)
      const override {
    NOTIMPLEMENTED();
    CallbackResult<R> callback_result;
    return callback_result;
  }

  v8::Local<v8::Value> value() const { return weak_function_.GetValue(); }

 private:
  V8cGlobalEnvironment* env_;
  WeakHeapObject weak_function_;
};

template <typename R, typename A1, typename A2, typename A3, typename A4>
class V8cCallbackFunction<R(A1, A2, A3, A4)>
    : public CallbackFunction<R(A1, A2, A3, A4)> {
 public:
  typedef CallbackFunction<R(A1, A2, A3, A4)> BaseType;

  V8cCallbackFunction(V8cGlobalEnvironment* env, v8::Local<v8::Value> function)
    : env_(env), weak_function_(env, function) {
      DCHECK(env_);
      DCHECK(function->IsFunction());
  }

  CallbackResult<R> Run(
      typename base::internal::CallbackParamTraits<A1>::ForwardType a1,
      typename base::internal::CallbackParamTraits<A2>::ForwardType a2,
      typename base::internal::CallbackParamTraits<A3>::ForwardType a3,
      typename base::internal::CallbackParamTraits<A4>::ForwardType a4)
      const override {
    NOTIMPLEMENTED();
    CallbackResult<R> callback_result;
    return callback_result;
  }

  v8::Local<v8::Value> value() const { return weak_function_.GetValue(); }

 private:
  V8cGlobalEnvironment* env_;
  WeakHeapObject weak_function_;
};

template <typename R, typename A1, typename A2, typename A3, typename A4,
    typename A5>
class V8cCallbackFunction<R(A1, A2, A3, A4, A5)>
    : public CallbackFunction<R(A1, A2, A3, A4, A5)> {
 public:
  typedef CallbackFunction<R(A1, A2, A3, A4, A5)> BaseType;

  V8cCallbackFunction(V8cGlobalEnvironment* env, v8::Local<v8::Value> function)
    : env_(env), weak_function_(env, function) {
      DCHECK(env_);
      DCHECK(function->IsFunction());
  }

  CallbackResult<R> Run(
      typename base::internal::CallbackParamTraits<A1>::ForwardType a1,
      typename base::internal::CallbackParamTraits<A2>::ForwardType a2,
      typename base::internal::CallbackParamTraits<A3>::ForwardType a3,
      typename base::internal::CallbackParamTraits<A4>::ForwardType a4,
      typename base::internal::CallbackParamTraits<A5>::ForwardType a5)
      const override {
    NOTIMPLEMENTED();
    CallbackResult<R> callback_result;
    return callback_result;
  }

  v8::Local<v8::Value> value() const { return weak_function_.GetValue(); }

 private:
  V8cGlobalEnvironment* env_;
  WeakHeapObject weak_function_;
};

template <typename R, typename A1, typename A2, typename A3, typename A4,
    typename A5, typename A6>
class V8cCallbackFunction<R(A1, A2, A3, A4, A5, A6)>
    : public CallbackFunction<R(A1, A2, A3, A4, A5, A6)> {
 public:
  typedef CallbackFunction<R(A1, A2, A3, A4, A5, A6)> BaseType;

  V8cCallbackFunction(V8cGlobalEnvironment* env, v8::Local<v8::Value> function)
    : env_(env), weak_function_(env, function) {
      DCHECK(env_);
      DCHECK(function->IsFunction());
  }

  CallbackResult<R> Run(
      typename base::internal::CallbackParamTraits<A1>::ForwardType a1,
      typename base::internal::CallbackParamTraits<A2>::ForwardType a2,
      typename base::internal::CallbackParamTraits<A3>::ForwardType a3,
      typename base::internal::CallbackParamTraits<A4>::ForwardType a4,
      typename base::internal::CallbackParamTraits<A5>::ForwardType a5,
      typename base::internal::CallbackParamTraits<A6>::ForwardType a6)
      const override {
    NOTIMPLEMENTED();
    CallbackResult<R> callback_result;
    return callback_result;
  }

  v8::Local<v8::Value> value() const { return weak_function_.GetValue(); }

 private:
  V8cGlobalEnvironment* env_;
  WeakHeapObject weak_function_;
};

template <typename R, typename A1, typename A2, typename A3, typename A4,
    typename A5, typename A6, typename A7>
class V8cCallbackFunction<R(A1, A2, A3, A4, A5, A6, A7)>
    : public CallbackFunction<R(A1, A2, A3, A4, A5, A6, A7)> {
 public:
  typedef CallbackFunction<R(A1, A2, A3, A4, A5, A6, A7)> BaseType;

  V8cCallbackFunction(V8cGlobalEnvironment* env, v8::Local<v8::Value> function)
    : env_(env), weak_function_(env, function) {
      DCHECK(env_);
      DCHECK(function->IsFunction());
  }

  CallbackResult<R> Run(
      typename base::internal::CallbackParamTraits<A1>::ForwardType a1,
      typename base::internal::CallbackParamTraits<A2>::ForwardType a2,
      typename base::internal::CallbackParamTraits<A3>::ForwardType a3,
      typename base::internal::CallbackParamTraits<A4>::ForwardType a4,
      typename base::internal::CallbackParamTraits<A5>::ForwardType a5,
      typename base::internal::CallbackParamTraits<A6>::ForwardType a6,
      typename base::internal::CallbackParamTraits<A7>::ForwardType a7)
      const override {
    NOTIMPLEMENTED();
    CallbackResult<R> callback_result;
    return callback_result;
  }

  v8::Local<v8::Value> value() const { return weak_function_.GetValue(); }

 private:
  V8cGlobalEnvironment* env_;
  WeakHeapObject weak_function_;
};

template <typename Signature>
struct TypeTraits<CallbackFunction<Signature> > {
  typedef V8cUserObjectHolder<V8cCallbackFunction<Signature> > ConversionType;
  typedef const ScriptValue<CallbackFunction<Signature> >* ReturnType;
};

}  // namespace v8c
}  // namespace script
}  // namespace cobalt

#endif  // COBALT_SCRIPT_V8C_V8C_CALLBACK_FUNCTION_H_
