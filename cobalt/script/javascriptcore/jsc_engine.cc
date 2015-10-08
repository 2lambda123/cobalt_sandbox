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
#include "cobalt/script/javascriptcore/jsc_engine.h"

#include "base/logging.h"
#include "base/threading/thread_checker.h"
#include "cobalt/script/javascriptcore/jsc_global_object.h"
#include "cobalt/script/javascriptcore/jsc_global_object_proxy.h"
#include "third_party/WebKit/Source/JavaScriptCore/runtime/InitializeThreading.h"

namespace cobalt {
namespace script {
namespace javascriptcore {

JSCEngine::JSCEngine() {
  global_data_ = JSC::JSGlobalData::create(JSC::LargeHeap);
}

scoped_refptr<GlobalObjectProxy> JSCEngine::CreateGlobalObjectProxy() {
  return new JSCGlobalObjectProxy(global_data_.get(), &script_object_registry_);
}

void JSCEngine::CollectGarbage() {
  DCHECK(thread_checker_.CalledOnValidThread());
  JSC::JSLockHolder lock(global_data_.get());
  DCHECK(global_data_->heap.isSafeToCollect());
  global_data_->heap.collectAllGarbage();
}

void JSCEngine::ReportExtraMemoryCost(size_t bytes) {
  JSC::JSLockHolder lock(global_data_.get());
  global_data_->heap.reportExtraMemoryCost(bytes);
}

}  // namespace javascriptcore

scoped_ptr<JavaScriptEngine> JavaScriptEngine::CreateEngine() {
  // This must be called once on any thread that will run JavaScriptCore. There
  // must be at most one JSGlobalData instance created per thread.
  JSC::initializeThreading();
  scoped_ptr<JavaScriptEngine> engine(new javascriptcore::JSCEngine());
  return engine.Pass();
}

}  // namespace script
}  // namespace cobalt
