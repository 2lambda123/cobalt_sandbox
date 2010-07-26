// Copyright (c) 2006-2010 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_URL_REQUEST_VIEW_CACHE_HELPER_H_
#define NET_URL_REQUEST_VIEW_CACHE_HELPER_H_
#pragma once

#include <string>

#include "net/base/completion_callback.h"
#include "net/base/io_buffer.h"

class URLRequestContext;

namespace disk_cache {
class Backend;
class Entry;
}

namespace net {

class ViewCacheHelper {
 public:
  ViewCacheHelper()
      : disk_cache_(NULL), entry_(NULL), iter_(NULL), buf_len_(0), index_(0),
        data_(NULL), callback_(NULL), next_state_(STATE_NONE),
        ALLOW_THIS_IN_INITIALIZER_LIST(
            cache_callback_(this, &ViewCacheHelper::OnIOComplete)),
        ALLOW_THIS_IN_INITIALIZER_LIST(
            entry_callback_(new CancelableCompletionCallback<ViewCacheHelper>(
                this, &ViewCacheHelper::OnIOComplete))) {}
  ~ViewCacheHelper();

  // Formats the cache information for |key| as HTML. Returns a net error code.
  // If this method returns ERR_IO_PENDING, |callback| will be notified when the
  // operation completes. |out| must remain valid until this operation completes
  // or the object is destroyed.
  int GetEntryInfoHTML(const std::string& key, URLRequestContext* context,
                       std::string* out, CompletionCallback* callback);

  // Formats the cache contents as HTML. Returns a net error code.
  // If this method returns ERR_IO_PENDING, |callback| will be notified when the
  // operation completes. |out| must remain valid until this operation completes
  // or the object is destroyed. |url_prefix| will be prepended to each entry
  // key as a link to the entry.
  int GetContentsHTML(URLRequestContext* context, const std::string& url_prefix,
                      std::string* out, CompletionCallback* callback);

 private:
  enum State {
    STATE_NONE,
    STATE_GET_BACKEND,
    STATE_GET_BACKEND_COMPLETE,
    STATE_OPEN_NEXT_ENTRY,
    STATE_OPEN_NEXT_ENTRY_COMPLETE,
    STATE_OPEN_ENTRY,
    STATE_OPEN_ENTRY_COMPLETE,
    STATE_READ_RESPONSE,
    STATE_READ_RESPONSE_COMPLETE,
    STATE_READ_DATA,
    STATE_READ_DATA_COMPLETE
  };

  // Implements GetEntryInfoHTML and GetContentsHTML.
  int GetInfoHTML(const std::string& key, URLRequestContext* context,
                  const std::string& url_prefix, std::string* out,
                  CompletionCallback* callback);

  // This is a helper function used to trigger a completion callback. It may
  // only be called if callback_ is non-null.
  void DoCallback(int rv);

  // This will trigger the completion callback if appropriate.
  void HandleResult(int rv);

  // Runs the state transition loop.
  int DoLoop(int result);

  // Each of these methods corresponds to a State value. If there is an
  // argument, the value corresponds to the return of the previous state or
  // corresponding callback.
  int DoGetBackend();
  int DoGetBackendComplete(int result);
  int DoOpenNextEntry();
  int DoOpenNextEntryComplete(int result);
  int DoOpenEntry();
  int DoOpenEntryComplete(int result);
  int DoReadResponse();
  int DoReadResponseComplete(int result);
  int DoReadData();
  int DoReadDataComplete(int result);

  // Called to signal completion of asynchronous IO.
  void OnIOComplete(int result);

  scoped_refptr<URLRequestContext> context_;
  disk_cache::Backend* disk_cache_;
  disk_cache::Entry* entry_;
  void* iter_;
  scoped_refptr<net::IOBuffer> buf_;
  int buf_len_;
  int index_;

  std::string key_;
  std::string url_prefix_;
  std::string* data_;
  CompletionCallback* callback_;

  State next_state_;

  CompletionCallbackImpl<ViewCacheHelper> cache_callback_;
  scoped_refptr<CancelableCompletionCallback<ViewCacheHelper> > entry_callback_;

  DISALLOW_COPY_AND_ASSIGN(ViewCacheHelper);
};

}  // namespace net.

#endif  // NET_URL_REQUEST_VIEW_CACHE_HELPER_H_
