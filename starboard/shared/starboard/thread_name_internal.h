// Copyright 2015 The Cobalt Authors. All Rights Reserved.
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

#ifndef STARBOARD_SHARED_STARBOARD_THREAD_NAME_INTERNAL_H_
#define STARBOARD_SHARED_STARBOARD_THREAD_NAME_INTERNAL_H_

#include "starboard/shared/internal_only.h"

namespace starboard {
namespace shared {

// The maximum size of the string returned by GetThreadName().
static const int kMaxThreadName = 256;

char* GetThreadName();

}  // namespace shared
}  // namespace starboard

#endif  // STARBOARD_SHARED_STARBOARD_THREAD_NAME_INTERNAL_H_
