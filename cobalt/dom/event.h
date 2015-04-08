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

#ifndef DOM_EVENT_H_
#define DOM_EVENT_H_

#include "base/memory/weak_ptr.h"
#include "base/string_piece.h"
#include "cobalt/dom/event_wrapper_creator.h"
#include "cobalt/script/wrappable.h"

namespace cobalt {
namespace dom {

// The forward declaration is necessary for breaking the bi-directional
// dependency between Event and EventTarget.
class EventTarget;

// The Event interface can be passed from the event target to event listener to
// pass information between them.
//   http://www.w3.org/TR/2014/WD-dom-20140710/#interface-event
//
// TODO(b/19335817): We only support the attributes/methods that are in use.
// We need to investigate the exact subset of them required in Cobalt.
class Event : public script::Wrappable {
 public:
  // Web API: Event
  // EventPhase values as defined by Web API Event.eventPhase.
  //
  enum EventPhase { kNone, kCapturingPhase, kAtTarget, kBubblingPhase };

  // Custom, not in any spec.
  //
  enum Bubbles { kNotBubbles, kBubbles };

  enum Cancelable { kNotCancelable, kCancelable };

  // The default ctor creates an event that cannot be bubbled and cancelled.
  explicit Event(const std::string& type);
  Event(const std::string& type, Bubbles bubbles, Cancelable cancelable);
  ~Event() OVERRIDE;

  // Web API: Event
  //
  const std::string& type() const { return type_; }
  const scoped_refptr<EventTarget>& target() const;
  const scoped_refptr<EventTarget>& current_target() const;
  EventPhase event_phase() const { return event_phase_; }

  void StopPropagation() { propagation_stopped_ = true; }
  void StopImmediatePropagation() {
    propagation_stopped_ = true;
    immediate_propagation_stopped_ = true;
  }

  bool bubbles() const { return bubbles_; }
  bool cancelable() const { return cancelable_; }
  void PreventDefault() {
    if (cancelable()) {
      default_prevented_ = true;
    }
  }
  bool default_prevented() const { return default_prevented_; }

  uint64 time_stamp() const { return time_stamp_; }

  void InitEvent(const std::string& type, bool bubbles, bool cancelable);

  // Custom, not in any spec.
  //
  bool IsBeingDispatched() const { return event_phase() != kNone; }
  void set_target(const scoped_refptr<EventTarget>& target);
  void set_current_target(const scoped_refptr<EventTarget>& target);
  void set_event_phase(EventPhase event_phase) { event_phase_ = event_phase; }

  // The event dispatching process usually looks like:
  //
  // for (each event target) {
  //   for (each event listener in the current target) {
  //     listener->handleEvent(event);
  //     if (event->immediate_propagation_stopped()) break;
  //   }
  //   if (event->propagation_stopped()) break;
  // }
  //
  // When propagation_stopped() returns true, the inside loop will continue but
  // the outside loop will break.
  bool propagation_stopped() const { return propagation_stopped_; }
  // When immediate_propagation_stopped() returns true, the inside loop will
  // break. As StopImmediatePropagation() will also set propagation_stopped_ to
  // true. This implies the outside loop will be break too.
  bool immediate_propagation_stopped() const {
    return immediate_propagation_stopped_;
  }

  // Custom, not in any spec.
  //
  virtual scoped_ptr<script::ScriptObjectHandle> CreateWrapper(
      EventWrapperCreator* creator) {
    DCHECK(!get_wrapper_handle() || !get_wrapper_handle()->IsValidHandle());
    return creator->CreateWrapper(this);
  }

 private:
  std::string type_;

  scoped_refptr<EventTarget> target_;
  scoped_refptr<EventTarget> current_target_;
  EventPhase event_phase_;

  bool propagation_stopped_;
  bool immediate_propagation_stopped_;

  bool bubbles_;
  bool cancelable_;
  bool default_prevented_;

  uint64 time_stamp_;
};

}  // namespace dom
}  // namespace cobalt

#endif  // DOM_EVENT_H_
