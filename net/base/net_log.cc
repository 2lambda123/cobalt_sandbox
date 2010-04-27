// Copyright (c) 2010 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "net/base/net_log.h"
#include "base/logging.h"
#include "base/string_util.h"
#include "base/values.h"

namespace net {

// static
const char* NetLog::EventTypeToString(EventType event) {
  switch (event) {
#define EVENT_TYPE(label) case TYPE_ ## label: return #label;
#include "net/base/net_log_event_type_list.h"
#undef EVENT_TYPE
  }
  return NULL;
}

// static
std::vector<NetLog::EventType> NetLog::GetAllEventTypes() {
  std::vector<NetLog::EventType> types;
#define EVENT_TYPE(label) types.push_back(TYPE_ ## label);
#include "net/base/net_log_event_type_list.h"
#undef EVENT_TYPE
  return types;
}

void BoundNetLog::AddEntry(NetLog::EventType type,
                           NetLog::EventPhase phase,
                           NetLog::EventParameters* extra_parameters) const {
  if (net_log_) {
    net_log_->AddEntry(type, base::TimeTicks::Now(), source_, phase,
                       extra_parameters);
  }
}

void BoundNetLog::AddEntryWithTime(
    NetLog::EventType type,
    const base::TimeTicks& time,
    NetLog::EventPhase phase,
    NetLog::EventParameters* extra_parameters) const {
  if (net_log_) {
    net_log_->AddEntry(type, time, source_, phase, extra_parameters);
  }
}

bool BoundNetLog::HasListener() const {
  if (net_log_)
    return net_log_->HasListener();
  return false;
}

void BoundNetLog::AddEvent(NetLog::EventType event_type) const {
  AddEventWithParameters(event_type, NULL);
}

void BoundNetLog::AddEventWithParameters(
    NetLog::EventType event_type,
    NetLog::EventParameters* params) const {
  AddEntry(event_type, NetLog::PHASE_NONE, params);
}

void BoundNetLog::AddEventWithInteger(NetLog::EventType event_type,
                                      const char* name,
                                      int value) const {
  scoped_refptr<NetLog::EventParameters> params =
      new NetLogIntegerParameter(name, value);
  AddEventWithParameters(event_type, params);
}

void BoundNetLog::AddEventWithString(NetLog::EventType event_type,
                                     const char* name,
                                     const std::string& value) const {
  scoped_refptr<NetLog::EventParameters> params =
      new NetLogStringParameter(name, value);
  AddEventWithParameters(event_type, params);
}

void BoundNetLog::BeginEvent(NetLog::EventType event_type) const {
  BeginEventWithParameters(event_type, NULL);
}

void BoundNetLog::BeginEventWithParameters(
    NetLog::EventType event_type,
    NetLog::EventParameters* params) const {
  AddEntry(event_type, NetLog::PHASE_BEGIN, params);
}

void BoundNetLog::BeginEventWithString(NetLog::EventType event_type,
                                       const char* name,
                                       const std::string& value) const {
  scoped_refptr<NetLog::EventParameters> params =
      new NetLogStringParameter(name, value);
  BeginEventWithParameters(event_type, params);
}

void BoundNetLog::BeginEventWithInteger(NetLog::EventType event_type,
                                        const char* name,
                                        int value) const {
  scoped_refptr<NetLog::EventParameters> params =
      new NetLogIntegerParameter(name, value);
  BeginEventWithParameters(event_type, params);
}

void BoundNetLog::EndEvent(NetLog::EventType event_type) const {
  EndEventWithParameters(event_type, NULL);
}

void BoundNetLog::EndEventWithParameters(
    NetLog::EventType event_type,
    NetLog::EventParameters* params) const {
  AddEntry(event_type, NetLog::PHASE_END, params);
}

void BoundNetLog::EndEventWithInteger(NetLog::EventType event_type,
                                      const char* name,
                                      int value) const {
  scoped_refptr<NetLog::EventParameters> params =
      new NetLogIntegerParameter(name, value);
  EndEventWithParameters(event_type, params);
}

// static
BoundNetLog BoundNetLog::Make(NetLog* net_log,
                              NetLog::SourceType source_type) {
  if (!net_log)
    return BoundNetLog();

  NetLog::Source source(source_type, net_log->NextID());
  return BoundNetLog(source, net_log);
}

NetLogStringParameter::NetLogStringParameter(const char* name,
                                             const std::string& value)
    : name_(name), value_(value) {
}

Value* NetLogIntegerParameter::ToValue() const {
  DictionaryValue* dict = new DictionaryValue();
  dict->SetInteger(ASCIIToWide(name_), value_);
  return dict;
}

Value* NetLogStringParameter::ToValue() const {
  DictionaryValue* dict = new DictionaryValue();
  dict->SetString(ASCIIToWide(name_), value_);
  return dict;
}

void CapturingNetLog::AddEntry(EventType type,
                               const base::TimeTicks& time,
                               const Source& source,
                               EventPhase phase,
                               EventParameters* extra_parameters) {
  Entry entry(type, time, source, phase, extra_parameters);
  if (entries_.size() + 1 < max_num_entries_)
    entries_.push_back(entry);
}

uint32 CapturingNetLog::NextID() {
  return next_id_++;
}

void CapturingNetLog::Clear() {
  entries_.clear();
}

void CapturingBoundNetLog::Clear() {
  capturing_net_log_->Clear();
}

void CapturingBoundNetLog::AppendTo(const BoundNetLog& net_log) const {
  for (size_t i = 0; i < entries().size(); ++i) {
    const CapturingNetLog::Entry& entry = entries()[i];
    net_log.AddEntryWithTime(entry.type, entry.time, entry.phase,
                             entry.extra_parameters);
  }
}

}  // namespace net
