// Copyright (c) 2006-2010 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_DISK_CACHE_ENTRY_IMPL_H_
#define NET_DISK_CACHE_ENTRY_IMPL_H_
#pragma once

#include "base/scoped_ptr.h"
#include "net/disk_cache/disk_cache.h"
#include "net/disk_cache/storage_block.h"
#include "net/disk_cache/storage_block-inl.h"

namespace disk_cache {

class BackendImpl;
class SparseControl;

// This class implements the Entry interface. An object of this
// class represents a single entry on the cache.
class EntryImpl : public Entry, public base::RefCounted<EntryImpl> {
  friend class base::RefCounted<EntryImpl>;
  friend class SparseControl;
 public:
  enum Operation {
    kRead,
    kWrite,
    kSparseRead,
    kSparseWrite,
    kAsyncIO
  };

  EntryImpl(BackendImpl* backend, Addr address, bool read_only);

  // Entry interface.
  virtual void Doom();
  virtual void Close();
  virtual std::string GetKey() const;
  virtual base::Time GetLastUsed() const;
  virtual base::Time GetLastModified() const;
  virtual int32 GetDataSize(int index) const;
  virtual int ReadData(int index, int offset, net::IOBuffer* buf, int buf_len,
                       net::CompletionCallback* completion_callback);
  virtual int WriteData(int index, int offset, net::IOBuffer* buf, int buf_len,
                        net::CompletionCallback* completion_callback,
                        bool truncate);
  virtual int ReadSparseData(int64 offset, net::IOBuffer* buf, int buf_len,
                             net::CompletionCallback* completion_callback);
  virtual int WriteSparseData(int64 offset, net::IOBuffer* buf, int buf_len,
                              net::CompletionCallback* completion_callback);
  virtual int GetAvailableRange(int64 offset, int len, int64* start,
                                CompletionCallback* callback);
  virtual bool CouldBeSparse() const;
  virtual void CancelSparseIO();
  virtual int ReadyForSparseIO(net::CompletionCallback* completion_callback);

  // Background implementation of the Entry interface.
  void DoomImpl();
  int ReadDataImpl(int index, int offset, net::IOBuffer* buf, int buf_len,
                   CompletionCallback* callback);
  int WriteDataImpl(int index, int offset, net::IOBuffer* buf, int buf_len,
                    CompletionCallback* callback, bool truncate);
  int ReadSparseDataImpl(int64 offset, net::IOBuffer* buf, int buf_len,
                         CompletionCallback* callback);
  int WriteSparseDataImpl(int64 offset, net::IOBuffer* buf, int buf_len,
                          CompletionCallback* callback);
  int GetAvailableRangeImpl(int64 offset, int len, int64* start);
  void CancelSparseIOImpl();
  int ReadyForSparseIOImpl(CompletionCallback* callback);

  inline CacheEntryBlock* entry() {
    return &entry_;
  }

  inline CacheRankingsBlock* rankings() {
    return &node_;
  }

  uint32 GetHash();

  // Performs the initialization of a EntryImpl that will be added to the
  // cache.
  bool CreateEntry(Addr node_address, const std::string& key, uint32 hash);

  // Returns true if this entry matches the lookup arguments.
  bool IsSameEntry(const std::string& key, uint32 hash);

  // Permamently destroys this entry.
  void InternalDoom();

  // Deletes this entry from disk. If |everything| is false, only the user data
  // will be removed, leaving the key and control data intact.
  void DeleteEntryData(bool everything);

  // Returns the address of the next entry on the list of entries with the same
  // hash.
  CacheAddr GetNextAddress();

  // Sets the address of the next entry on the list of entries with the same
  // hash.
  void SetNextAddress(Addr address);

  // Reloads the rankings node information.
  bool LoadNodeAddress();

  // Updates the stored data to reflect the run-time information for this entry.
  // Returns false if the data could not be updated. The purpose of this method
  // is to be able to detect entries that are currently in use.
  bool Update();

  // Returns true if this entry is marked as dirty on disk.
  bool IsDirty(int32 current_id);
  void ClearDirtyFlag();

  // Fixes this entry so it can be treated as valid (to delete it).
  void SetPointerForInvalidEntry(int32 new_id);

  // Returns false if the entry is clearly invalid.
  bool SanityCheck();

  // Handle the pending asynchronous IO count.
  void IncrementIoCount();
  void DecrementIoCount();

  // Set the access times for this entry. This method provides support for
  // the upgrade tool.
  void SetTimes(base::Time last_used, base::Time last_modified);

  // Generates a histogram for the time spent working on this operation.
  void ReportIOTime(Operation op, const base::TimeTicks& start);

 private:
  enum {
     kNumStreams = 3
  };
  class UserBuffer;

  ~EntryImpl();

  // Initializes the storage for an internal or external data block.
  bool CreateDataBlock(int index, int size);

  // Initializes the storage for an internal or external generic block.
  bool CreateBlock(int size, Addr* address);

  // Deletes the data pointed by address, maybe backed by files_[index].
  void DeleteData(Addr address, int index);

  // Updates ranking information.
  void UpdateRank(bool modified);

  // Returns a pointer to the file that stores the given address.
  File* GetBackingFile(Addr address, int index);

  // Returns a pointer to the file that stores external data.
  File* GetExternalFile(Addr address, int index);

  // Prepares the target file or buffer for a write of buf_len bytes at the
  // given offset.
  bool PrepareTarget(int index, int offset, int buf_len, bool truncate);

  // Adjusts the internal buffer and file handle for a write that truncates this
  // stream.
  bool HandleTruncation(int index, int offset, int buf_len);

  // Copies data from disk to the internal buffer.
  bool CopyToLocalBuffer(int index);

  // Reads from a block data file to this object's memory buffer.
  bool MoveToLocalBuffer(int index);

  // Loads the external file to this object's memory buffer.
  bool ImportSeparateFile(int index, int new_size);

  // Makes sure that the internal buffer can handle the a write of |buf_len|
  // bytes to |offset|.
  bool PrepareBuffer(int index, int offset, int buf_len);

  // Flushes the in-memory data to the backing storage.
  bool Flush(int index);

  // Updates the size of a given data stream.
  void UpdateSize(int index, int old_size, int new_size);

  // Initializes the sparse control object. Returns a net error code.
  int InitSparseData();

  // Adds the provided |flags| to the current EntryFlags for this entry.
  void SetEntryFlags(uint32 flags);

  // Returns the current EntryFlags for this entry.
  uint32 GetEntryFlags();

  // Gets the data stored at the given index. If the information is in memory,
  // a buffer will be allocated and the data will be copied to it (the caller
  // can find out the size of the buffer before making this call). Otherwise,
  // the cache address of the data will be returned, and that address will be
  // removed from the regular book keeping of this entry so the caller is
  // responsible for deleting the block (or file) from the backing store at some
  // point; there is no need to report any storage-size change, only to do the
  // actual cleanup.
  void GetData(int index, char** buffer, Addr* address);

  // Logs this entry to the internal trace buffer.
  void Log(const char* msg);

  CacheEntryBlock entry_;     // Key related information for this entry.
  CacheRankingsBlock node_;   // Rankings related information for this entry.
  BackendImpl* backend_;      // Back pointer to the cache.
  scoped_ptr<UserBuffer> user_buffers_[kNumStreams];  // Stores user data.
  // Files to store external user data and key.
  scoped_refptr<File> files_[kNumStreams + 1];
  mutable std::string key_;           // Copy of the key.
  int unreported_size_[kNumStreams];  // Bytes not reported yet to the backend.
  bool doomed_;               // True if this entry was removed from the cache.
  bool read_only_;            // True if not yet writing.
  scoped_ptr<SparseControl> sparse_;  // Support for sparse entries.

  DISALLOW_COPY_AND_ASSIGN(EntryImpl);
};

}  // namespace disk_cache

#endif  // NET_DISK_CACHE_ENTRY_IMPL_H_
