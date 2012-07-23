// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_BASE_NET_LOG_H_
#define NET_BASE_NET_LOG_H_

#include <string>

#include "base/basictypes.h"
#include "base/callback_forward.h"
#include "base/compiler_specific.h"
#include "base/string16.h"
#include "net/base/net_export.h"

namespace base {
class DictionaryValue;
class TimeTicks;
class Value;
}

namespace net {

// NetLog is the destination for log messages generated by the network stack.
// Each log message has a "source" field which identifies the specific entity
// that generated the message (for example, which URLRequest or which
// SocketStream).
//
// To avoid needing to pass in the "source ID" to the logging functions, NetLog
// is usually accessed through a BoundNetLog, which will always pass in a
// specific source ID.
//
// All NetLog methods must be thread-safe.
//
// For a broader introduction see the design document:
// https://sites.google.com/a/chromium.org/dev/developers/design-documents/network-stack/netlog
class NET_EXPORT NetLog {
 public:
  enum EventType {
#define EVENT_TYPE(label) TYPE_ ## label,
#include "net/base/net_log_event_type_list.h"
#undef EVENT_TYPE
    EVENT_COUNT
  };

  // The 'phase' of an event trace (whether it marks the beginning or end
  // of an event.).
  enum EventPhase {
    PHASE_NONE,
    PHASE_BEGIN,
    PHASE_END,
  };

  // The "source" identifies the entity that generated the log message.
  enum SourceType {
#define SOURCE_TYPE(label) SOURCE_ ## label,
#include "net/base/net_log_source_type_list.h"
#undef SOURCE_TYPE
    SOURCE_COUNT
  };

  // Specifies the granularity of events that should be emitted to the log.
  enum LogLevel {
    // Log everything possible, even if it is slow and memory expensive.
    // Includes logging of transferred bytes.
    LOG_ALL,

    // Log all events, but do not include the actual transferred bytes as
    // parameters for bytes sent/received events.
    LOG_ALL_BUT_BYTES,

    // Only log events which are cheap, and don't consume much memory.
    LOG_BASIC,
  };

  // A callback function that return a Value representation of the parameters
  // associated with an event.  If called, it will be called synchonously,
  // so it need not have owning references.  May be called more than once, or
  // not at all.  May return NULL.
  typedef base::Callback<base::Value*(LogLevel)> ParametersCallback;

  // Identifies the entity that generated this log. The |id| field should
  // uniquely identify the source, and is used by log observers to infer
  // message groupings. Can use NetLog::NextID() to create unique IDs.
  struct NET_EXPORT Source {
    static const uint32 kInvalidId = 0;

    Source() : type(SOURCE_NONE), id(kInvalidId) {}
    Source(SourceType type, uint32 id) : type(type), id(id) {}
    bool is_valid() const { return id != kInvalidId; }

    // Adds the source to a DictionaryValue containing event parameters,
    // using the name "source_dependency".
    void AddToEventParameters(base::DictionaryValue* event_params) const;

    // Returns a callback that returns a dictionary with a single entry
    // named "source_dependecy" that describes |this|.
    ParametersCallback ToEventParametersCallback() const;

    // Attempts to extract a Source from a set of event parameters.  Returns
    // true and writes the result to |source| on success.  Returns false and
    // makes |source| an invalid source on failure.
    // TODO(mmenke):  Long term, we want to remove this.
    static bool FromEventParameters(base::Value* event_params, Source* source);

    SourceType type;
    uint32 id;
  };

  class NET_EXPORT Entry {
   public:
    Entry(EventType type,
          Source source,
          EventPhase phase,
          const ParametersCallback* parameters_callback,
          LogLevel log_level);
    ~Entry();

    EventType type() const { return type_; }
    Source source() const { return source_; }
    EventPhase phase() const { return phase_; }

    // Serializes the specified event to a Value.  The Value also includes the
    // current time.  Caller takes ownership of returned Value.
    base::Value* ToValue() const;

    // Returns the parameters as a Value.  Returns NULL if there are no
    // parameters.  Caller takes ownership of returned Value.
    base::Value* ParametersToValue() const;

   private:
    const EventType type_;
    const Source source_;
    const EventPhase phase_;
    const ParametersCallback* parameters_callback_;

    // Log level when the event occurred.
    const LogLevel log_level_;

    // It is not safe to copy this class, since |parameters_callback_| may
    // include pointers that become stale immediately after the event is added,
    // even if the code were modified to keep its own copy of the callback.
    DISALLOW_COPY_AND_ASSIGN(Entry);
  };

  // An observer, that must ensure its own thread safety, for events
  // being added to a NetLog.
  class NET_EXPORT ThreadSafeObserver {
   public:
    // Constructs an observer that wants to see network events, with
    // the specified minimum event granularity.  A ThreadSafeObserver can only
    // observe a single NetLog at a time.
    //
    // Observers will be called on the same thread an entry is added on,
    // and are responsible for ensuring their own thread safety.
    //
    // Observers must stop watching a NetLog before either the Observer or the
    // NetLog is destroyed.
    ThreadSafeObserver();

    // Returns the minimum log level for events this observer wants to
    // receive.  Must not be called when not watching a NetLog.
    LogLevel log_level() const;

    // Returns the NetLog we are currently watching, if any.  Returns NULL
    // otherwise.
    NetLog* net_log() const;

    // This method will be called on the thread that the event occurs on.  It
    // is the responsibility of the observer to handle it in a thread safe
    // manner.
    //
    // It is illegal for an Observer to call any NetLog or
    // NetLog::Observer functions in response to a call to OnAddEntry.
    virtual void OnAddEntry(const Entry& entry) = 0;

   protected:
    virtual ~ThreadSafeObserver();

   private:
    friend class NetLog;

    // Both of these values are only modified by the NetLog.
    LogLevel log_level_;
    NetLog* net_log_;

    DISALLOW_COPY_AND_ASSIGN(ThreadSafeObserver);
  };

  NetLog() {}
  virtual ~NetLog() {}

  // Emits a global event to the log stream, with its own unique source ID.
  void AddGlobalEntry(EventType type);
  void AddGlobalEntry(EventType type,
                      const NetLog::ParametersCallback& parameters_callback);

  // Returns a unique ID which can be used as a source ID.  All returned IDs
  // will be unique and greater than 0.
  virtual uint32 NextID() = 0;

  // Returns the logging level for this NetLog. This is used to avoid computing
  // and saving expensive log entries.
  virtual LogLevel GetLogLevel() const = 0;

  // Adds an observer and sets its log level.  The observer must not be
  // watching any NetLog, including this one, when this is called.
  //
  // Typical observers should specify LOG_BASIC.
  //
  // Observers that need to see the full granularity of events can specify
  // LOG_ALL_BUT_BYTES. However, doing so will have performance consequences.
  //
  // NetLog implementations must call NetLog::OnAddObserver to update the
  // observer's internal state.
  virtual void AddThreadSafeObserver(ThreadSafeObserver* observer,
                                     LogLevel log_level) = 0;

  // Sets the log level of |observer| to |log_level|.  |observer| must be
  // watching |this|.  NetLog implementations must call
  // NetLog::OnSetObserverLogLevel to update the observer's internal state.
  virtual void SetObserverLogLevel(ThreadSafeObserver* observer,
                                   LogLevel log_level) = 0;

  // Removes an observer.  NetLog implementations must call
  // NetLog::OnAddObserver to update the observer's internal state.
  //
  // For thread safety reasons, it is recommended that this not be called in
  // an object's destructor.
  virtual void RemoveThreadSafeObserver(ThreadSafeObserver* observer) = 0;

  // Converts a time to the string format that the NetLog uses to represent
  // times.  Strings are used since integers may overflow.
  static std::string TickCountToString(const base::TimeTicks& time);

  // Returns a C-String symbolic name for |event_type|.
  static const char* EventTypeToString(EventType event_type);

  // Returns a dictionary that maps event type symbolic names to their enum
  // values.  Caller takes ownership of the returned Value.
  static base::Value* GetEventTypesAsValue();

  // Returns a C-String symbolic name for |source_type|.
  static const char* SourceTypeToString(SourceType source_type);

  // Returns a dictionary that maps source type symbolic names to their enum
  // values.  Caller takes ownership of the returned Value.
  static base::Value* GetSourceTypesAsValue();

  // Returns a C-String symbolic name for |event_phase|.
  static const char* EventPhaseToString(EventPhase event_phase);

  // Returns true if |log_level| indicates the actual bytes transferred should
  // be logged.  This is only the case when |log_level| is LOG_ALL.
  static bool IsLoggingBytes(LogLevel log_level);

  // Returns true if |log_level| indicates that all events should be logged,
  // including frequently occuring ones that may impact performances.
  // This is the case when |log_level| is LOG_ALL or LOG_ALL_BUT_BYTES.
  static bool IsLoggingAllEvents(LogLevel log_level);

  // Creates a ParametersCallback that encapsulates a single integer.
  // Warning: |name| must remain valid for the life of the callback.
  // TODO(mmenke):  Rename this to be consistent with Int64Callback.
  static ParametersCallback IntegerCallback(const char* name, int value);

  // Creates a ParametersCallback that encapsulates a single int64.  The
  // callback will return the value as a StringValue, since IntegerValues
  // only support 32-bit values.
  // Warning: |name| must remain valid for the life of the callback.
  static ParametersCallback Int64Callback(const char* name, int64 value);

  // Creates a ParametersCallback that encapsulates a single UTF8 string.  Takes
  // |value| as a pointer to avoid copying, and emphasize it must be valid for
  // the life of the callback.  |value| may not be NULL.
  // Warning: |name| and |value| must remain valid for the life of the callback.
  static ParametersCallback StringCallback(const char* name,
                                           const std::string* value);

  // Same as above, but takes in a UTF16 string.
  static ParametersCallback StringCallback(const char* name,
                                           const string16* value);

 protected:
  // Child classes should respond to the new entry here.  This includes
  // creating the Entry object and alerting their observers.
  virtual void OnAddEntry(const Entry& entry) = 0;

  // Subclasses must call these in the corresponding functions to set an
  // observer's |net_log_| and |log_level_| values.
  void OnAddObserver(ThreadSafeObserver* observer, LogLevel log_level);
  void OnSetObserverLogLevel(ThreadSafeObserver* observer,
                             LogLevel log_level);
  void OnRemoveObserver(ThreadSafeObserver* observer);

 private:
  friend class BoundNetLog;

  void AddEntry(EventType type,
                const Source& source,
                EventPhase phase,
                const NetLog::ParametersCallback* parameters_callback);

  DISALLOW_COPY_AND_ASSIGN(NetLog);
};

// Helper that binds a Source to a NetLog, and exposes convenience methods to
// output log messages without needing to pass in the source.
class NET_EXPORT BoundNetLog {
 public:
  BoundNetLog() : net_log_(NULL) {}

  // Add a log entry to the NetLog for the bound source.
  void AddEntry(NetLog::EventType type, NetLog::EventPhase phase) const;
  void AddEntry(NetLog::EventType type,
                NetLog::EventPhase phase,
                const NetLog::ParametersCallback& get_parameters) const;

  // Convenience methods that call AddEntry with a fixed "capture phase"
  // (begin, end, or none).
  void BeginEvent(NetLog::EventType type) const;
  void BeginEvent(NetLog::EventType type,
                  const NetLog::ParametersCallback& get_parameters) const;

  void EndEvent(NetLog::EventType type) const;
  void EndEvent(NetLog::EventType type,
                const NetLog::ParametersCallback& get_parameters) const;

  void AddEvent(NetLog::EventType type) const;
  void AddEvent(NetLog::EventType type,
                const NetLog::ParametersCallback& get_parameters) const;

  // Just like AddEvent, except |net_error| is a net error code.  A parameter
  // called "net_error" with the indicated value will be recorded for the event.
  // |net_error| must be negative, and not ERR_IO_PENDING, as it's not a true
  // error.
  void AddEventWithNetErrorCode(NetLog::EventType event_type,
                                int net_error) const;

  // Just like EndEvent, except |net_error| is a net error code.  If it's
  // negative, a parameter called "net_error" with a value of |net_error| is
  // associated with the event.  Otherwise, the end event has no parameters.
  // |net_error| must not be ERR_IO_PENDING, as it's not a true error.
  void EndEventWithNetErrorCode(NetLog::EventType event_type,
                                int net_error) const;

  // Logs a byte transfer event to the NetLog.  Determines whether to log the
  // received bytes or not based on the current logging level.
  void AddByteTransferEvent(NetLog::EventType event_type,
                            int byte_count, const char* bytes) const;

  NetLog::LogLevel GetLogLevel() const;

  // Shortcut for NetLog::IsLoggingBytes(this->GetLogLevel()).
  bool IsLoggingBytes() const;

  // Shortcut for NetLog::IsLoggingAllEvents(this->GetLogLevel()).
  bool IsLoggingAllEvents() const;

  // Helper to create a BoundNetLog given a NetLog and a SourceType. Takes care
  // of creating a unique source ID, and handles the case of NULL net_log.
  static BoundNetLog Make(NetLog* net_log, NetLog::SourceType source_type);

  const NetLog::Source& source() const { return source_; }
  NetLog* net_log() const { return net_log_; }

 private:
  BoundNetLog(const NetLog::Source& source, NetLog* net_log)
      : source_(source), net_log_(net_log) {
  }

  NetLog::Source source_;
  NetLog* net_log_;
};

}  // namespace net

#endif  // NET_BASE_NET_LOG_H_
