// Copyright (c) 2009 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "base/i18n/icu_string_conversions.h"

#include <vector>

#include "base/basictypes.h"
#include "base/logging.h"
#include "base/string_util.h"
#include "unicode/ucnv.h"
#include "unicode/ucnv_cb.h"
#include "unicode/ucnv_err.h"
#include "unicode/ustring.h"

namespace {

inline bool IsValidCodepoint(uint32 code_point) {
  // Excludes the surrogate code points ([0xD800, 0xDFFF]) and
  // codepoints larger than 0x10FFFF (the highest codepoint allowed).
  // Non-characters and unassigned codepoints are allowed.
  return code_point < 0xD800u ||
         (code_point >= 0xE000u && code_point <= 0x10FFFFu);
}

// ToUnicodeCallbackSubstitute() is based on UCNV_TO_U_CALLBACK_SUSBSTITUTE
// in source/common/ucnv_err.c.

// Copyright (c) 1995-2006 International Business Machines Corporation
// and others
//
// All rights reserved.
//

// Permission is hereby granted, free of charge, to any person obtaining a
// copy of this software and associated documentation files (the "Software"),
// to deal in the Software without restriction, including without limitation
// the rights to use, copy, modify, merge, publish, distribute, and/or
// sell copies of the Software, and to permit persons to whom the Software
// is furnished to do so, provided that the above copyright notice(s) and
// this permission notice appear in all copies of the Software and that
// both the above copyright notice(s) and this permission notice appear in
// supporting documentation.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
// EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
// MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT
// OF THIRD PARTY RIGHTS. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR HOLDERS
// INCLUDED IN THIS NOTICE BE LIABLE FOR ANY CLAIM, OR ANY SPECIAL INDIRECT
// OR CONSEQUENTIAL DAMAGES, OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS
// OF USE, DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE
// OR OTHER TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE
// OR PERFORMANCE OF THIS SOFTWARE.
//
// Except as contained in this notice, the name of a copyright holder
// shall not be used in advertising or otherwise to promote the sale, use
// or other dealings in this Software without prior written authorization
// of the copyright holder.

//  ___________________________________________________________________________
//
// All trademarks and registered trademarks mentioned herein are the property
// of their respective owners.

void ToUnicodeCallbackSubstitute(const void* context,
                                 UConverterToUnicodeArgs *to_args,
                                 const char* code_units,
                                 int32_t length,
                                 UConverterCallbackReason reason,
                                 UErrorCode * err) {
  static const UChar kReplacementChar = 0xFFFD;
  if (reason <= UCNV_IRREGULAR) {
      if (context == NULL ||
          (*(reinterpret_cast<const char*>(context)) == 'i' &&
           reason == UCNV_UNASSIGNED)) {
        *err = U_ZERO_ERROR;
        ucnv_cbToUWriteUChars(to_args, &kReplacementChar, 1, 0, err);
      }
      // else the caller must have set the error code accordingly.
  }
  // else ignore the reset, close and clone calls.
}

bool ConvertFromUTF16(UConverter* converter, const UChar* uchar_src,
                      int uchar_len, OnStringUtilConversionError::Type on_error,
                      std::string* encoded) {
  int encoded_max_length = UCNV_GET_MAX_BYTES_FOR_STRING(uchar_len,
      ucnv_getMaxCharSize(converter));
  encoded->resize(encoded_max_length);

  UErrorCode status = U_ZERO_ERROR;

  // Setup our error handler.
  switch (on_error) {
    case OnStringUtilConversionError::FAIL:
      ucnv_setFromUCallBack(converter, UCNV_FROM_U_CALLBACK_STOP, 0,
                            NULL, NULL, &status);
      break;
    case OnStringUtilConversionError::SKIP:
    case OnStringUtilConversionError::SUBSTITUTE:
      ucnv_setFromUCallBack(converter, UCNV_FROM_U_CALLBACK_SKIP, 0,
                            NULL, NULL, &status);
      break;
    default:
      NOTREACHED();
  }

  // ucnv_fromUChars returns size not including terminating null
  int actual_size = ucnv_fromUChars(converter, &(*encoded)[0],
      encoded_max_length, uchar_src, uchar_len, &status);
  encoded->resize(actual_size);
  ucnv_close(converter);
  if (U_SUCCESS(status))
    return true;
  encoded->clear();  // Make sure the output is empty on error.
  return false;
}

// Set up our error handler for ToUTF-16 converters
void SetUpErrorHandlerForToUChars(OnStringUtilConversionError::Type on_error,
                                  UConverter* converter, UErrorCode* status) {
  switch (on_error) {
    case OnStringUtilConversionError::FAIL:
      ucnv_setToUCallBack(converter, UCNV_TO_U_CALLBACK_STOP, 0,
                          NULL, NULL, status);
      break;
    case OnStringUtilConversionError::SKIP:
      ucnv_setToUCallBack(converter, UCNV_TO_U_CALLBACK_SKIP, 0,
                          NULL, NULL, status);
      break;
    case OnStringUtilConversionError::SUBSTITUTE:
      ucnv_setToUCallBack(converter, ToUnicodeCallbackSubstitute, 0,
                          NULL, NULL, status);
      break;
    default:
      NOTREACHED();
  }
}

inline UConverterType utf32_platform_endian() {
#if U_IS_BIG_ENDIAN
  return UCNV_UTF32_BigEndian;
#else
  return UCNV_UTF32_LittleEndian;
#endif
}

}  // namespace

// Codepage <-> Wide/UTF-16  ---------------------------------------------------

// Convert a wstring into the specified codepage_name.  If the codepage
// isn't found, return false.
bool WideToCodepage(const std::wstring& wide,
                    const char* codepage_name,
                    OnStringUtilConversionError::Type on_error,
                    std::string* encoded) {
#if defined(WCHAR_T_IS_UTF16)
  return UTF16ToCodepage(wide, codepage_name, on_error, encoded);
#elif defined(WCHAR_T_IS_UTF32)
  encoded->clear();

  UErrorCode status = U_ZERO_ERROR;
  UConverter* converter = ucnv_open(codepage_name, &status);
  if (!U_SUCCESS(status))
    return false;

  int utf16_len;
  // When wchar_t is wider than UChar (16 bits), transform |wide| into a
  // UChar* string.  Size the UChar* buffer to be large enough to hold twice
  // as many UTF-16 code units (UChar's) as there are Unicode code points,
  // in case each code points translates to a UTF-16 surrogate pair,
  // and leave room for a NUL terminator.
  std::vector<UChar> utf16(wide.length() * 2 + 1);
  u_strFromWCS(&utf16[0], utf16.size(), &utf16_len,
               wide.c_str(), wide.length(), &status);
  DCHECK(U_SUCCESS(status)) << "failed to convert wstring to UChar*";

  return ConvertFromUTF16(converter, &utf16[0], utf16_len, on_error, encoded);
#endif  // defined(WCHAR_T_IS_UTF32)
}

// Convert a UTF-16 string into the specified codepage_name.  If the codepage
// isn't found, return false.
bool UTF16ToCodepage(const string16& utf16,
                    const char* codepage_name,
                    OnStringUtilConversionError::Type on_error,
                    std::string* encoded) {
  encoded->clear();

  UErrorCode status = U_ZERO_ERROR;
  UConverter* converter = ucnv_open(codepage_name, &status);
  if (!U_SUCCESS(status))
    return false;

  return ConvertFromUTF16(converter, utf16.c_str(),
                          static_cast<int>(utf16.length()), on_error, encoded);
}

// Converts a string of the given codepage into wstring.
// If the codepage isn't found, return false.
bool CodepageToWide(const std::string& encoded,
                    const char* codepage_name,
                    OnStringUtilConversionError::Type on_error,
                    std::wstring* wide) {
#if defined(WCHAR_T_IS_UTF16)
  return CodepageToUTF16(encoded, codepage_name, on_error, wide);
#elif defined(WCHAR_T_IS_UTF32)
  wide->clear();

  UErrorCode status = U_ZERO_ERROR;
  UConverter* converter = ucnv_open(codepage_name, &status);
  if (!U_SUCCESS(status))
    return false;

  // The maximum length in 4 byte unit of UTF-32 output would be
  // at most the same as the number of bytes in input. In the worst
  // case of GB18030 (excluding escaped-based encodings like ISO-2022-JP),
  // this can be 4 times larger than actually needed.
  size_t wchar_max_length = encoded.length() + 1;

  // The byte buffer and its length to pass to ucnv_toAlgorithimic.
  char* byte_buffer = reinterpret_cast<char*>(
      WriteInto(wide, wchar_max_length));
  int byte_buffer_length = static_cast<int>(wchar_max_length) * 4;

  SetUpErrorHandlerForToUChars(on_error, converter, &status);
  int actual_size = ucnv_toAlgorithmic(utf32_platform_endian(),
                                       converter,
                                       byte_buffer,
                                       byte_buffer_length,
                                       encoded.data(),
                                       static_cast<int>(encoded.length()),
                                       &status);
  ucnv_close(converter);

  if (!U_SUCCESS(status)) {
    wide->clear();  // Make sure the output is empty on error.
    return false;
  }

  // actual_size is # of bytes.
  wide->resize(actual_size / 4);
  return true;
#endif  // defined(WCHAR_T_IS_UTF32)
}

// Converts a string of the given codepage into UTF-16.
// If the codepage isn't found, return false.
bool CodepageToUTF16(const std::string& encoded,
                     const char* codepage_name,
                     OnStringUtilConversionError::Type on_error,
                     string16* utf16) {
  utf16->clear();

  UErrorCode status = U_ZERO_ERROR;
  UConverter* converter = ucnv_open(codepage_name, &status);
  if (!U_SUCCESS(status))
    return false;

  // Even in the worst case, the maximum length in 2-byte units of UTF-16
  // output would be at most the same as the number of bytes in input. There
  // is no single-byte encoding in which a character is mapped to a
  // non-BMP character requiring two 2-byte units.
  //
  // Moreover, non-BMP characters in legacy multibyte encodings
  // (e.g. EUC-JP, GB18030) take at least 2 bytes. The only exceptions are
  // BOCU and SCSU, but we don't care about them.
  size_t uchar_max_length = encoded.length() + 1;

  SetUpErrorHandlerForToUChars(on_error, converter, &status);
  int actual_size = ucnv_toUChars(converter,
                                  WriteInto(utf16, uchar_max_length),
                                  static_cast<int>(uchar_max_length),
                                  encoded.data(),
                                  static_cast<int>(encoded.length()),
                                  &status);
  ucnv_close(converter);
  if (!U_SUCCESS(status)) {
    utf16->clear();  // Make sure the output is empty on error.
    return false;
  }

  utf16->resize(actual_size);
  return true;
}

