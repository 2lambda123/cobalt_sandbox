// Copyright (c) 2006-2008 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef BASE_FILE_VERSION_INFO_H__
#define BASE_FILE_VERSION_INFO_H__
#pragma once

#include "build/build_config.h"

#include <string>

#include "base/string16.h"

class FilePath;

// Provides an interface for accessing the version information for a file. This
// is the information you access when you select a file in the Windows Explorer,
// right-click select Properties, then click the Version tab, and on the Mac
// when you select a file in the Finder and do a Get Info.
//
// This list of properties is straight out of Win32's VerQueryValue
// <http://msdn.microsoft.com/en-us/library/ms647464.aspx> and the Mac
// version returns values from the Info.plist as appropriate. TODO(avi): make
// this a less-obvious Windows-ism.

class FileVersionInfo {
 public:
  virtual ~FileVersionInfo() {}
#if defined(OS_WIN) || defined(OS_MACOSX)
  // Creates a FileVersionInfo for the specified path. Returns NULL if something
  // goes wrong (typically the file does not exit or cannot be opened). The
  // returned object should be deleted when you are done with it.
  static FileVersionInfo* CreateFileVersionInfo(const FilePath& file_path);
#endif  // OS_WIN || OS_MACOSX

  // Creates a FileVersionInfo for the current module. Returns NULL in case
  // of error. The returned object should be deleted when you are done with it.
  static FileVersionInfo* CreateFileVersionInfoForCurrentModule();

  // Accessors to the different version properties.
  // Returns an empty string if the property is not found.
  virtual string16 company_name() = 0;
  virtual string16 company_short_name() = 0;
  virtual string16 product_name() = 0;
  virtual string16 product_short_name() = 0;
  virtual string16 internal_name() = 0;
  virtual string16 product_version() = 0;
  virtual string16 private_build() = 0;
  virtual string16 special_build() = 0;
  virtual string16 comments() = 0;
  virtual string16 original_filename() = 0;
  virtual string16 file_description() = 0;
  virtual string16 file_version() = 0;
  virtual string16 legal_copyright() = 0;
  virtual string16 legal_trademarks() = 0;
  virtual string16 last_change() = 0;
  virtual bool is_official_build() = 0;
};

#endif  // BASE_FILE_VERSION_INFO_H__
