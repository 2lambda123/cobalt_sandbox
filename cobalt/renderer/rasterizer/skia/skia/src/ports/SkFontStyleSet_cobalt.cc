// Copyright 2017 Google Inc. All Rights Reserved.
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

#include "cobalt/renderer/rasterizer/skia/skia/src/ports/SkFontStyleSet_cobalt.h"

#include <cmath>
#include <limits>

#include "base/debug/trace_event.h"
#include "base/logging.h"
#include "base/memory/ref_counted.h"
#include "cobalt/renderer/rasterizer/skia/skia/src/ports/SkTypeface_cobalt.h"
#include "SkOSFile.h"

namespace {

int MatchScore(const SkFontStyle& pattern, const SkFontStyle& candidate) {
  int score = 0;
  score += std::abs((pattern.width() - candidate.width()) * 100);
  score += (pattern.isItalic() == candidate.isItalic()) ? 0 : 1000;
  score += std::abs(pattern.weight() - candidate.weight());
  return score;
}

}  // namespace

// These functions are used by FreeType during FT_Open_Face.
extern "C" {

static unsigned long sk_cobalt_ft_stream_io(FT_Stream ftStream,
                                            unsigned long offset,
                                            unsigned char* buffer,
                                            unsigned long count) {
  SkStreamAsset* stream =
      static_cast<SkStreamAsset*>(ftStream->descriptor.pointer);
  stream->seek(offset);
  return stream->read(buffer, count);
}

static void sk_cobalt_ft_stream_close(FT_Stream) {}
}

//  This class is used by SkFontMgr_Cobalt to hold SkTypeface_CobaltSystem
//  families.
SkFontStyleSet_Cobalt::SkFontStyleSet_Cobalt(
    const FontFamily& family, const char* base_path,
    SkFileMemoryChunkStreamManager* const system_typeface_stream_manager,
    SkMutex* const manager_owned_mutex)
    : system_typeface_stream_manager_(system_typeface_stream_manager),
      manager_owned_mutex_(manager_owned_mutex),
      is_fallback_family_(family.is_fallback_family),
      language_(family.language),
      page_ranges_(family.page_ranges),
      is_character_map_generated_(!is_fallback_family_) {
  TRACE_EVENT0("cobalt::renderer",
               "SkFontStyleSet_Cobalt::SkFontStyleSet_Cobalt()");
  DCHECK(manager_owned_mutex_);

  if (family.names.count() == 0) {
    return;
  }

  family_name_ = family.names[0];

  for (int i = 0; i < family.fonts.count(); ++i) {
    const FontFileInfo& font_file = family.fonts[i];

    SkString file_path(SkOSPath::Join(base_path, font_file.file_name.c_str()));

    // Sanity check that something exists at this location.
    if (!sk_exists(file_path.c_str(), kRead_SkFILE_Flag)) {
      LOG(ERROR) << "Failed to find font file: " << file_path.c_str();
      continue;
    }

    SkFontStyle style(font_file.weight, SkFontStyle::kNormal_Width,
                      font_file.style == FontFileInfo::kItalic_FontStyle
                          ? SkFontStyle::kItalic_Slant
                          : SkFontStyle::kUpright_Slant);

    std::string full_font_name;
    if (!font_file.full_font_name.isEmpty()) {
      full_font_name = std::string(font_file.full_font_name.c_str(),
                                   font_file.full_font_name.size());
    }
    std::string postscript_name;
    if (!font_file.postscript_name.isEmpty()) {
      postscript_name = std::string(font_file.postscript_name.c_str(),
                                    font_file.postscript_name.size());
    }

    styles_.push_back().reset(SkNEW_ARGS(
        SkFontStyleSetEntry_Cobalt,
        (file_path, font_file.index, style, full_font_name, postscript_name)));
  }
}

int SkFontStyleSet_Cobalt::count() {
  SkAutoMutexAcquire scoped_mutex(*manager_owned_mutex_);
  return styles_.count();
}

void SkFontStyleSet_Cobalt::getStyle(int index, SkFontStyle* style,
                                     SkString* name) {
  // SkFontStyleSet_Cobalt does not support publicly interacting with entries
  // via index, as entries can potentially be removed, thereby invalidating the
  // indices.
  NOTREACHED();
}

SkTypeface* SkFontStyleSet_Cobalt::createTypeface(int index) {
  // SkFontStyleSet_Cobalt does not support publicly interacting with entries
  // via index, as entries can potentially be removed, thereby invalidating the
  // indices.
  NOTREACHED();
  return NULL;
}

SkTypeface* SkFontStyleSet_Cobalt::matchStyle(const SkFontStyle& pattern) {
  SkAutoMutexAcquire scoped_mutex(*manager_owned_mutex_);
  return MatchStyleWithoutLocking(pattern);
}

SkTypeface* SkFontStyleSet_Cobalt::MatchStyleWithoutLocking(
    const SkFontStyle& pattern) {
  SkTypeface* typeface = NULL;
  while (typeface == NULL && styles_.count() > 0) {
    typeface = TryRetrieveTypefaceAndRemoveStyleOnFailure(
        GetClosestStyleIndex(pattern));
  }
  return typeface;
}

SkTypeface* SkFontStyleSet_Cobalt::MatchFullFontName(const std::string& name) {
  for (int i = 0; i < styles_.count(); ++i) {
    if (styles_[i]->full_font_name == name) {
      return TryRetrieveTypefaceAndRemoveStyleOnFailure(i);
    }
  }
  return NULL;
}

SkTypeface* SkFontStyleSet_Cobalt::MatchFontPostScriptName(
    const std::string& name) {
  for (int i = 0; i < styles_.count(); ++i) {
    if (styles_[i]->font_postscript_name == name) {
      return TryRetrieveTypefaceAndRemoveStyleOnFailure(i);
    }
  }
  return NULL;
}

SkTypeface* SkFontStyleSet_Cobalt::TryRetrieveTypefaceAndRemoveStyleOnFailure(
    int style_index) {
  DCHECK(style_index >= 0 && style_index < styles_.count());
  SkFontStyleSetEntry_Cobalt* style = styles_[style_index];
  // If the typeface doesn't already exist, then attempt to create it.
  if (style->typeface == NULL) {
    CreateSystemTypeface(style);
    // If the creation attempt failed and the typeface is still NULL, then
    // remove the entry from the set's styles.
    if (style->typeface == NULL) {
      styles_[style_index].swap(&styles_.back());
      styles_.pop_back();
      return NULL;
    }
  }
  return SkRef(style->typeface.get());
}

bool SkFontStyleSet_Cobalt::ContainsTypeface(const SkTypeface* typeface) {
  for (int i = 0; i < styles_.count(); ++i) {
    if (styles_[i]->typeface == typeface) {
      return true;
    }
  }
  return false;
}

bool SkFontStyleSet_Cobalt::ContainsCharacter(const SkFontStyle& style,
                                              SkUnichar character) {
  // If page ranges exist for this style set, then verify that this character
  // falls within the ranges. Otherwise, the style set is treated as having a
  // page range containing all characters.
  size_t num_ranges = page_ranges_.count();
  if (num_ranges > 0) {
    int16 page = font_character_map::GetPage(character);

    // Verify that the last page range is not less than the page containing the
    // character. If it's less, then this character can't be contained
    // within the pages. Otherwise, this last range acts as a sentinel for the
    // search.
    if (page_ranges_[num_ranges - 1].second < page) {
      return false;
    }

    int range_index = 0;
    while (page_ranges_[range_index].second < page) {
      ++range_index;
    }

    if (page_ranges_[range_index].first > page) {
      return false;
    }
  }

  // If this point is reached, then the character is contained within one of
  // the font style set's page ranges. Now, verify that the specific character
  // is supported by the font style set.

  // The character map is lazily generated. Generate it now if it isn't already
  // generated.
  if (!is_character_map_generated_) {
    TRACE_EVENT0("cobalt::renderer",
                 "SkFontStyleSet_Cobalt::ContainsCharacter() and "
                 "!is_character_map_generated_");
    // Attempt to load the closest font style from the set. If it fails to load,
    // it will be removed from the set and, as long as font styles remain in the
    // set, the logic will be attempted again.
    while (styles_.count() > 0) {
      int style_index = GetClosestStyleIndex(style);
      SkFontStyleSetEntry_Cobalt* closest_style = styles_[style_index];

      SkFileMemoryChunkStreamProvider* stream_provider =
          system_typeface_stream_manager_->GetStreamProvider(
              closest_style->font_file_path.c_str());

      // Create a snapshot prior to loading any additional memory chunks. In the
      // case where the typeface does not end up being created, this enables the
      // stream provider to purge newly created memory chunks, while retaining
      // any pre-existing ones.
      scoped_ptr<const SkFileMemoryChunks> memory_chunks_snapshot(
          stream_provider->CreateMemoryChunksSnapshot());

      SkAutoTUnref<SkFileMemoryChunkStream> stream(
          stream_provider->OpenStream());
      if (GenerateStyleFaceInfo(closest_style, stream)) {
        if (CharacterMapContainsCharacter(character)) {
          CreateSystemTypeface(closest_style, stream_provider);
          return true;
        } else {
          // If a typeface was not created, destroy the stream and purge any
          // newly created memory chunks. The stream must be destroyed first or
          // it will retain references to memory chunks, preventing them from
          // being purged.
          stream.reset(NULL);
          stream_provider->PurgeUnusedMemoryChunks();
          return false;
        }
      }

      styles_[style_index].swap(&styles_.back());
      styles_.pop_back();
    }

    is_character_map_generated_ = true;
  }

  return CharacterMapContainsCharacter(character);
}

bool SkFontStyleSet_Cobalt::CharacterMapContainsCharacter(SkUnichar character) {
  font_character_map::CharacterMap::iterator page_iterator =
      character_map_.find(font_character_map::GetPage(character));
  return page_iterator != character_map_.end() &&
         page_iterator->second.test(
             font_character_map::GetPageCharacterIndex(character));
}

bool SkFontStyleSet_Cobalt::GenerateStyleFaceInfo(
    SkFontStyleSetEntry_Cobalt* style, SkStreamAsset* stream) {
  if (style->is_face_info_generated) {
    return true;
  }

  TRACE_EVENT0("cobalt::renderer", "GenerateStyleFaceInfo()");

  FT_Library freetype_lib;
  if (FT_Init_FreeType(&freetype_lib) != 0) {
    return false;
  }

  FT_StreamRec streamRec;
  memset(&streamRec, 0, sizeof(streamRec));
  streamRec.size = stream->getLength();
  streamRec.descriptor.pointer = stream;
  streamRec.read = sk_cobalt_ft_stream_io;
  streamRec.close = sk_cobalt_ft_stream_close;

  FT_Open_Args args;
  memset(&args, 0, sizeof(args));
  args.flags = FT_OPEN_STREAM;
  args.stream = &streamRec;

  FT_Face face;
  FT_Error err = FT_Open_Face(freetype_lib, &args, style->face_index, &face);
  if (err) {
    FT_Done_FreeType(freetype_lib);
    return false;
  }

  int face_style = SkTypeface::kNormal;
  if (face->style_flags & FT_STYLE_FLAG_BOLD) {
    face_style |= SkTypeface::kBold;
  }
  if (face->style_flags & FT_STYLE_FLAG_ITALIC) {
    face_style |= SkTypeface::kItalic;
  }

  style->face_name.set(face->family_name);
  style->face_style = static_cast<SkTypeface::Style>(face_style);
  style->face_is_fixed_pitch = FT_IS_FIXED_WIDTH(face);
  style->is_face_info_generated = true;

  // Map out this family's characters if they haven't been generated yet.
  if (!is_character_map_generated_) {
    FT_UInt glyph_index;

    int last_page = -1;
    font_character_map::PageCharacters* page_characters = NULL;

    SkUnichar code_point = FT_Get_First_Char(face, &glyph_index);
    while (glyph_index) {
      int page = font_character_map::GetPage(code_point);
      if (page != last_page) {
        page_characters = &character_map_[page];
        last_page = page;
      }
      page_characters->set(
          font_character_map::GetPageCharacterIndex(code_point));

      code_point = FT_Get_Next_Char(face, code_point, &glyph_index);
    }

    is_character_map_generated_ = true;
  }

  // release this font.
  FT_Done_Face(face);

  // shut down FreeType.
  FT_Done_FreeType(freetype_lib);
  return true;
}

int SkFontStyleSet_Cobalt::GetClosestStyleIndex(const SkFontStyle& pattern) {
  int closest_index = 0;
  int min_score = std::numeric_limits<int>::max();
  for (int i = 0; i < styles_.count(); ++i) {
    int score = MatchScore(pattern, styles_[i]->font_style);
    if (score < min_score) {
      closest_index = i;
      min_score = score;
    }
  }
  return closest_index;
}

void SkFontStyleSet_Cobalt::CreateSystemTypeface(
    SkFontStyleSetEntry_Cobalt* style_entry,
    SkFileMemoryChunkStreamProvider* stream_provider /*=NULL*/) {
  TRACE_EVENT0("cobalt::renderer",
               "SkFontStyleSet_Cobalt::CreateSystemTypeface()");

  if (!stream_provider) {
    stream_provider = system_typeface_stream_manager_->GetStreamProvider(
        style_entry->font_file_path.c_str());
  }

  SkAutoTUnref<SkFileMemoryChunkStream> stream(stream_provider->OpenStream());
  if (GenerateStyleFaceInfo(style_entry, stream)) {
    LOG(ERROR) << "Scanned font from file: " << style_entry->face_name.c_str()
               << "(" << style_entry->face_style << ")";
    style_entry->typeface.reset(SkNEW_ARGS(
        SkTypeface_CobaltSystem,
        (stream_provider, style_entry->face_index, style_entry->face_style,
         style_entry->face_is_fixed_pitch, family_name_)));
  } else {
    LOG(ERROR) << "Failed to scan font: "
               << style_entry->font_file_path.c_str();
  }
}
