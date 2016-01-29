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

#ifndef COBALT_LOADER_IMAGE_JPEG_IMAGE_DECODER_H_
#define COBALT_LOADER_IMAGE_JPEG_IMAGE_DECODER_H_

#include <setjmp.h>
#include <string>

#include "base/callback.h"
#include "base/memory/scoped_ptr.h"
#include "cobalt/loader/image/image_data_decoder.h"

// Inhibit C++ name-mangling for libjpeg functions.
extern "C" {
#include "third_party/libjpeg/jpeglib.h"
}

namespace cobalt {
namespace loader {
namespace image {

class JPEGImageDecoder : public ImageDataDecoder {
 public:
  explicit JPEGImageDecoder(render_tree::ResourceProvider* resource_provider);
  ~JPEGImageDecoder() OVERRIDE;

  // From ImageDataDecoder
  std::string GetTypeString() const OVERRIDE { return "JPEGImageDecoder"; }

  // Returns true if the signature is valid for the particular image type.
  static bool IsValidSignature(const uint8* header) {
    return !memcmp(header, "\xFF\xD8\xFF", 3);
  }

 private:
  // From ImageDataDecoder
  size_t DecodeChunkInternal(const uint8* data, size_t size) OVERRIDE;

  bool ReadHeader();
  bool StartDecompress();
  bool DecodeProgressiveJPEG();
  bool ReadLines();

  jpeg_decompress_struct info_;
  jpeg_source_mgr source_manager_;
  jpeg_error_mgr error_manager_;
};

}  // namespace image
}  // namespace loader
}  // namespace cobalt

#endif  // COBALT_LOADER_IMAGE_JPEG_IMAGE_DECODER_H_
