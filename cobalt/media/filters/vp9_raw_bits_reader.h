// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef COBALT_MEDIA_FILTERS_VP9_RAW_BITS_READER_H_
#define COBALT_MEDIA_FILTERS_VP9_RAW_BITS_READER_H_

#include <memory>

#include "base/basictypes.h"
#include "cobalt/media/base/media_export.h"
#include "starboard/types.h"

namespace cobalt {
namespace media {

class BitReader;

// A class to read raw bits stream. See VP9 spec, "RAW-BITS DECODING" section
// for detail.
class MEDIA_EXPORT Vp9RawBitsReader {
 public:
  Vp9RawBitsReader();
  ~Vp9RawBitsReader();

  // |data| is the input buffer with |size| bytes.
  void Initialize(const uint8_t* data, size_t size);

  // Returns true if none of the reads since the last Initialize() call has
  // gone beyond the end of available data.
  bool IsValid() const { return valid_; }

  // Returns how many bytes were read since the last Initialize() call.
  // Partial bytes will be counted as one byte. For example, it will return 1
  // if 3 bits were read.
  size_t GetBytesRead() const;

  // Reads one bit.
  // If the read goes beyond the end of buffer, the return value is undefined.
  bool ReadBool();

  // Reads a literal with |bits| bits.
  // If the read goes beyond the end of buffer, the return value is undefined.
  int ReadLiteral(int bits);

  // Reads a signed literal with |bits| bits (not including the sign bit).
  // If the read goes beyond the end of buffer, the return value is undefined.
  int ReadSignedLiteral(int bits);

  // Consumes trailing bits up to next byte boundary. Returns true if no
  // trailing bits or they are all zero.
  bool ConsumeTrailingBits();

 private:
  std::unique_ptr<BitReader> reader_;

  // Indicates if none of the reads since the last Initialize() call has gone
  // beyond the end of available data.
  bool valid_;

  DISALLOW_COPY_AND_ASSIGN(Vp9RawBitsReader);
};

}  // namespace media
}  // namespace cobalt

#endif  // COBALT_MEDIA_FILTERS_VP9_RAW_BITS_READER_H_
