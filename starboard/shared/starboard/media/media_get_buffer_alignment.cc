// Copyright 2018 The Cobalt Authors. All Rights Reserved.
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

#include "starboard/media.h"

#include "starboard/log.h"

#if SB_API_VERSION >= 10
int SbMediaGetBufferAlignment(SbMediaType type) {
  SB_UNREFERENCED_PARAMETER(type);
#if defined(COBALT_MEDIA_BUFFER_ALIGNMENT)
  SB_DLOG(WARNING) << "COBALT_MEDIA_BUFFER_ALIGNMENT will be deprecated in a "
                      "future Starboard version.";
  // Use define forwarded from GYP variable.
  return COBALT_MEDIA_BUFFER_ALIGNMENT;
#else   // defined(COBALT_MEDIA_BUFFER_ALIGNMENT
  return 1 * 1024 * 1024;
#endif  // defined(COBALT_MEDIA_BUFFER_ALIGNMENT
}
#endif  // SB_API_VERSION >= 10
