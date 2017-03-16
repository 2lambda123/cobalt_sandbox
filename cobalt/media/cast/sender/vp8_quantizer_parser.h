// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef COBALT_MEDIA_CAST_SENDER_VP8_QUANTIZER_PARSER_H_
#define COBALT_MEDIA_CAST_SENDER_VP8_QUANTIZER_PARSER_H_

#include "media/cast/cast_config.h"
#include "starboard/types.h"

namespace cobalt {
namespace media {
namespace cast {

// Partially parse / skip data in the header and the first partition,
// and return the base quantizer in the range [0,63], or -1 on parse error.
int ParseVp8HeaderQuantizer(const uint8_t* data, size_t size);

}  // namespace cast
}  // namespace media
}  // namespace cobalt

#endif  // COBALT_MEDIA_CAST_SENDER_VP8_QUANTIZER_PARSER_H_
