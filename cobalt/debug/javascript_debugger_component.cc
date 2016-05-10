/*
 * Copyright 2016 Google Inc. All Rights Reserved.
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

#include "cobalt/debug/javascript_debugger_component.h"

#include "base/bind.h"
#include "base/optional.h"
#include "base/values.h"

namespace cobalt {
namespace debug {

namespace {
// Command, parameter and event names as specified by the protocol:
// https://developer.chrome.com/devtools/docs/protocol/1.1/debugger

// Commands.
const char kDisable[] = "Debugger.disable";
const char kEnable[] = "Debugger.enable";
const char kGetScriptSource[] = "Debugger.getScriptSource";
const char kPause[] = "Debugger.pause";
const char kResume[] = "Debugger.resume";
const char kStepInto[] = "Debugger.stepInto";
const char kStepOut[] = "Debugger.stepOut";
const char kStepOver[] = "Debugger.stepOver";

// Parameter names.
const char kCallFrameId[] = "callFrameId";
const char kCallFrames[] = "callFrames";
const char kErrorLine[] = "errorLine";
const char kErrorMessage[] = "errorMessage";
const char kFunctionName[] = "functionName";
const char kLocationColumnNumber[] = "location.columnNumber";
const char kLocationLineNumber[] = "location.lineNumber";
const char kLocationScriptId[] = "location.scriptId";
const char kObject[] = "object";
const char kReason[] = "reason";
const char kScopeChain[] = "scopeChain";
const char kScriptId[] = "scriptId";
const char kStartColumn[] = "startColumn";
const char kStartLine[] = "startLine";
const char kThis[] = "this";
const char kType[] = "type";
const char kUrl[] = "url";

// Parameter values.
const char kDebugCommand[] = "debugCommand";

// Result parameters.
const char kScriptSource[] = "result.scriptSource";

// Events.
const char kPaused[] = "Debugger.paused";
const char kResumed[] = "Debugger.resumed";
const char kScriptFailedToParse[] = "Debugger.scriptFailedToParse";
const char kScriptParsed[] = "Debugger.scriptParsed";
}  // namespace

JavaScriptDebuggerComponent::JavaScriptDebuggerComponent(
    const base::WeakPtr<DebugServer>& server,
    script::GlobalObjectProxy* global_object_proxy)
    : DebugServer::Component(server),
      global_object_proxy_(global_object_proxy),
      source_providers_deleter_(&source_providers_) {
  AddCommand(kDisable, base::Bind(&JavaScriptDebuggerComponent::Disable,
                                  base::Unretained(this)));
  AddCommand(kEnable, base::Bind(&JavaScriptDebuggerComponent::Enable,
                                 base::Unretained(this)));
  AddCommand(kGetScriptSource,
             base::Bind(&JavaScriptDebuggerComponent::GetScriptSource,
                        base::Unretained(this)));
  AddCommand(kPause, base::Bind(&JavaScriptDebuggerComponent::Pause,
                                base::Unretained(this)));
  AddCommand(kResume, base::Bind(&JavaScriptDebuggerComponent::Resume,
                                 base::Unretained(this)));
  AddCommand(kStepInto, base::Bind(&JavaScriptDebuggerComponent::StepInto,
                                   base::Unretained(this)));
  AddCommand(kStepOut, base::Bind(&JavaScriptDebuggerComponent::StepOut,
                                  base::Unretained(this)));
  AddCommand(kStepOver, base::Bind(&JavaScriptDebuggerComponent::StepOver,
                                   base::Unretained(this)));
}

JavaScriptDebuggerComponent::~JavaScriptDebuggerComponent() {}

JSONObject JavaScriptDebuggerComponent::Enable(const JSONObject& params) {
  UNREFERENCED_PARAMETER(params);

  // Reset the debugger first, to detach the current connection, if any.
  script_debugger_.reset(NULL);

  script_debugger_ =
      script::ScriptDebugger::CreateDebugger(global_object_proxy_, this);
  DCHECK(script_debugger_);

  if (script_debugger_) {
    return JSONObject(new base::DictionaryValue());
  } else {
    return ErrorResponse("Cannot create JavaScript debugger.");
  }
}

JSONObject JavaScriptDebuggerComponent::Disable(const JSONObject& params) {
  UNREFERENCED_PARAMETER(params);
  script_debugger_.reset(NULL);
  return JSONObject(new base::DictionaryValue());
}

JSONObject JavaScriptDebuggerComponent::GetScriptSource(
    const JSONObject& params) {
  // Get the scriptId from the parameters.
  std::string script_id;
  bool got_script_id = params->GetString(kScriptId, &script_id);
  if (!got_script_id) {
    return ErrorResponse("No scriptId specified in parameters.");
  }

  // Find the source provider with a matching scriptId.
  SourceProviderMap::iterator it = source_providers_.find(script_id);
  if (it == source_providers_.end()) {
    return ErrorResponse("No script found with specified scriptId.");
  }
  script::SourceProvider* source_provider = it->second;
  DCHECK(source_provider);

  // Build and return the result.
  JSONObject result(new base::DictionaryValue());
  result->SetString(kScriptSource, source_provider->GetScriptSource());
  return result.Pass();
}

JSONObject JavaScriptDebuggerComponent::Pause(const JSONObject& params) {
  UNREFERENCED_PARAMETER(params);
  if (!script_debugger_) {
    return ErrorResponse("JavaScript debugger is not enabled.");
  }

  script_debugger_->Pause();
  return JSONObject(new base::DictionaryValue());
}

JSONObject JavaScriptDebuggerComponent::Resume(const JSONObject& params) {
  UNREFERENCED_PARAMETER(params);
  if (!(script_debugger_ && server())) {
    return ErrorResponse("JavaScript debugger is not enabled.");
  }

  script_debugger_->Resume();
  server()->SetPaused(false);
  return JSONObject(new base::DictionaryValue());
}

JSONObject JavaScriptDebuggerComponent::StepInto(const JSONObject& params) {
  UNREFERENCED_PARAMETER(params);
  if (!(script_debugger_ && server())) {
    return ErrorResponse("JavaScript debugger is not enabled.");
  }

  script_debugger_->StepInto();
  server()->SetPaused(false);
  return JSONObject(new base::DictionaryValue());
}

JSONObject JavaScriptDebuggerComponent::StepOut(const JSONObject& params) {
  UNREFERENCED_PARAMETER(params);
  if (!(script_debugger_ && server())) {
    return ErrorResponse("JavaScript debugger is not enabled.");
  }

  script_debugger_->StepOut();
  server()->SetPaused(false);
  return JSONObject(new base::DictionaryValue());
}

JSONObject JavaScriptDebuggerComponent::StepOver(const JSONObject& params) {
  UNREFERENCED_PARAMETER(params);
  if (!(script_debugger_ && server())) {
    return ErrorResponse("JavaScript debugger is not enabled.");
  }

  script_debugger_->StepOver();
  server()->SetPaused(false);
  return JSONObject(new base::DictionaryValue());
}

void JavaScriptDebuggerComponent::OnScriptDebuggerDetach(
    const std::string& reason) {
  DLOG(INFO) << "JavaScript debugger detached: " << reason;
  NOTIMPLEMENTED();
}

void JavaScriptDebuggerComponent::OnScriptDebuggerPause(
    scoped_ptr<script::CallFrame> call_frame) {
  // Notify the clients we're about to pause.
  SendPausedEvent(call_frame.Pass());

  // Tell the debug server to enter paused state - block this thread.
  if (server()) {
    server()->SetPaused(true);
  }

  // Notify the clients we've resumed.
  SendResumedEvent();
}

void JavaScriptDebuggerComponent::OnScriptFailedToParse(
    scoped_ptr<script::SourceProvider> source_provider) {
  DCHECK(source_provider);
  HandleScriptEvent(kScriptFailedToParse, source_provider.Pass());
}

void JavaScriptDebuggerComponent::OnScriptParsed(
    scoped_ptr<script::SourceProvider> source_provider) {
  DCHECK(source_provider);
  HandleScriptEvent(kScriptParsed, source_provider.Pass());
}

JSONObject JavaScriptDebuggerComponent::CreateCallFrameData(
    const scoped_ptr<script::CallFrame>& call_frame) {
  DCHECK(call_frame);

  // Create the JSON object and add the data for this call frame.
  JSONObject call_frame_data(new base::DictionaryValue());
  call_frame_data->SetString(kCallFrameId, call_frame->GetCallFrameId());
  call_frame_data->SetString(kFunctionName, call_frame->GetFunctionName());

  // Offset the line number according to the start line of the source.
  const std::string script_id = call_frame->GetScriptId();
  int line_number = call_frame->GetLineNumber();
  DCHECK(source_providers_[script_id]);
  base::optional<int> start_line = source_providers_[script_id]->GetStartLine();
  line_number -= start_line.value_or(1);

  // Add the location data.
  call_frame_data->SetString(kLocationScriptId, script_id);
  call_frame_data->SetInteger(kLocationLineNumber, line_number);
  base::optional<int> column_number = call_frame->GetColumnNumber();
  if (column_number) {
    call_frame_data->SetInteger(kLocationColumnNumber, column_number.value());
  }

  // Add the scope chain data.
  JSONList scope_chain_data(CreateScopeChainData(call_frame->GetScopeChain()));
  call_frame_data->Set(kScopeChain, scope_chain_data.release());

  // Add the "this" object data.
  const script::OpaqueHandleHolder* this_object = call_frame->GetThis();
  if (this_object) {
    JSONObject this_object_data(CreateRemoteObject(this_object));
    call_frame_data->Set(kThis, this_object_data.release());
  }

  return call_frame_data.Pass();
}

JSONList JavaScriptDebuggerComponent::CreateCallStackData(
    scoped_ptr<script::CallFrame> call_frame) {
  JSONList call_frame_list(new base::ListValue());

  // Consume the scoped CallFrame objects as we iterate over them.
  while (call_frame) {
    JSONObject call_frame_data(CreateCallFrameData(call_frame));
    DCHECK(call_frame_data);
    call_frame_list->Append(call_frame_data.release());
    scoped_ptr<script::CallFrame> next_call_frame(call_frame->GetCaller());
    call_frame = next_call_frame.Pass();
  }

  return call_frame_list.Pass();
}

JSONObject JavaScriptDebuggerComponent::CreateScopeData(
    const scoped_ptr<script::Scope>& scope) {
  DCHECK(scope);
  const script::OpaqueHandleHolder* scope_object = scope->GetObject();
  JSONObject scope_data(new base::DictionaryValue());
  scope_data->Set(kObject, CreateRemoteObject(scope_object).release());
  scope_data->SetString(kType, script::Scope::TypeToString(scope->GetType()));
  return scope_data.Pass();
}

JSONList JavaScriptDebuggerComponent::CreateScopeChainData(
    scoped_ptr<script::Scope> scope) {
  JSONList scope_chain_list(new base::ListValue());

  // Consume the scoped Scope objects as we iterate over them.
  while (scope) {
    JSONObject scope_data(CreateScopeData(scope));
    DCHECK(scope_data);
    scope_chain_list->Append(scope_data.release());
    scoped_ptr<script::Scope> next_scope(scope->GetNext());
    scope = next_scope.Pass();
  }

  return scope_chain_list.Pass();
}

void JavaScriptDebuggerComponent::HandleScriptEvent(
    const std::string& method,
    scoped_ptr<script::SourceProvider> source_provider) {
  DCHECK(source_provider);

  // Send the event notification to the debugger clients.
  JSONObject params(new base::DictionaryValue());
  params->SetString(kScriptId, source_provider->GetScriptId());
  params->SetString(kUrl, source_provider->GetUrl());
  base::optional<int> error_line = source_provider->GetErrorLine();
  if (error_line) {
    DCHECK_EQ(method, kScriptFailedToParse);
    params->SetInteger(kErrorLine, error_line.value());
  }
  base::optional<std::string> error_message =
      source_provider->GetErrorMessage();
  if (error_message) {
    DCHECK_EQ(method, kScriptFailedToParse);
    params->SetString(kErrorMessage, error_message.value());
  }
  base::optional<int> start_line = source_provider->GetStartLine();
  if (start_line) {
    DCHECK_GE(start_line.value(), 1);
    params->SetInteger(kStartLine, start_line.value());
  }
  base::optional<int> start_column = source_provider->GetStartColumn();
  if (start_column) {
    DCHECK_GE(start_column.value(), 1);
    params->SetInteger(kStartColumn, start_column.value());
  }
  SendEvent(method, params.Pass());

  // Store the raw pointer to the source provider in the map.
  // The values in the map will be deleted on destruction by
  // |source_providers_deleter_|.
  const std::string script_id = source_provider->GetScriptId();
  SourceProviderMap::iterator it = source_providers_.find(script_id);
  if (it != source_providers_.end()) {
    delete it->second;
  }
  source_providers_[script_id] = source_provider.release();
}

void JavaScriptDebuggerComponent::SendPausedEvent(
    scoped_ptr<script::CallFrame> call_frame) {
  std::string event_method = kPaused;
  JSONObject event_params(new base::DictionaryValue());
  JSONList call_stack_data(CreateCallStackData(call_frame.Pass()));
  DCHECK(call_stack_data);
  event_params->Set(kCallFrames, call_stack_data.release());
  event_params->SetString(kReason, kDebugCommand);
  SendEvent(event_method, event_params);
}

void JavaScriptDebuggerComponent::SendResumedEvent() {
  // Send the event to the clients. No parameters.
  std::string event_method = kResumed;
  scoped_ptr<base::DictionaryValue> event_params(new base::DictionaryValue());
  SendEvent(event_method, event_params);
}

}  // namespace debug
}  // namespace cobalt
