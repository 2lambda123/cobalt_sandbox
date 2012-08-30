// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_BASE_UPLOAD_ELEMENT_H_
#define NET_BASE_UPLOAD_ELEMENT_H_

#include <vector>

#include "base/basictypes.h"
#include "base/file_path.h"
#include "base/gtest_prod_util.h"
#include "base/time.h"
#include "googleurl/src/gurl.h"
#include "net/base/net_export.h"

namespace net {

class FileStream;

// A class representing an element contained by UploadData.
class NET_EXPORT UploadElement {
 public:
  enum Type {
    TYPE_BYTES,
    TYPE_FILE,
  };

  UploadElement();
  ~UploadElement();

  Type type() const { return type_; }

  const char* bytes() const { return bytes_start_ ? bytes_start_ : &buf_[0]; }
  uint64 bytes_length() const { return buf_.size() + bytes_length_; }
  const FilePath& file_path() const { return file_path_; }
  uint64 file_range_offset() const { return file_range_offset_; }
  uint64 file_range_length() const { return file_range_length_; }
  // If NULL time is returned, we do not do the check.
  const base::Time& expected_file_modification_time() const {
    return expected_file_modification_time_;
  }

  void SetToBytes(const char* bytes, int bytes_len) {
    type_ = TYPE_BYTES;
    buf_.assign(bytes, bytes + bytes_len);
  }

  // This does not copy the given data and the caller should make sure
  // the data is secured somewhere else (e.g. by attaching the data
  // using SetUserData).
  void SetToSharedBytes(const char* bytes, int bytes_len) {
    type_ = TYPE_BYTES;
    bytes_start_ = bytes;
    bytes_length_ = bytes_len;
  }

  void SetToFilePath(const FilePath& path) {
    SetToFilePathRange(path, 0, kuint64max, base::Time());
  }

  // If expected_modification_time is NULL, we do not check for the file
  // change. Also note that the granularity for comparison is time_t, not
  // the full precision.
  void SetToFilePathRange(const FilePath& path,
                          uint64 offset, uint64 length,
                          const base::Time& expected_modification_time) {
    type_ = TYPE_FILE;
    file_path_ = path;
    file_range_offset_ = offset;
    file_range_length_ = length;
    expected_file_modification_time_ = expected_modification_time;
  }

  // Returns the byte-length of the element.  For files that do not exist, 0
  // is returned.  This is done for consistency with Mozilla.
  uint64 GetContentLength();

  // Reads up to |buf_len| bytes synchronously. Returns the number of bytes
  // read. This function never fails. If there's less data to read than we
  // initially observed, then pad with zero (this can happen with files).
  // |buf_len| must be greater than 0.
  int ReadSync(char* buf, int buf_len);

  // Returns the number of bytes remaining to read.
  uint64 BytesRemaining();

  // Resets the offset to zero and closes the file stream if opened, so
  // that the element can be reread.
  void ResetOffset();

 private:
  // Returns a FileStream opened for reading for this element, positioned
  // at |file_range_offset_|. Returns NULL if the file is not openable.
  FileStream* OpenFileStream();

  // Reads up to |buf_len| bytes synchronously from memory (i.e. type_ is
  // TYPE_BYTES).
  int ReadFromMemorySync(char* buf, int buf_len);

  // Reads up to |buf_len| bytes synchronously from a file (i.e. type_ is
  // TYPE_FILE).
  int ReadFromFileSync(char* buf, int buf_len);

  // Allows tests to override the result of GetContentLength.
  void SetContentLength(uint64 content_length) {
    override_content_length_ = true;
    content_length_ = content_length;
  }

  Type type_;
  std::vector<char> buf_;
  const char* bytes_start_;
  uint64 bytes_length_;
  FilePath file_path_;
  uint64 file_range_offset_;
  uint64 file_range_length_;
  base::Time expected_file_modification_time_;
  bool override_content_length_;
  bool content_length_computed_;
  uint64 content_length_;

  // The byte offset from the beginning of the element data. Used to track
  // the current position when reading data.
  uint64 offset_;

  // The stream of the element data, if this element is of TYPE_FILE.
  FileStream* file_stream_;

  FRIEND_TEST_ALL_PREFIXES(UploadDataStreamTest, FileSmallerThanLength);
  FRIEND_TEST_ALL_PREFIXES(HttpNetworkTransactionTest,
                           UploadFileSmallerThanLength);
  FRIEND_TEST_ALL_PREFIXES(HttpNetworkTransactionSpdy2Test,
                           UploadFileSmallerThanLength);
  FRIEND_TEST_ALL_PREFIXES(HttpNetworkTransactionSpdy3Test,
                           UploadFileSmallerThanLength);
};

#if defined(UNIT_TEST)
inline bool operator==(const UploadElement& a,
                       const UploadElement& b) {
  if (a.type() != b.type())
    return false;
  if (a.type() == UploadElement::TYPE_BYTES)
    return a.bytes_length() == b.bytes_length() &&
           memcmp(a.bytes(), b.bytes(), b.bytes_length()) == 0;
  if (a.type() == UploadElement::TYPE_FILE) {
    return a.file_path() == b.file_path() &&
           a.file_range_offset() == b.file_range_offset() &&
           a.file_range_length() == b.file_range_length() &&
           a.expected_file_modification_time() ==
              b.expected_file_modification_time();
  }
  return false;
}

inline bool operator!=(const UploadElement& a,
                       const UploadElement& b) {
  return !(a == b);
}
#endif  // defined(UNIT_TEST)

}  // namespace net

#endif  // NET_BASE_UPLOAD_ELEMENT_H_
