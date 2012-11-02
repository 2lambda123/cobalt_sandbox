// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "net/ftp/ftp_directory_listing_parser.h"

#include "base/bind.h"
#include "base/callback.h"
#include "base/i18n/icu_encoding_detection.h"
#include "base/i18n/icu_string_conversions.h"
#include "base/stl_util.h"
#include "base/string_split.h"
#include "base/string_util.h"
#include "net/base/net_errors.h"
#include "net/ftp/ftp_directory_listing_parser_ls.h"
#include "net/ftp/ftp_directory_listing_parser_netware.h"
#include "net/ftp/ftp_directory_listing_parser_os2.h"
#include "net/ftp/ftp_directory_listing_parser_vms.h"
#include "net/ftp/ftp_directory_listing_parser_windows.h"
#include "net/ftp/ftp_server_type_histograms.h"

namespace net {

namespace {

// Fills in |raw_name| for all |entries| using |encoding|. Returns network
// error code.
int FillInRawName(const std::string& encoding,
                  std::vector<FtpDirectoryListingEntry>* entries) {
  for (size_t i = 0; i < entries->size(); i++) {
    if (!base::UTF16ToCodepage(entries->at(i).name, encoding.c_str(),
                               base::OnStringConversionError::FAIL,
                               &entries->at(i).raw_name)) {
      return ERR_ENCODING_CONVERSION_FAILED;
    }
  }

  return OK;
}

// Parses |text| as an FTP directory listing. Fills in |entries|
// and |server_type| and returns network error code.
int ParseListing(const string16& text,
                 const std::string& encoding,
                 const base::Time& current_time,
                 std::vector<FtpDirectoryListingEntry>* entries,
                 FtpServerType* server_type) {
  std::vector<string16> lines;
  base::SplitString(text, '\n', &lines);

  struct {
    base::Callback<bool(void)> callback;
    FtpServerType server_type;
  } parsers[] = {
    {
      base::Bind(&ParseFtpDirectoryListingLs, lines, current_time, entries),
      SERVER_LS
    },
    {
      base::Bind(&ParseFtpDirectoryListingWindows, lines, entries),
      SERVER_WINDOWS
    },
    {
      base::Bind(&ParseFtpDirectoryListingVms, lines, entries),
      SERVER_VMS
    },
    {
      base::Bind(&ParseFtpDirectoryListingNetware,
                 lines, current_time, entries),
      SERVER_NETWARE
    },
    {
      base::Bind(&ParseFtpDirectoryListingOS2, lines, entries),
      SERVER_OS2
    }
  };

  for (size_t i = 0; i < ARRAYSIZE_UNSAFE(parsers); i++) {
    entries->clear();
    if (parsers[i].callback.Run()) {
      *server_type = parsers[i].server_type;
      return FillInRawName(encoding, entries);
    }
  }

  entries->clear();
  return ERR_UNRECOGNIZED_FTP_DIRECTORY_LISTING_FORMAT;
}

// Detects encoding of |text| and parses it as an FTP directory listing.
// Fills in |entries| and |server_type| and returns network error code.
int DecodeAndParse(const std::string& text,
                   const base::Time& current_time,
                   std::vector<FtpDirectoryListingEntry>* entries,
                   FtpServerType* server_type) {
  std::vector<std::string> encodings;
  if (!base::DetectAllEncodings(text, &encodings))
    return ERR_ENCODING_DETECTION_FAILED;

  // Use first encoding that can be used to decode the text.
  for (size_t i = 0; i < encodings.size(); i++) {
    string16 converted_text;
    if (base::CodepageToUTF16(text,
                              encodings[i].c_str(),
                              base::OnStringConversionError::FAIL,
                              &converted_text)) {
      int rv = ParseListing(converted_text,
                            encodings[i],
                            current_time,
                            entries,
                            server_type);
      if (rv == OK)
        return rv;
    }
  }

  entries->clear();
  *server_type = SERVER_UNKNOWN;
  return ERR_UNRECOGNIZED_FTP_DIRECTORY_LISTING_FORMAT;
}

}  // namespace

FtpDirectoryListingEntry::FtpDirectoryListingEntry()
    : type(UNKNOWN),
      size(-1) {
}

int ParseFtpDirectoryListing(const std::string& text,
                             const base::Time& current_time,
                             std::vector<FtpDirectoryListingEntry>* entries) {
  FtpServerType server_type = SERVER_UNKNOWN;
  int rv = DecodeAndParse(text, current_time, entries, &server_type);
  UpdateFtpServerTypeHistograms(server_type);
  return rv;
}

}  // namespace net
