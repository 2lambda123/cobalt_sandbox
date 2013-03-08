/*
 * Copyright 2012 Google Inc. All Rights Reserved.
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

#ifndef MEDIA_FILTERS_SHELL_MP4_MAP_H_
#define MEDIA_FILTERS_SHELL_MP4_MAP_H_

#include <vector>

#include "base/callback.h"
#include "base/memory/ref_counted.h"
#include "media/base/shell_buffer_factory.h"
#include "media/base/shell_data_source_reader.h"

namespace media {

// per-atom sizes of individual table entries
static const int kEntrySize_co64 = 8;
static const int kEntrySize_ctts = 8;
static const int kEntrySize_stco = 4;
static const int kEntrySize_stts = 8;
static const int kEntrySize_stsc = 12;
static const int kEntrySize_stss = 4;
static const int kEntrySize_stsz = 4;

class ShellFilterGraphLog;

// Utility class to parse the various subatoms of the stbl mp4 atom and use
// them to provide byte offsets, sizes, and timestamps of a mp4 atom while
// reusing memory issued by ShellBufferFactory. The caching design benefits
// from, but does not require, sequential access in sample numbers.
class ShellMP4Map : public base::RefCountedThreadSafe<ShellMP4Map> {
 public:
  ShellMP4Map(scoped_refptr<ShellDataSourceReader> reader,
              scoped_refptr<ShellFilterGraphLog> filter_graph_log);

  bool IsComplete();

  // All Get() methods return true on success and save their values by
  // reference in the latter argument.
  bool GetSize(uint32 sample_number, uint32& size_out);
  bool GetOffset(uint32 sample_number, uint64& offset_out);
  // all time values in *ticks* as defined by the mp4 container
  bool GetTimestamp(uint32 sample_number, uint64& timestamp_out);
  bool GetDuration(uint32 sample_number, uint32& duration_out);
  bool GetIsKeyframe(uint32 sample_number, bool& is_keyframe_out);
  // Used to determine if the failure reported by any of the above methods
  // is due to EOS or other (fatal) error. The length of a mp4 file in samples
  // may not be known until iterating through almost the entire map, in the
  // case of a default sample size (rare in compressed media)
  bool IsEOS(uint32 sample_number);

  // Returns the keyframe sample number nearest the provided timestamp
  bool GetKeyframe(uint64 timestamp, uint32& sample_out);

  void SetAtom(uint32 four_cc, // fourCC code ascii code as big-endian uint32
               uint64 offset,  // offset of atom body in file
               uint64 size,    // total size of atom in bytes
               uint32 cache_size_entries,  // num of entries to cache in memory
               const uint8* atom);         // pointer to atom body start

 private:
  void co64_Init();

  void ctts_Init();
  // advance the ctts cache and integration state to contain sample number.
  bool ctts_AdvanceToSample(uint32 sample_number);
  bool ctts_SlipCacheToSample(uint32 sample_number, int starting_cache_index);

  void stco_Init();

  void stsc_Init();
  // advance the stsc cache and integration state to contain sample number.
  bool stsc_AdvanceToSample(uint32 sample_number);
  // re-use previously calculated sums to jump through the table to get to the
  // nearest cache entry that contains given sample number. Starts the search
  // from the starting_cache_index.
  bool stsc_SlipCacheToSample(uint32 sample_number, int starting_cache_index);

  // bool stss_FindNearestSample(uint32 sample_number);

  void stts_Init();
  bool stts_AdvanceToSample(uint32 sample_number);
  bool stts_SlipCacheToSample(uint32 sample_number, int starting_cache_index);
  /*
  bool stts_AdvanceToTime(uint64 timestamp);
  bool stts_SlipCacheToTime(uint64 timestamp, int starting_cache_index);
  */

  // TableCache manages the caching of each atom table in a separate instance.
  // As each atom has a different per-entry byte size, and may want different
  // caching behavior based on consumption rate of entries and the overall size
  // of the table, it allows each atom to use its own policy for caching.
  // To keep things relatively simple it always keep a table of size of the
  // minimum of cache_size_entries or entry_count in memory, and that cached
  // table is always aligned with the full table in cache_size_entries, so
  // that the position in the cache is trivially calculated from
  // entry_number % cache_size_entries, and the cache index is similarly
  // caculated from entry_number / cache_size_entries.
  class TableCache : public base::RefCountedThreadSafe<TableCache> {
   public:
    TableCache(uint64 table_offset, // byte offset of start of table in file
               uint32 entry_count,  // number of entries in table
               uint32 entry_size,   // size in bytes of each entry in table
               uint32 cache_size_entries,  // number of entries to cache in mem
               scoped_refptr<ShellDataSourceReader> reader,  // reader to use
               scoped_refptr<ShellFilterGraphLog> filter_graph_log);  // logger

    uint8* GetBytesAtEntry(uint32 entry_number);

    // how many entries total in the table?
    inline uint32 GetEntryCount() const { return entry_count_; }
    // how many entries are we caching in memory at once?
    inline uint32 GetCacheSizeEntries() const { return cache_size_entries_; }

   private:
    friend class base::RefCountedThreadSafe<TableCache>;
    uint32 entry_size_;           // size of entry in bytes
    uint32 entry_count_;          // size of table in entries
    uint32 cache_size_entries_;   // max number of entries to fit in memory
    uint64 table_offset_;         // offset of table in stream
    scoped_refptr<ShellDataSourceReader> reader_;  // means to read more table
    scoped_refptr<ShellFilterGraphLog> filter_graph_log_;

    // current cache state
    scoped_refptr<ShellScopedArray> cache_;  // the cached part of the table
    uint32 cache_first_entry_number_;    // first table entry number in cache
    uint32 cache_entry_count_;           // number of valid entries in cache
  };

  scoped_refptr<ShellDataSourceReader> reader_;
  scoped_refptr<ShellFilterGraphLog> filter_graph_log_;

  // current integration state for GetOffset(), we save the sum of sample sizes
  // within the current chunk.
  uint32 current_chunk_sample_;  // sample number last included in summation
  uint32 next_chunk_sample_;     // first sample number of next chunk
  uint64 current_chunk_offset_;  // file byte offset of current_chunk_sample_

  // Can be set by a stsz entry count but an stsz may provide a default size,
  // in which case this number may not be known until itegration through
  // the ctts, or stts has completed. In the event that one of those tables
  // ends at a lower number than the others this number will be amended
  // to return the lower number.
  uint32 highest_valid_sample_number_;

  // ==== c064 - per-chunk list of file offsets (64-bit)
  scoped_refptr<TableCache> co64_;

  // ==== ctts - run-length sample number to composition time offset
  scoped_refptr<TableCache> ctts_;
  uint32 ctts_first_sample_;
  uint32 ctts_sample_offset_;
  uint32 ctts_next_first_sample_;
  uint32 ctts_table_index_;
  std::vector<uint32> ctts_samples_;

  // ==== stco - per-chunk list of chunk file offsets (32-bit)
  scoped_refptr<TableCache> stco_;

  // ==== stsc - chunk-number to samples-per-chunk
  scoped_refptr<TableCache> stsc_;
  uint32 stsc_first_chunk_;  // first chunk of the current sample size range
  uint32 stsc_first_chunk_sample_; // sum of samples of all prev chunk ranges
  uint32 stsc_samples_per_chunk_;  // current samples-per-chunk in this range
  uint32 stsc_next_first_chunk_;  // the chunk number the next region begins in
  uint32 stsc_next_first_chunk_sample_;  // sample number next region begins in
  uint32 stsc_table_index_;  // the index in the table of the current
  std::vector<uint32> stsc_sample_sums_;  // saved sums of cache segments

  // ==== stss - list of keyframe sample numbers
  scoped_refptr<TableCache> stss_;

  // ==== stts - run-length sample number to time duration
  scoped_refptr<TableCache> stts_;
  uint32 stts_first_sample_;  // first sample of the current duration range
  uint64 stts_first_sample_time_;  // sum of all durations of previous ranges
  uint32 stts_sample_duration_;  // current duration of samples in this range
  uint32 stts_next_first_sample_;  // first sample number of next range
  uint32 stts_table_index_;  // index in the table of the next entry
  std::vector<uint32> stts_samples_;
  std::vector<uint64> stts_timestamps_;

  // ==== stsz - per-sample list of sample sizes
  scoped_refptr<TableCache> stsz_;
  uint32 stsz_default_size_;
};

}  // namespace media

#endif  // MEDIA_FILTERS_SHELL_MP4_MAP_H_
