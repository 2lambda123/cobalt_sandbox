// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "net/base/network_delegate.h"

#include "base/logging.h"

namespace net {

int NetworkDelegate::NotifyBeforeURLRequest(URLRequest* request,
                                            CompletionCallback* callback) {
  DCHECK(CalledOnValidThread());
  DCHECK(request);
  DCHECK(callback);
  return OnBeforeURLRequest(request, callback);
}

int NetworkDelegate::NotifyBeforeSendHeaders(uint64 request_id,
                                             HttpRequestHeaders* headers,
                                             CompletionCallback* callback) {
  DCHECK(CalledOnValidThread());
  DCHECK(headers);
  DCHECK(callback);
  return OnBeforeSendHeaders(request_id, headers, callback);
}

void NetworkDelegate::NotifyResponseStarted(URLRequest* request) {
  DCHECK(CalledOnValidThread());
  DCHECK(request);
  OnResponseStarted(request);
}

void NetworkDelegate::NotifyReadCompleted(URLRequest* request, int bytes_read) {
  DCHECK(CalledOnValidThread());
  DCHECK(request);
  OnReadCompleted(request, bytes_read);
}

void NetworkDelegate::NotifyURLRequestDestroyed(URLRequest* request) {
  DCHECK(request);
  return OnURLRequestDestroyed(request);
}

URLRequestJob* NetworkDelegate::MaybeCreateURLRequestJob(URLRequest* request) {
  DCHECK(CalledOnValidThread());
  DCHECK(request);
  return OnMaybeCreateURLRequestJob(request);
}

}  // namespace net
