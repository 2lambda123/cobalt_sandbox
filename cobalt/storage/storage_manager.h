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

#ifndef COBALT_STORAGE_STORAGE_MANAGER_H_
#define COBALT_STORAGE_STORAGE_MANAGER_H_

#include <vector>

#include "base/callback.h"
#include "base/memory/scoped_ptr.h"
#include "base/message_loop.h"
#include "base/synchronization/waitable_event.h"
#include "base/threading/thread.h"
#include "base/threading/thread_checker.h"
#include "base/timer.h"
#include "cobalt/storage/savegame_thread.h"
#include "cobalt/storage/sql_vfs.h"
#include "cobalt/storage/virtual_file_system.h"
#include "sql/connection.h"

namespace cobalt {
namespace storage {
class SqlContext;

// StorageManager manages a SQLite database containing cookies and local
// storage data. On most platforms, this is written to disk as a savegame
// using platform APIs. On Linux/Windows, it's a regular file.
// Internally this runs two threads: one thread to perform blocking I/O,
// and one where SQL operations occur.  Users are expected to access the
// database via an SqlContext, which can be obtained with GetSqlContext().
// The callback to GetSqlCallback will run on the SQL thread.
// Operations on SqlContext will block the SQL thread until the savegame
// is loaded.
class StorageManager {
 public:
  struct Options {
    Savegame::Options savegame_options;
  };

  typedef base::Callback<void(SqlContext*)> SqlCallback;

  // Database version "2" indicates that this was created by Steel v1.x.
  // Database version "0" indicates that this was created by v2.x beta,
  //   patches 0-3.  Version "0" is a default from sqlite3 because these
  //   versions of the application did not set this value at all.
  // Database version "3" indicates that the schema versions of individual
  //   tables should be tracked in SchemaTable.
  static const int kDatabaseUserVersion = 3;

  // Schema-related error codes.  See GetSchemaVersion().
  enum {
    kSchemaTableIsNew = -1,
    kSchemaVersionLost = -2,
  };

  explicit StorageManager(const Options& options);
  virtual ~StorageManager();

  // Obtain the SqlContext for our database.
  // |callback| will be called with an SqlContext that can be used to operate on
  // the database. The callback will run on the storage manager's message loop.
  void GetSqlContext(const SqlCallback& callback);

  // Schedule a write of our database to disk to happen at some point in the
  // future. Multiple calls to Flush() do not necessarily result in multiple
  // writes to disk.
  // This call returns immediately.
  void Flush();

  // Triggers a write to disk to happen immediately.  Each call to FlushNow()
  // will result in a write to disk.
  // |callback|, if provided, will be called when the I/O has completed,
  // and will be run on the storage manager's IO thread.
  // This call returns immediately.
  void FlushNow(const base::Closure& callback);

  const Options& options() const { return options_; }

 protected:
  // Queues a flush to be executed as soon as possible.  As soon as possible
  // will be as soon as any existing flush completes, or right away if no
  // existing flush is happening.  Note that it is protected and virtual for
  // white box testing purposes.
  virtual void QueueFlush(const base::Closure& callback);

 private:
  // SqlContext needs access to our internal APIs.
  friend class SqlContext;
  // Give StorageManagerTest access, so we can more easily test some internals.
  friend class StorageManagerTest;

  // Flushes all queued flushes to the savegame thread.
  virtual void FlushInternal();

  // Initialize the SQLite database. This blocks until the savegame load is
  // complete.
  void FinishInit();

  // Callback when flush timer has elapsed.
  void OnFlushTimerFired();

  // Logic to be executed on the SQL thread when a flush completes.  Will
  // dispatch |flush_processing_callbacks_| callbacks and execute a new flush
  // if |flush_requested_| is true.
  void OnFlushIOCompletedSQLCallback();

  // This function will not return until all queued I/O is completed.  Since
  // it will require the SQL message loop to process, it must be called from
  // outside the SQL message loop (such as from StorageManager's destructor).
  void FinishIO();

  // Called by the destructor, to ensure we destroy certain objects on the
  // sql thread.
  void OnDestroy();

  // Internal API for use by SqlContext.
  sql::Connection* sql_connection();
  bool GetSchemaVersion(const char* table_name, int* schema_version);
  void UpdateSchemaVersion(const char* table_name, int version);

  // Configuration options for the Storage Manager.
  Options options_;

  // Storage manager runs on its own thread. This is where SQL database
  // operations are done.
  scoped_ptr<base::Thread> sql_thread_;
  scoped_refptr<base::MessageLoopProxy> sql_message_loop_;

  // An interface to the storage manager's SQL database that will run on
  // the correct thread.
  scoped_ptr<SqlContext> sql_context_;

  // The in-memory database connection.
  scoped_ptr<sql::Connection> connection_;

  // Virtual file system that contains our in-memory SQLite database.
  scoped_ptr<VirtualFileSystem> vfs_;

  // An interface between Sqlite and VirtualFileSystem.
  scoped_ptr<SqlVfs> sql_vfs_;

  // When the savegame is loaded at startup, we keep the raw data around
  // until we can initialize the database on the correct thread.
  scoped_ptr<Savegame::ByteVector> loaded_raw_bytes_;

  // Timer that starts running when Flush() is called. When the time elapses,
  // we actually perform the write. This is a simple form of rate limiting
  // for I/O writes.
  scoped_ptr<base::OneShotTimer<StorageManager> > flush_timer_;

  // See comments for for kDatabaseUserVersion.
  int loaded_database_version_;
  // false until the SQL database is fully configured.
  bool initialized_;

  // True if a flush is currently being processed on the storage message loop.
  // In this case, we should not issue more flushes, but instead set
  // |flush_requested_| to true to ensure that a new flush is submitted as
  // soon as we are done processing the current one.
  bool flush_processing_;

  // The queue of callbacks that are should be called when the current flush
  // completes.  If this is not empty, then |flush_processing_| must be true.
  std::vector<base::Closure> flush_processing_callbacks_;

  // True if |flush_processing_| is true, but we would like to perform a new
  // flush as soon as it completes.
  bool flush_requested_;

  // The queue of callbacks that will be called when the flush that follows
  // the current flush completes.  If this is non-empty, then |flush_requested_|
  // must be true.
  std::vector<base::Closure> flush_requested_callbacks_;

  base::WaitableEvent no_flushes_pending_;

  // An object that wraps Savegame inside of an I/O thread so that we can
  // flush data asynchronously.
  scoped_ptr<SavegameThread> savegame_thread_;

  DISALLOW_COPY_AND_ASSIGN(StorageManager);
};

// Proxy for accessing StorageManager's SQL database in a thread-safe way.
// All access to the StorageManager's database should be done via an SqlContext.
// Only the StorageManager can create this class.
class SqlContext {
 public:
  sql::Connection* sql_connection() const {
    return storage_manager_->sql_connection();
  }

  // Get the schema version for the given table.
  // Returns false if the table does not exist, otherwise returns true
  // and writes the version number to the schema_version pointer.
  // schema_version will be set to kSchemaTableIsNew if the table exists,
  // but the schema table was newly created in this session.
  // schema_version will be set to kSchemaVersionLost if the table exists,
  // and the schema table existed once before, but has since been lost.
  // In this case, the schema version cannot be known or directly inferred.
  bool GetSchemaVersion(const char* table_name, int* schema_version) {
    return storage_manager_->GetSchemaVersion(table_name, schema_version);
  }

  // Updates the schema version for the given table.
  // The version number must be greater than 0.
  void UpdateSchemaVersion(const char* table_name, int version) {
    return storage_manager_->UpdateSchemaVersion(table_name, version);
  }

  void Flush() { storage_manager_->Flush(); }

  void FlushNow(const base::Closure& callback) {
    storage_manager_->FlushNow(callback);
  }

 private:
  StorageManager* storage_manager_;

  explicit SqlContext(StorageManager* storage_manager)
      : storage_manager_(storage_manager) {}

  friend StorageManager::StorageManager(const Options& options);
  DISALLOW_COPY_AND_ASSIGN(SqlContext);
};

}  // namespace storage
}  // namespace cobalt

#endif  // COBALT_STORAGE_STORAGE_MANAGER_H_
