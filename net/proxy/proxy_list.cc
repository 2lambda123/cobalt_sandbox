// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "net/proxy/proxy_list.h"

#include "base/logging.h"
#include "base/string_tokenizer.h"
#include "base/time.h"
#include "net/proxy/proxy_server.h"

using base::TimeDelta;
using base::TimeTicks;

namespace net {

ProxyList::ProxyList() {
}

ProxyList::~ProxyList() {
}

void ProxyList::Set(const std::string& proxy_uri_list) {
  proxies_.clear();
  StringTokenizer str_tok(proxy_uri_list, ";");
  while (str_tok.GetNext()) {
    ProxyServer uri = ProxyServer::FromURI(
        str_tok.token_begin(), str_tok.token_end(), ProxyServer::SCHEME_HTTP);
    // Silently discard malformed inputs.
    if (uri.is_valid())
      proxies_.push_back(uri);
  }
}

void ProxyList::SetSingleProxyServer(const ProxyServer& proxy_server) {
  proxies_.clear();
  if (proxy_server.is_valid())
    proxies_.push_back(proxy_server);
}

void ProxyList::DeprioritizeBadProxies(
    const ProxyRetryInfoMap& proxy_retry_info) {
  // Partition the proxy list in two:
  //   (1) the known bad proxies
  //   (2) everything else
  std::vector<ProxyServer> good_proxies;
  std::vector<ProxyServer> bad_proxies;

  std::vector<ProxyServer>::const_iterator iter = proxies_.begin();
  for (; iter != proxies_.end(); ++iter) {
    ProxyRetryInfoMap::const_iterator bad_proxy =
        proxy_retry_info.find(iter->ToURI());
    if (bad_proxy != proxy_retry_info.end()) {
      // This proxy is bad. Check if it's time to retry.
      if (bad_proxy->second.bad_until >= TimeTicks::Now()) {
        // still invalid.
        bad_proxies.push_back(*iter);
        continue;
      }
    }
    good_proxies.push_back(*iter);
  }

  // "proxies_ = good_proxies + bad_proxies"
  proxies_.swap(good_proxies);
  proxies_.insert(proxies_.end(), bad_proxies.begin(), bad_proxies.end());
}

void ProxyList::RemoveProxiesWithoutScheme(int scheme_bit_field) {
  for (std::vector<ProxyServer>::iterator it = proxies_.begin();
       it != proxies_.end(); ) {
    if (!(scheme_bit_field & it->scheme())) {
      it = proxies_.erase(it);
      continue;
    }
    ++it;
  }
}

void ProxyList::Clear() {
  proxies_.clear();
}

bool ProxyList::IsEmpty() const {
  return proxies_.empty();
}

size_t ProxyList::size() const {
  return proxies_.size();
}

const ProxyServer& ProxyList::Get() const {
  DCHECK(!proxies_.empty());
  return proxies_[0];
}

void ProxyList::SetFromPacString(const std::string& pac_string) {
  StringTokenizer entry_tok(pac_string, ";");
  proxies_.clear();
  while (entry_tok.GetNext()) {
    ProxyServer uri = ProxyServer::FromPacString(
        entry_tok.token_begin(), entry_tok.token_end());
    // Silently discard malformed inputs.
    if (uri.is_valid())
      proxies_.push_back(uri);
  }

  // If we failed to parse anything from the PAC results list, fallback to
  // DIRECT (this basically means an error in the PAC script).
  if (proxies_.empty()) {
    proxies_.push_back(ProxyServer::Direct());
  }
}

std::string ProxyList::ToPacString() const {
  std::string proxy_list;
  std::vector<ProxyServer>::const_iterator iter = proxies_.begin();
  for (; iter != proxies_.end(); ++iter) {
    if (!proxy_list.empty())
      proxy_list += ";";
    proxy_list += iter->ToPacString();
  }
  return proxy_list.empty() ? std::string() : proxy_list;
}

bool ProxyList::Fallback(ProxyRetryInfoMap* proxy_retry_info,
                         const BoundNetLog& net_log) {
  // Number of minutes to wait before retrying a bad proxy server.
  const TimeDelta kProxyRetryDelay = TimeDelta::FromMinutes(5);

  // TODO(eroman): It would be good if instead of removing failed proxies
  // from the list, we simply annotated them with the error code they failed
  // with. Of course, ProxyService::ReconsiderProxyAfterError() would need to
  // be given this information by the network transaction.
  //
  // The advantage of this approach is when the network transaction
  // fails, we could output the full list of proxies that were attempted, and
  // why each one of those failed (as opposed to just the last failure).
  //
  // And also, before failing the transaction wholesale, we could go back and
  // retry the "bad proxies" which we never tried to begin with.
  // (RemoveBadProxies would annotate them as 'expected bad' rather then delete
  // them from the list, so we would know what they were).

  if (proxies_.empty()) {
    NOTREACHED();
    return false;
  }

  if (!proxies_[0].is_direct()) {
    std::string key = proxies_[0].ToURI();
    // Mark this proxy as bad.
    ProxyRetryInfoMap::iterator iter = proxy_retry_info->find(key);
    if (iter != proxy_retry_info->end()) {
      // TODO(nsylvain): This is not the first time we get this. We should
      // double the retry time. Bug 997660.
      iter->second.bad_until = TimeTicks::Now() + iter->second.current_delay;
    } else {
      ProxyRetryInfo retry_info;
      retry_info.current_delay = kProxyRetryDelay;
      retry_info.bad_until = TimeTicks().Now() + retry_info.current_delay;
      (*proxy_retry_info)[key] = retry_info;
    }
    net_log.AddEvent(
        NetLog::TYPE_PROXY_LIST_FALLBACK,
        make_scoped_refptr(new NetLogStringParameter("bad_proxy", key)));
  }

  // Remove this proxy from our list.
  proxies_.erase(proxies_.begin());

  return !proxies_.empty();
}

}  // namespace net
