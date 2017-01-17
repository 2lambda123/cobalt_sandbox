// Copyright 2016 Google Inc. All Rights Reserved.
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

#include "starboard/decode_target.h"

#if SB_API_VERSION < SB_EXPERIMENTAL_API_VERSION
SbDecodeTarget SbDecodeTargetCreate(EGLDisplay /*display*/,
                                    EGLContext /*context*/,
                                    SbDecodeTargetFormat /*format*/,
                                    GLuint* /*planes*/) {
  return kSbDecodeTargetInvalid;
}
#else  // SB_API_VERSION < SB_EXPERIMENTAL_API_VERSION
SbDecodeTarget SbDecodeTargetCreate(void* /*display*/,
                                    void* /*context*/,
                                    SbDecodeTargetFormat /*format*/,
                                    int /*width*/,
                                    int /*height*/,
                                    uint32_t* /*planes*/) {
  return kSbDecodeTargetInvalid;
}
#endif  // SB_API_VERSION < SB_EXPERIMENTAL_API_VERSION
