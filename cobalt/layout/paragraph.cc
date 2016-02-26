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

#include "cobalt/layout/paragraph.h"

#include "base/i18n/char_iterator.h"

#include "third_party/icu/public/common/unicode/ubidi.h"


namespace cobalt {
namespace layout {

namespace {

int ConvertBaseDirectionToBidiLevel(Paragraph::BaseDirection base_direction) {
  return base_direction == Paragraph::kRightToLeftBaseDirection ? 1 : 0;
}

}  // namespace

Paragraph::Paragraph(const icu::Locale& locale,
                     Paragraph::BaseDirection base_direction,
                     icu::BreakIterator* line_break_iterator,
                     icu::BreakIterator* character_break_iterator)
    : locale_(locale),
      base_direction_(base_direction),
      line_break_iterator_(line_break_iterator),
      character_break_iterator_(character_break_iterator),
      is_closed_(false),
      last_retrieved_run_index_(0) {
  DCHECK(line_break_iterator_);
  DCHECK(character_break_iterator_);

  level_runs_.push_back(
      BidiLevelRun(0, ConvertBaseDirectionToBidiLevel(base_direction)));
}

int32 Paragraph::AppendUtf8String(const std::string& utf8_string) {
  return AppendUtf8String(utf8_string, kNoTextTransform);
}

int32 Paragraph::AppendUtf8String(const std::string& utf8_string,
                                  TextTransform transform) {
  int32 start_position = GetTextEndPosition();
  DCHECK(!is_closed_);
  if (!is_closed_) {
    icu::UnicodeString unicode_string =
        icu::UnicodeString::fromUTF8(utf8_string);
    if (transform == kUppercaseTextTransform) {
      unicode_string.toUpper(locale_);
    }

    unicode_text_ += unicode_string;
  }

  return start_position;
}

int32 Paragraph::AppendCodePoint(CodePoint code_point) {
  int32 start_position = GetTextEndPosition();
  DCHECK(!is_closed_);
  if (!is_closed_) {
    switch (code_point) {
      case kLineFeedCodePoint:
        unicode_text_ += UChar(0x0a);
        break;
      case kObjectReplacementCharacterCodePoint:
        unicode_text_ += UChar(0xfffc);
        break;
    }
  }
  return start_position;
}

bool Paragraph::FindBreakPosition(const scoped_refptr<dom::FontList>& used_font,
                                  int32 start_position, int32 end_position,
                                  float available_width, bool allow_overflow,
                                  BreakPolicy break_policy,
                                  int32* break_position, float* break_width) {
  DCHECK(is_closed_);

  *break_position = start_position;
  *break_width = 0;

  // If overflow isn't allowed and there is no available width, then there is
  // nothing to do. No break position can be found.
  if (!allow_overflow && available_width <= 0) {
    return false;
  }

  // If the break policy allows for soft wrapping, then always initially attempt
  // a soft wrap break. However, only allow soft wrap overflow if the policy is
  // |kSoftWrap|; otherwise, overflowing via soft wrap would be too greedy in
  // what it included and overflow wrapping will be attempted via word breaking.
  bool allow_soft_wrap_overflow = allow_overflow && break_policy == kSoftWrap;

  // Find the last available soft wrap break position. |break_position| and
  // |break_width| will be updated with the position of the last available break
  // position.
  FindIteratorBreakPosition(
      used_font, line_break_iterator_, start_position, end_position,
      available_width, allow_soft_wrap_overflow, break_position, break_width);

  // Only continue allowing overflow if the break position has not moved from
  // start, meaning that no break position has been found yet.
  allow_overflow = allow_overflow && (*break_position == start_position);

  // If break word is allowed by the break policy, attempt to break unbreakable
  // "words" at an arbitrary point, while still maintaining grapheme clusters
  // as indivisible units. The search begins at the location of the last soft
  // wrap break position that fit within the available width.
  //
  // NOTE: In the case where the policy is |kSoftWrapWithBreakWordOnOverflow|,
  // this is only permitted when overflow is allowed, meaning that no
  // otherwise-acceptable break points have been found in the line
  // (https://www.w3.org/TR/css3-text/#overflow-wrap).
  if ((break_policy == kBreakWord) ||
      (break_policy == kSoftWrapWithBreakWordOnOverflow && allow_overflow)) {
    FindIteratorBreakPosition(used_font, character_break_iterator_,
                              *break_position, end_position, available_width,
                              allow_overflow, break_position, break_width);
  }

  // No usable break position was found if the break position has not moved
  // from the start position.
  return *break_position > start_position;
}

std::string Paragraph::RetrieveUtf8SubString(int32 start_position,
                                             int32 end_position) const {
  return RetrieveUtf8SubString(start_position, end_position, kLogicalTextOrder);
}

std::string Paragraph::RetrieveUtf8SubString(int32 start_position,
                                             int32 end_position,
                                             TextOrder text_order) const {
  std::string utf8_sub_string;

  if (start_position < end_position) {
    icu::UnicodeString unicode_sub_string =
        unicode_text_.tempSubStringBetween(start_position, end_position);

    // Odd bidi levels signify RTL directionality. If the text is being
    // retrieved in visual order and has RTL directionality, then reverse the
    // text.
    if (text_order == kVisualTextOrder && IsRTL(start_position)) {
      unicode_sub_string.reverse();
    }

    unicode_sub_string.toUTF8String(utf8_sub_string);
  }

  return utf8_sub_string;
}

const char16* Paragraph::GetTextBuffer() const {
  return unicode_text_.getBuffer();
}

const icu::Locale& Paragraph::GetLocale() const { return locale_; }

Paragraph::BaseDirection Paragraph::GetBaseDirection() const {
  return base_direction_;
}

int Paragraph::GetBidiLevel(int32 position) const {
  return level_runs_[GetRunIndex(position)].level_;
}

bool Paragraph::IsRTL(int32 position) const {
  return (GetBidiLevel(position) % 2) == 1;
}

bool Paragraph::IsSpace(int32 position) const {
  return unicode_text_[position] == ' ';
}

bool Paragraph::GetNextRunPosition(int32 position,
                                   int32* next_run_position) const {
  size_t next_run_index = GetRunIndex(position) + 1;
  if (next_run_index >= level_runs_.size()) {
    return false;
  } else {
    *next_run_position = level_runs_[next_run_index].start_position_;
    return true;
  }
}

int32 Paragraph::GetTextEndPosition() const { return unicode_text_.length(); }

void Paragraph::Close() {
  DCHECK(!is_closed_);
  if (!is_closed_) {
    is_closed_ = true;
    GenerateBidiLevelRuns();
  }
}

bool Paragraph::IsClosed() const { return is_closed_; }

void Paragraph::FindIteratorBreakPosition(
    const scoped_refptr<dom::FontList>& used_font,
    icu::BreakIterator* const break_iterator, int32 start_position,
    int32 end_position, float available_width, bool allow_overflow,
    int32* break_position, float* break_width) {
  // Iterate through soft wrap locations, beginning from the passed in start
  // position. Continue until TryIncludeSegmentWithinAvailableWidth() returns
  // false, indicating that no more segments can be included.
  break_iterator->setText(unicode_text_);
  for (int32 segment_end = break_iterator->following(start_position);
       segment_end != icu::BreakIterator::DONE && segment_end < end_position;
       segment_end = break_iterator->next()) {
    if (!TryIncludeSegmentWithinAvailableWidth(
            used_font, *break_position, segment_end, available_width,
            &allow_overflow, break_position, break_width)) {
      break;
    }
  }
}

bool Paragraph::TryIncludeSegmentWithinAvailableWidth(
    const scoped_refptr<dom::FontList>& used_font, int32 segment_start,
    int32 segment_end, float available_width, bool* allow_overflow,
    int32* break_position, float* break_width) {
  // Add the width of the segment encountered to the total, until reaching one
  // that causes the available width to be exceeded. The previous break position
  // is the last usable one. However, if overflow is allowed and no segment has
  // been found, then the first overflowing segment is accepted.
  float segment_width = used_font->GetTextWidth(
      unicode_text_.getBuffer() + segment_start, segment_end - segment_start,
      IsRTL(segment_start), NULL);

  if (!*allow_overflow && *break_width + segment_width > available_width) {
    return false;
  }

  *break_position = segment_end;
  *break_width += segment_width;

  if (*allow_overflow) {
    *allow_overflow = false;
    if (*break_width >= available_width) {
      return false;
    }
  }

  return true;
}

namespace {

// This class guarantees that a UBiDi object is closed when it goes out of
// scope.
class ScopedUBiDiPtr {
 public:
  explicit ScopedUBiDiPtr(UBiDi* ubidi) : ubidi_(ubidi) {}

  ~ScopedUBiDiPtr() { ubidi_close(ubidi_); }

  UBiDi* get() const { return ubidi_; }

 private:
  UBiDi* ubidi_;
};

}  // namespace

// Should only be called by Close().
void Paragraph::GenerateBidiLevelRuns() {
  DCHECK(is_closed_);

  UErrorCode error = U_ZERO_ERROR;

  // Create a scoped ubidi ptr when opening the text. It guarantees that the
  // UBiDi object will be closed when it goes out of scope.
  ScopedUBiDiPtr ubidi(ubidi_openSized(unicode_text_.length(), 0, &error));
  if (U_FAILURE(error)) {
    return;
  }

  ubidi_setPara(ubidi.get(), unicode_text_.getBuffer(), unicode_text_.length(),
                UBiDiLevel(ConvertBaseDirectionToBidiLevel(base_direction_)),
                NULL, &error);
  if (U_FAILURE(error)) {
    return;
  }

  const int runs = ubidi_countRuns(ubidi.get(), &error);
  if (U_FAILURE(error)) {
    return;
  }

  level_runs_.clear();
  level_runs_.reserve(size_t(runs));

  int run_start_position = 0;
  int run_end_position;
  UBiDiLevel run_ubidi_level;

  int32 text_end_position = GetTextEndPosition();
  while (run_start_position < text_end_position) {
    ubidi_getLogicalRun(ubidi.get(), run_start_position, &run_end_position,
                        &run_ubidi_level);
    level_runs_.push_back(
        BidiLevelRun(run_start_position, static_cast<int>(run_ubidi_level)));
    run_start_position = run_end_position;
  }
}

size_t Paragraph::GetRunIndex(int32 position) const {
  DCHECK(is_closed_);

  // Start the search from the last retrieved index. This is tracked because
  // the next retrieved run index is almost always either the same as the last
  // retrieved one or a neighbor of it, so this reduces nearly all calls to only
  // requiring a couple comparisons.

  // First iterate up the level run vector, finding the first element that has
  // a start position larger than the passed in position. The upward iteration
  // is stopped if the last element is reached.
  while (last_retrieved_run_index_ < level_runs_.size() - 1 &&
         level_runs_[last_retrieved_run_index_].start_position_ < position) {
    ++last_retrieved_run_index_;
  }
  // Iterate down the level run vector until a start position smaller than or
  // equal to the passed in position is reached. This run contains the passed in
  // position. The start position of the first run has a value of 0 and serves
  // as a sentinel for the loop.
  while (level_runs_[last_retrieved_run_index_].start_position_ > position) {
    --last_retrieved_run_index_;
  }

  return last_retrieved_run_index_;
}

}  // namespace layout
}  // namespace cobalt
