// Copyright (c) 2006-2009 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_BASE_SSL_CONFIG_SERVICE_H_
#define NET_BASE_SSL_CONFIG_SERVICE_H_

#include <vector>

#include "base/ref_counted.h"
#include "net/base/x509_certificate.h"

namespace net {

// A collection of SSL-related configuration settings.
struct SSLConfig {
  // Default to no revocation checking.
  // Default to SSL 2.0 off, SSL 3.0 on, and TLS 1.0 on.
  SSLConfig()
      : rev_checking_enabled(false),  ssl2_enabled(false), ssl3_enabled(true),
        tls1_enabled(true), send_client_cert(false), verify_ev_cert(false) {
  }

  bool rev_checking_enabled;  // True if server certificate revocation
                              // checking is enabled.
  bool ssl2_enabled;  // True if SSL 2.0 is enabled.
  bool ssl3_enabled;  // True if SSL 3.0 is enabled.
  bool tls1_enabled;  // True if TLS 1.0 is enabled.

  // TODO(wtc): move the following members to a new SSLParams structure.  They
  // are not SSL configuration settings.

  struct CertAndStatus {
    scoped_refptr<X509Certificate> cert;
    int cert_status;
  };

  // Returns true if |cert| is one of the certs in |allowed_bad_certs|.
  // TODO(wtc): Move this to a .cc file.  ssl_config_service.cc is Windows
  // only right now, so I can't move it there.
  bool IsAllowedBadCert(X509Certificate* cert) const {
    for (size_t i = 0; i < allowed_bad_certs.size(); ++i) {
      if (cert == allowed_bad_certs[i].cert)
        return true;
    }
    return false;
  }

  // Add any known-bad SSL certificate (with its cert status) to
  // |allowed_bad_certs| that should not trigger an ERR_CERT_* error when
  // calling SSLClientSocket::Connect.  This would normally be done in
  // response to the user explicitly accepting the bad certificate.
  std::vector<CertAndStatus> allowed_bad_certs;

  // True if we should send client_cert to the server.
  bool send_client_cert;

  bool verify_ev_cert;  // True if we should verify the certificate for EV.

  scoped_refptr<X509Certificate> client_cert;
};

// The interface for retrieving the system SSL configuration.  This interface
// does not cover setting the SSL configuration, as on some systems, the
// SSLConfigService objects may not have direct access to the configuration, or
// live longer than the configuration preferences.
class SSLConfigService : public base::RefCountedThreadSafe<SSLConfigService> {
 public:
  virtual ~SSLConfigService() {}

  // May not be thread-safe, should only be called on the IO thread.
  virtual void GetSSLConfig(SSLConfig* config) = 0;
};

}  // namespace net

#endif  // NET_BASE_SSL_CONFIG_SERVICE_H_
