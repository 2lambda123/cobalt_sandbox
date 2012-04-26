// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

// This is a glue file, which allows third party code to call into our profiler
// without having to include most any functions from base.

#ifndef BASE_PROFILER_ALTERNATE_TIMER_H_
#define BASE_PROFILER_ALTERNATE_TIMER_H_

#include "base/base_export.h"

namespace tracked_objects {

// Provide type for an alternate timer function.
typedef unsigned int NowFunction();

// Environment variable name that is used to activate alternate timer profiling
// (such as using TCMalloc allocations to provide a pseudo-timer) for tasks
// instead of wall clock profiling.
extern const char kAlternateProfilerTime[];

// Set an alternate timer function to replace the OS time function when
// profiling.  Typically this is called by an allocator that is providing a
// function that indicates how much memory has been allocated on any given
// thread.
void SetAlternateTimeSource(NowFunction* now_function);

// Gets the pointer to a function that was set via SetAlternateTimeSource().
// Returns NULL if no set was done prior to calling GetAlternateTimeSource.
BASE_EXPORT NowFunction* GetAlternateTimeSource();

}  // namespace tracked_objects

#endif  // BASE_PROFILER_ALTERNATE_TIMER_H_
