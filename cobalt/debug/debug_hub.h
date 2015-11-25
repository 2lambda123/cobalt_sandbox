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

#ifndef DEBUG_DEBUG_HUB_H_
#define DEBUG_DEBUG_HUB_H_

#if defined(ENABLE_DEBUG_CONSOLE)

#include <list>
#include <map>
#include <string>

#include "base/callback.h"
#include "base/message_loop.h"
#include "base/stl_util.h"
#include "base/synchronization/lock.h"
#include "cobalt/base/log_message_handler.h"
#include "cobalt/base/source_location.h"
#include "cobalt/debug/debugger.h"
#include "cobalt/debug/system_stats_tracker.h"
#include "cobalt/script/callback_function.h"
#include "cobalt/script/debug_server.h"
#include "cobalt/script/script_object.h"
#include "cobalt/script/wrappable.h"

namespace cobalt {
namespace debug {

// This class implements an interface to JavaScript for debugging.
// The public methods of this class are expected to be exposed in DebugHub.idl.
// The main (and typically only) JavaScript client is the debug console.
// A stub implementation is used if ENABLE_DEBUG_CONSOLE is not defined.
//
// Log messages generated by the app via LOG are sent to the client
// via JavaScript callback(s) registered with AddLogMessageCallback.
// This typically includes console output from any web module.
//
// The JavaScript client can examine the available list of registered console
// values.
//
// The JavaScript client can execute JavaScript commands in the main web module
// by calling the ExecuteCommand method.
//
// The JavaScript client can send arbitrary messages to an object of this class
// on a specific named channel using the SendCommand method. These messages are
// forwarded to any component of the app that registers a message handler for
// that channel by creating an instance of a class derived from CommandHandler.

class DebugHub : public script::Wrappable {
 public:
  // Type for log message callback on JS side.
  typedef script::CallbackFunction<
      void(int severity, const std::string& file, int line,
           size_t message_start, const std::string& msg)> LogMessageCallback;
  typedef script::ScriptObject<LogMessageCallback> LogMessageCallbackArg;

  // Javascript command execution callback type.
  typedef base::Callback<std::string(
      const std::string& script_utf8,
      const base::SourceLocation& script_location)> ExecuteJavascriptCallback;

  // Type for stored callback info.
  // We store the message loop from which the callback was registered,
  // so we can run the callback on the same loop.
  struct LogMessageCallbackInfo {
    LogMessageCallbackInfo(const DebugHub* const debug_hub,
                           const LogMessageCallbackArg& cb,
                           const scoped_refptr<base::MessageLoopProxy>& proxy)
        : callback(debug_hub, cb), message_loop_proxy(proxy) {}
    LogMessageCallbackArg::Reference callback;
    scoped_refptr<base::MessageLoopProxy> message_loop_proxy;
  };

  // Debug console modes.
  static const int kDebugConsoleOff = 0;
  static const int kDebugConsoleHud = 1;
  static const int kDebugConsoleOn = 2;
  static const int kDebugConsoleNumModes = kDebugConsoleOn + 1;

  // Declare logging levels here for export to JS
  static const int kLogInfo = logging::LOG_INFO;
  static const int kLogWarning = logging::LOG_WARNING;
  static const int kLogError = logging::LOG_ERROR;
  static const int kLogErrorReport = logging::LOG_ERROR_REPORT;
  static const int kLogFatal = logging::LOG_FATAL;

  DebugHub(
      const ExecuteJavascriptCallback& execute_javascript_callback,
      const Debugger::CreateDebugServerCallback& create_debug_server_callback);
  ~DebugHub();

  // Gets the JavaScript debugger client interface.
  const scoped_refptr<Debugger>& debugger() { return debugger_; }

  // Called from JS to register a log message callback.
  // Returns an id that can be used to remove the callback.
  int AddLogMessageCallback(const LogMessageCallbackArg& callback);

  // Called from JS to remove a callback using the id returned by
  // AddLogMessageCallback.
  void RemoveLogMessageCallback(int id);

  // Removes all registered callbacks connected to this DebugHub.
  void RemoveAllLogMessageCallbacks();

  // Gets the collection of available CVal names as an alphabetically ordered,
  // space-separated list.
  std::string GetConsoleValueNames() const;

  // Gets the value of a named CVal as a pretty string.
  std::string GetConsoleValue(const std::string& name) const;

  int GetDebugConsoleMode() const;
  void SetDebugConsoleMode(int debug_console_mode);
  int CycleDebugConsoleMode();

  // Convenience methods to get/set debug console mode as string.
  // This supports command line options and persistent local storage.
  std::string GetDebugConsoleModeAsString() const;
  void SetDebugConsoleModeAsString(const std::string& mode_string);

  // Executes JavaScript in the main web module.
  std::string ExecuteJavascript(const std::string& javascript);

  // Gets the collection of registered command channels as an alphabetically
  // ordered, space-separated list.
  std::string GetCommandChannels() const;

  // Gets the help strings for a specified command channel.
  std::string GetCommandChannelShortHelp(const std::string& channel) const;
  std::string GetCommandChannelLongHelp(const std::string& channel) const;

  // Sends a command to be handled by registered handers.
  // This lets the JavaScript debug console trigger actions in the app.
  void SendCommand(const std::string& channel, const std::string& message);

  DEFINE_WRAPPABLE_TYPE(DebugHub);

 private:
  // Called by LogMessageHandler for each log message.
  bool OnLogMessage(int severity, const char* file, int line,
                    size_t message_start, const std::string& str);
  // Called by OnLogMessage. This shall be run on the same message loop as
  // the one on which the LogMessageCallback was registered.
  void LogMessageTo(int id, int severity, const char* file, int line,
                    size_t message_start, const std::string& str);

  typedef std::map<int, LogMessageCallbackInfo*> LogMessageCallbacks;

  // The callback to run to execute some JS.
  const ExecuteJavascriptCallback execute_javascript_callback_;

  int next_log_message_callback_id_;
  LogMessageCallbacks log_message_callbacks_;
  STLValueDeleter<LogMessageCallbacks> log_message_callbacks_deleter_;
  base::Lock lock_;
  base::LogMessageHandler::CallbackId log_message_handler_callback_id_;

  // Maintains a collection of CVals continuously updated with system stats.
  SystemStatsTracker system_stats_tracker;

  // The current debug console mode
  int debug_console_mode_;

  // Interface to the JavaScript debugger client.
  scoped_refptr<Debugger> debugger_;
};

}  // namespace debug
}  // namespace cobalt

#endif  // ENABLE_DEBUG_CONSOLE
#endif  // DEBUG_DEBUG_HUB_H_
