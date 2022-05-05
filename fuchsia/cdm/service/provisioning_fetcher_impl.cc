// Copyright 2020 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "media/fuchsia/cdm/service/provisioning_fetcher_impl.h"

#include "base/bind.h"
#include "base/callback.h"
#include "base/fuchsia/fuchsia_logging.h"
#include "base/fuchsia/mem_buffer_util.h"
#include "base/logging.h"

namespace media {

ProvisioningFetcherImpl::ProvisioningFetcherImpl(
    CreateFetcherCB create_fetcher_callback)
    : create_fetcher_callback_(std::move(create_fetcher_callback)),
      binding_(this) {
  DCHECK(create_fetcher_callback_);
}

ProvisioningFetcherImpl::~ProvisioningFetcherImpl() = default;

fidl::InterfaceHandle<fuchsia::media::drm::ProvisioningFetcher>
ProvisioningFetcherImpl::Bind(base::OnceClosure error_callback) {
  error_callback_ = std::move(error_callback);

  binding_.set_error_handler(
      [&error_callback = error_callback_](zx_status_t status) {
        ZX_DLOG_IF(ERROR, (status != ZX_OK) && (status != ZX_ERR_PEER_CLOSED),
                   status)
            << "ProvisioningFetcher closed with an unexpected status";
        std::move(error_callback).Run();
      });
  return binding_.NewBinding();
}

void ProvisioningFetcherImpl::Fetch(
    fuchsia::media::drm::ProvisioningRequest request,
    FetchCallback callback) {
  if (retrieve_in_progress_) {
    DLOG(WARNING)
        << "ProvisioningFetcherImpl received too many ProvisioningRequests";
    OnError(ZX_ERR_BAD_STATE);
    return;
  }

  absl::optional<std::string> request_str =
      base::StringFromMemBuffer(request.message);
  if (!request_str) {
    DLOG(WARNING) << "Failed to read ProvisioningRequest.";
    OnError(ZX_ERR_INVALID_ARGS);
    return;
  }

  if (!request.default_provisioning_server_url) {
    DLOG(WARNING) << "Missing default provisioning server URL.";
    OnError(ZX_ERR_INVALID_ARGS);
    return;
  }

  if (!fetcher_) {
    fetcher_ = create_fetcher_callback_.Run();
  }

  retrieve_in_progress_ = true;
  fetcher_->Retrieve(
      GURL(request.default_provisioning_server_url.value()), *request_str,
      base::BindRepeating(&ProvisioningFetcherImpl::OnRetrieveComplete,
                          base::Unretained(this),
                          base::Passed(std::move(callback))));
}

void ProvisioningFetcherImpl::OnRetrieveComplete(FetchCallback callback,
                                                 bool success,
                                                 const std::string& response) {
  retrieve_in_progress_ = false;
  // Regardless of success or failure, send the response back to acknowledge
  // it has been completed.
  DLOG_IF(WARNING, !success) << "Failed to fetch provision response.";

  fuchsia::media::drm::ProvisioningResponse provision_response;
  provision_response.message =
      base::MemBufferFromString(response, "cr-drm-provision-response");

  callback(std::move(provision_response));
}

void ProvisioningFetcherImpl::OnError(zx_status_t status) {
  DCHECK(error_callback_);

  binding_.Close(status);
  std::move(error_callback_).Run();
}

}  // namespace media
