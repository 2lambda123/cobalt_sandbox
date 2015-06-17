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

#include "cobalt/storage/savegame.h"

#include <algorithm>

#include "base/file_util.h"
#include "base/path_service.h"

namespace cobalt {
namespace storage {

namespace {
const char kDefaultSavegamePath[] = ".cobalt.sav";

FilePath GetSavegamePath() {
  FilePath exe_dir;
  bool ok = PathService::Get(base::DIR_EXE, &exe_dir);
  DCHECK_EQ(true, ok);
  return exe_dir.Append(kDefaultSavegamePath);
}

}  // namespace

class SavegameFile : public Savegame {
 public:
  explicit SavegameFile(const Options& options) : Savegame(options) {
    if (options.path_override.length() > 0) {
      savegame_path_ = FilePath(options.path_override);
    } else {
      savegame_path_ = GetSavegamePath();
    }
  }

  ~SavegameFile() OVERRIDE {
    if (options_.delete_on_destruction) {
      Delete();
    }
  }

  bool PlatformRead(ByteVector* bytes) OVERRIDE;
  bool PlatformWrite(const ByteVector& bytes) OVERRIDE;
  bool PlatformDelete() OVERRIDE;

 private:
  FilePath savegame_path_;
};

bool SavegameFile::PlatformRead(ByteVector* bytes_ptr) {
  if (!file_util::PathExists(savegame_path_)) {
    DLOG(INFO) << savegame_path_.value() << " does not exist.";
    return false;
  }

  int64 file_size;
  if (!file_util::GetFileSize(savegame_path_, &file_size)) {
    DLOG(WARNING) << "GetFileSize of " << savegame_path_.value() << " failed";
    return false;
  }
  ByteVector& bytes = *bytes_ptr;
  bytes.resize(file_size);
  int bytes_read = file_util::ReadFile(
      savegame_path_, reinterpret_cast<char*>(&bytes[0]), file_size);
  bytes.resize(std::max(0, bytes_read));
  return bytes_read == file_size;
}

bool SavegameFile::PlatformWrite(const ByteVector& bytes) {
  int byte_count = bytes.size();
  int bytes_written = file_util::WriteFile(
      savegame_path_, reinterpret_cast<const char*>(&bytes[0]), byte_count);
  return bytes_written == byte_count;
}

bool SavegameFile::PlatformDelete() {
  return file_util::Delete(savegame_path_, false /* recursive */);
}

// static
scoped_ptr<Savegame> Savegame::Create(const Options& options) {
  scoped_ptr<Savegame> savegame(new SavegameFile(options));
  return savegame.Pass();
}

}  // namespace storage
}  // namespace cobalt
