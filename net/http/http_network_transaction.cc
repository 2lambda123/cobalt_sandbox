// Copyright (c) 2010 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "net/http/http_network_transaction.h"

#include "base/compiler_specific.h"
#include "base/field_trial.h"
#include "base/format_macros.h"
#include "base/histogram.h"
#include "base/scoped_ptr.h"
#include "base/stats_counters.h"
#include "base/stl_util-inl.h"
#include "base/string_util.h"
#include "base/values.h"
#include "build/build_config.h"
#include "googleurl/src/gurl.h"
#include "net/base/connection_type_histograms.h"
#include "net/base/host_mapping_rules.h"
#include "net/base/io_buffer.h"
#include "net/base/load_flags.h"
#include "net/base/net_errors.h"
#include "net/base/net_util.h"
#include "net/base/ssl_cert_request_info.h"
#include "net/base/upload_data_stream.h"
#include "net/http/http_auth.h"
#include "net/http/http_auth_handler.h"
#include "net/http/http_auth_handler_factory.h"
#include "net/http/http_basic_stream.h"
#include "net/http/http_chunked_decoder.h"
#include "net/http/http_network_session.h"
#include "net/http/http_request_headers.h"
#include "net/http/http_request_info.h"
#include "net/http/http_response_headers.h"
#include "net/http/http_response_info.h"
#include "net/http/http_util.h"
#include "net/http/url_security_manager.h"
#include "net/socket/client_socket_factory.h"
#include "net/socket/socks_client_socket_pool.h"
#include "net/socket/ssl_client_socket.h"
#include "net/socket/tcp_client_socket_pool.h"
#include "net/spdy/spdy_http_stream.h"
#include "net/spdy/spdy_session.h"
#include "net/spdy/spdy_session_pool.h"

using base::Time;

namespace net {

namespace {

const HostMappingRules* g_host_mapping_rules = NULL;
const std::string* g_next_protos = NULL;
bool g_use_alternate_protocols = false;

// A set of host:port strings. These are servers which we have needed to back
// off to SSLv3 for.
std::set<std::string>* g_tls_intolerant_servers = NULL;

void BuildRequestHeaders(const HttpRequestInfo* request_info,
                         const HttpRequestHeaders& authorization_headers,
                         const UploadDataStream* upload_data_stream,
                         bool using_proxy,
                         std::string* request_line,
                         HttpRequestHeaders* request_headers) {
  const std::string path = using_proxy ?
                           HttpUtil::SpecForRequest(request_info->url) :
                           HttpUtil::PathForRequest(request_info->url);
  *request_line = StringPrintf(
      "%s %s HTTP/1.1\r\n", request_info->method.c_str(), path.c_str());
  request_headers->SetHeader(HttpRequestHeaders::kHost,
                             GetHostAndOptionalPort(request_info->url));

  // For compat with HTTP/1.0 servers and proxies:
  if (using_proxy) {
    request_headers->SetHeader(HttpRequestHeaders::kProxyConnection,
                               "keep-alive");
  } else {
    request_headers->SetHeader(HttpRequestHeaders::kConnection, "keep-alive");
  }

  // Our consumer should have made sure that this is a safe referrer.  See for
  // instance WebCore::FrameLoader::HideReferrer.
  if (request_info->referrer.is_valid()) {
    request_headers->SetHeader(HttpRequestHeaders::kReferer,
                               request_info->referrer.spec());
  }

  // Add a content length header?
  if (upload_data_stream) {
    request_headers->SetHeader(
        HttpRequestHeaders::kContentLength,
        Uint64ToString(upload_data_stream->size()));
  } else if (request_info->method == "POST" || request_info->method == "PUT" ||
             request_info->method == "HEAD") {
    // An empty POST/PUT request still needs a content length.  As for HEAD,
    // IE and Safari also add a content length header.  Presumably it is to
    // support sending a HEAD request to an URL that only expects to be sent a
    // POST or some other method that normally would have a message body.
    request_headers->SetHeader(HttpRequestHeaders::kContentLength, "0");
  }

  // Honor load flags that impact proxy caches.
  if (request_info->load_flags & LOAD_BYPASS_CACHE) {
    request_headers->SetHeader(HttpRequestHeaders::kPragma, "no-cache");
    request_headers->SetHeader(HttpRequestHeaders::kCacheControl, "no-cache");
  } else if (request_info->load_flags & LOAD_VALIDATE_CACHE) {
    request_headers->SetHeader(HttpRequestHeaders::kCacheControl, "max-age=0");
  }

  request_headers->MergeFrom(authorization_headers);

  // Headers that will be stripped from request_info->extra_headers to prevent,
  // e.g., plugins from overriding headers that are controlled using other
  // means. Otherwise a plugin could set a referrer although sending the
  // referrer is inhibited.
  // TODO(jochen): check whether also other headers should be stripped.
  static const char* const kExtraHeadersToBeStripped[] = {
    "Referer"
  };

  HttpRequestHeaders stripped_extra_headers;
  stripped_extra_headers.CopyFrom(request_info->extra_headers);
  for (size_t i = 0; i < arraysize(kExtraHeadersToBeStripped); ++i)
    stripped_extra_headers.RemoveHeader(kExtraHeadersToBeStripped[i]);
  request_headers->MergeFrom(stripped_extra_headers);
}

// The HTTP CONNECT method for establishing a tunnel connection is documented
// in draft-luotonen-web-proxy-tunneling-01.txt and RFC 2817, Sections 5.2 and
// 5.3.
void BuildTunnelRequest(const HttpRequestInfo* request_info,
                        const HttpRequestHeaders& authorization_headers,
                        const HostPortPair& endpoint,
                        std::string* request_line,
                        HttpRequestHeaders* request_headers) {
  // RFC 2616 Section 9 says the Host request-header field MUST accompany all
  // HTTP/1.1 requests.  Add "Proxy-Connection: keep-alive" for compat with
  // HTTP/1.0 proxies such as Squid (required for NTLM authentication).
  *request_line = StringPrintf(
      "CONNECT %s HTTP/1.1\r\n", endpoint.ToString().c_str());
  request_headers->SetHeader(HttpRequestHeaders::kHost,
                             GetHostAndOptionalPort(request_info->url));
  request_headers->SetHeader(HttpRequestHeaders::kProxyConnection,
                             "keep-alive");

  std::string user_agent;
  if (request_info->extra_headers.GetHeader(HttpRequestHeaders::kUserAgent,
                                            &user_agent))
    request_headers->SetHeader(HttpRequestHeaders::kUserAgent, user_agent);

  request_headers->MergeFrom(authorization_headers);
}

void ProcessAlternateProtocol(const HttpResponseHeaders& headers,
                              const HostPortPair& http_host_port_pair,
                              HttpAlternateProtocols* alternate_protocols) {
  if (!g_next_protos || g_next_protos->empty()) {
    // This implies that NPN is not supported.  We don't currently support any
    // alternate protocols that don't use NPN.
    return;
  }

  std::string alternate_protocol_str;
  if (!headers.EnumerateHeader(NULL, HttpAlternateProtocols::kHeader,
                               &alternate_protocol_str)) {
    // Header is not present.
    return;
  }

  std::vector<std::string> port_protocol_vector;
  SplitString(alternate_protocol_str, ':', &port_protocol_vector);
  if (port_protocol_vector.size() != 2) {
    DLOG(WARNING) << HttpAlternateProtocols::kHeader
                  << " header has too many tokens: "
                  << alternate_protocol_str;
    return;
  }

  int port;
  if (!StringToInt(port_protocol_vector[0], &port) ||
      port <= 0 || port >= 1 << 16) {
    DLOG(WARNING) << HttpAlternateProtocols::kHeader
                  << " header has unrecognizable port: "
                  << port_protocol_vector[0];
    return;
  }

  if (port_protocol_vector[1] !=
      HttpAlternateProtocols::kProtocolStrings[
          HttpAlternateProtocols::NPN_SPDY_1]) {
    // Currently, we only recognize the npn-spdy protocol.
    DLOG(WARNING) << HttpAlternateProtocols::kHeader
                  << " header has unrecognized protocol: "
                  << port_protocol_vector[1];
    return;
  }

  HostPortPair host_port(http_host_port_pair);
  if (g_host_mapping_rules)
    g_host_mapping_rules->RewriteHost(&host_port);

  if (alternate_protocols->HasAlternateProtocolFor(host_port)) {
    const HttpAlternateProtocols::PortProtocolPair existing_alternate =
        alternate_protocols->GetAlternateProtocolFor(host_port);
    // If we think the alternate protocol is broken, don't change it.
    if (existing_alternate.protocol == HttpAlternateProtocols::BROKEN)
      return;
  }

  alternate_protocols->SetAlternateProtocolFor(
      host_port, port, HttpAlternateProtocols::NPN_SPDY_1);
}

class NetLogHttpRequestParameter : public NetLog::EventParameters {
 public:
  NetLogHttpRequestParameter(const std::string& line,
                             const HttpRequestHeaders& headers)
      : line_(line) {
    headers_.CopyFrom(headers);
  }

  Value* ToValue() const {
    DictionaryValue* dict = new DictionaryValue();
    dict->SetString(L"line", line_);
    ListValue* headers = new ListValue();
    HttpRequestHeaders::Iterator iterator(headers_);
    while (iterator.GetNext()) {
      headers->Append(
          new StringValue(StringPrintf("%s: %s",
                                       iterator.name().c_str(),
                                       iterator.value().c_str())));
    }
    dict->Set(L"headers", headers);
    return dict;
  }

 private:
  ~NetLogHttpRequestParameter() {}

  const std::string line_;
  HttpRequestHeaders headers_;

  DISALLOW_COPY_AND_ASSIGN(NetLogHttpRequestParameter);
};

class NetLogHttpResponseParameter : public NetLog::EventParameters {
 public:
  explicit NetLogHttpResponseParameter(
      const scoped_refptr<HttpResponseHeaders>& headers)
      : headers_(headers) {}

  Value* ToValue() const {
    DictionaryValue* dict = new DictionaryValue();
    ListValue* headers = new ListValue();
    headers->Append(new StringValue(headers_->GetStatusLine()));
    void* iterator = NULL;
    std::string name;
    std::string value;
    while (headers_->EnumerateHeaderLines(&iterator, &name, &value)) {
      headers->Append(
          new StringValue(StringPrintf("%s: %s", name.c_str(), value.c_str())));
    }
    dict->Set(L"headers", headers);
    return dict;
  }

 private:
  ~NetLogHttpResponseParameter() {}

  const scoped_refptr<HttpResponseHeaders> headers_;

  DISALLOW_COPY_AND_ASSIGN(NetLogHttpResponseParameter);
};

}  // namespace

//-----------------------------------------------------------------------------

bool HttpNetworkTransaction::g_ignore_certificate_errors = false;

HttpNetworkTransaction::HttpNetworkTransaction(HttpNetworkSession* session)
    : pending_auth_target_(HttpAuth::AUTH_NONE),
      ALLOW_THIS_IN_INITIALIZER_LIST(
          io_callback_(this, &HttpNetworkTransaction::OnIOComplete)),
      user_callback_(NULL),
      session_(session),
      request_(NULL),
      pac_request_(NULL),
      connection_(new ClientSocketHandle),
      reused_socket_(false),
      headers_valid_(false),
      logged_response_time_(false),
      using_ssl_(false),
      using_spdy_(false),
      alternate_protocol_mode_(
          g_use_alternate_protocols ? kUnspecified :
          kDoNotUseAlternateProtocol),
      read_buf_len_(0),
      next_state_(STATE_NONE) {
  session->ssl_config_service()->GetSSLConfig(&ssl_config_);
  if (g_next_protos)
    ssl_config_.next_protos = *g_next_protos;
  if (!g_tls_intolerant_servers)
    g_tls_intolerant_servers = new std::set<std::string>;
}

// static
void HttpNetworkTransaction::SetHostMappingRules(const std::string& rules) {
  HostMappingRules* host_mapping_rules = new HostMappingRules();
  host_mapping_rules->SetRulesFromString(rules);
  delete g_host_mapping_rules;
  g_host_mapping_rules = host_mapping_rules;
}

// static
void HttpNetworkTransaction::SetUseAlternateProtocols(bool value) {
  g_use_alternate_protocols = value;
}

// static
void HttpNetworkTransaction::SetNextProtos(const std::string& next_protos) {
  delete g_next_protos;
  g_next_protos = new std::string(next_protos);
}

// static
void HttpNetworkTransaction::IgnoreCertificateErrors(bool enabled) {
  g_ignore_certificate_errors = enabled;
}

int HttpNetworkTransaction::Start(const HttpRequestInfo* request_info,
                                  CompletionCallback* callback,
                                  const BoundNetLog& net_log) {
  SIMPLE_STATS_COUNTER("HttpNetworkTransaction.Count");

  net_log_ = net_log;
  request_ = request_info;
  start_time_ = base::Time::Now();

  next_state_ = STATE_RESOLVE_PROXY;
  int rv = DoLoop(OK);
  if (rv == ERR_IO_PENDING)
    user_callback_ = callback;
  return rv;
}

int HttpNetworkTransaction::RestartIgnoringLastError(
    CompletionCallback* callback) {
  if (connection_->socket()->IsConnectedAndIdle()) {
    // TODO(wtc): Should we update any of the connection histograms that we
    // update in DoSSLConnectComplete if |result| is OK?
    if (using_spdy_) {
      // TODO(cbentzel): Add auth support to spdy. See http://crbug.com/46620
      next_state_ = STATE_SPDY_SEND_REQUEST;
    } else {
      next_state_ = STATE_GENERATE_PROXY_AUTH_TOKEN;
    }
  } else {
    connection_->socket()->Disconnect();
    connection_->Reset();
    next_state_ = STATE_INIT_CONNECTION;
  }
  int rv = DoLoop(OK);
  if (rv == ERR_IO_PENDING)
    user_callback_ = callback;
  return rv;
}

int HttpNetworkTransaction::RestartWithCertificate(
    X509Certificate* client_cert,
    CompletionCallback* callback) {
  ssl_config_.client_cert = client_cert;
  if (client_cert) {
    session_->ssl_client_auth_cache()->Add(GetHostAndPort(request_->url),
                                           client_cert);
  }
  ssl_config_.send_client_cert = true;
  next_state_ = STATE_INIT_CONNECTION;
  // Reset the other member variables.
  // Note: this is necessary only with SSL renegotiation.
  ResetStateForRestart();
  int rv = DoLoop(OK);
  if (rv == ERR_IO_PENDING)
    user_callback_ = callback;
  return rv;
}

int HttpNetworkTransaction::RestartWithAuth(
    const std::wstring& username,
    const std::wstring& password,
    CompletionCallback* callback) {
  HttpAuth::Target target = pending_auth_target_;
  if (target == HttpAuth::AUTH_NONE) {
    NOTREACHED();
    return ERR_UNEXPECTED;
  }
  pending_auth_target_ = HttpAuth::AUTH_NONE;

  auth_controllers_[target]->ResetAuth(username, password);

  PrepareForAuthRestart(target);

  DCHECK(user_callback_ == NULL);
  int rv = DoLoop(OK);
  if (rv == ERR_IO_PENDING)
    user_callback_ = callback;

  return rv;
}

void HttpNetworkTransaction::PrepareForAuthRestart(HttpAuth::Target target) {
  DCHECK(HaveAuth(target));
  bool keep_alive = false;
  // Even if the server says the connection is keep-alive, we have to be
  // able to find the end of each response in order to reuse the connection.
  if (GetResponseHeaders()->IsKeepAlive() &&
      http_stream_->CanFindEndOfResponse()) {
    // If the response body hasn't been completely read, we need to drain
    // it first.
    if (!http_stream_->IsResponseBodyComplete()) {
      next_state_ = STATE_DRAIN_BODY_FOR_AUTH_RESTART;
      read_buf_ = new IOBuffer(kDrainBodyBufferSize);  // A bit bucket.
      read_buf_len_ = kDrainBodyBufferSize;
      return;
    }
    keep_alive = true;
  }

  // We don't need to drain the response body, so we act as if we had drained
  // the response body.
  DidDrainBodyForAuthRestart(keep_alive);
}

void HttpNetworkTransaction::DidDrainBodyForAuthRestart(bool keep_alive) {
  if (keep_alive && connection_->socket()->IsConnectedAndIdle()) {
    // We should call connection_->set_idle_time(), but this doesn't occur
    // often enough to be worth the trouble.
    if (using_ssl_ && proxy_info_.is_http() &&
        ssl_connect_start_time_.is_null())
      next_state_ = STATE_TUNNEL_GENERATE_AUTH_TOKEN;
    else
      next_state_ = STATE_GENERATE_PROXY_AUTH_TOKEN;
    connection_->set_is_reused(true);
    reused_socket_ = true;
  } else {
    next_state_ = STATE_INIT_CONNECTION;
    connection_->socket()->Disconnect();
    connection_->Reset();
  }

  // Reset the other member variables.
  ResetStateForRestart();
}

int HttpNetworkTransaction::Read(IOBuffer* buf, int buf_len,
                                 CompletionCallback* callback) {
  DCHECK(buf);
  DCHECK_LT(0, buf_len);

  State next_state = STATE_NONE;

  // Are we using SPDY or HTTP?
  if (using_spdy_) {
    DCHECK(!http_stream_.get());
    DCHECK(spdy_http_stream_->GetResponseInfo()->headers);
    next_state = STATE_SPDY_READ_BODY;
  } else {
    DCHECK(!spdy_http_stream_.get());
    next_state = STATE_READ_BODY;

    if (!connection_->is_initialized())
      return 0;  // connection_->has been reset.  Treat like EOF.
  }

  scoped_refptr<HttpResponseHeaders> headers = GetResponseHeaders();
  DCHECK(headers.get());
  if (headers->response_code() == 407) {
    // We're trying to read the body of the response but we're still trying
    // to establish an SSL tunnel through the proxy.  We can't read these
    // bytes when establishing a tunnel because they might be controlled by
    // an active network attacker.  We don't worry about this for HTTP
    // because an active network attacker can already control HTTP sessions.
    // We reach this case when the user cancels a 407 proxy auth prompt.
    // See http://crbug.com/8473.
    DCHECK(proxy_info_.is_http());
    LogBlockedTunnelResponse(headers->response_code());
    return ERR_TUNNEL_CONNECTION_FAILED;
  }

  read_buf_ = buf;
  read_buf_len_ = buf_len;

  next_state_ = next_state;
  int rv = DoLoop(OK);
  if (rv == ERR_IO_PENDING)
    user_callback_ = callback;
  return rv;
}

const HttpResponseInfo* HttpNetworkTransaction::GetResponseInfo() const {
  return ((headers_valid_ && response_.headers) || response_.ssl_info.cert ||
          response_.cert_request_info) ? &response_ : NULL;
}

LoadState HttpNetworkTransaction::GetLoadState() const {
  // TODO(wtc): Define a new LoadState value for the
  // STATE_INIT_CONNECTION_COMPLETE state, which delays the HTTP request.
  switch (next_state_) {
    case STATE_RESOLVE_PROXY_COMPLETE:
      return LOAD_STATE_RESOLVING_PROXY_FOR_URL;
    case STATE_INIT_CONNECTION_COMPLETE:
      return connection_->GetLoadState();
    case STATE_TUNNEL_GENERATE_AUTH_TOKEN_COMPLETE:
    case STATE_TUNNEL_SEND_REQUEST_COMPLETE:
    case STATE_TUNNEL_READ_HEADERS_COMPLETE:
      return LOAD_STATE_ESTABLISHING_PROXY_TUNNEL;
    case STATE_SSL_CONNECT_COMPLETE:
      return LOAD_STATE_SSL_HANDSHAKE;
    case STATE_GENERATE_PROXY_AUTH_TOKEN_COMPLETE:
    case STATE_GENERATE_SERVER_AUTH_TOKEN_COMPLETE:
    case STATE_SEND_REQUEST_COMPLETE:
    case STATE_SPDY_SEND_REQUEST_COMPLETE:
      return LOAD_STATE_SENDING_REQUEST;
    case STATE_READ_HEADERS_COMPLETE:
    case STATE_SPDY_READ_HEADERS_COMPLETE:
      return LOAD_STATE_WAITING_FOR_RESPONSE;
    case STATE_READ_BODY_COMPLETE:
    case STATE_SPDY_READ_BODY_COMPLETE:
      return LOAD_STATE_READING_RESPONSE;
    default:
      return LOAD_STATE_IDLE;
  }
}

uint64 HttpNetworkTransaction::GetUploadProgress() const {
  if (!http_stream_.get())
    return 0;

  return http_stream_->GetUploadProgress();
}

HttpNetworkTransaction::~HttpNetworkTransaction() {
  // If we still have an open socket, then make sure to disconnect it so it
  // won't call us back and we don't try to reuse it later on. However,
  // don't close the socket if we should keep the connection alive.
  if (connection_.get() && connection_->is_initialized()) {
    // The STATE_NONE check guarantees there are no pending socket IOs that
    // could try to call this object back after it is deleted.
    bool keep_alive = next_state_ == STATE_NONE &&
                      http_stream_.get() &&
                      http_stream_->IsResponseBodyComplete() &&
                      http_stream_->CanFindEndOfResponse() &&
                      GetResponseHeaders()->IsKeepAlive();
    if (!keep_alive)
      connection_->socket()->Disconnect();
  }

  if (pac_request_)
    session_->proxy_service()->CancelPacRequest(pac_request_);

  if (spdy_http_stream_.get())
    spdy_http_stream_->Cancel();
}

void HttpNetworkTransaction::DoCallback(int rv) {
  DCHECK(rv != ERR_IO_PENDING);
  DCHECK(user_callback_);

  // Since Run may result in Read being called, clear user_callback_ up front.
  CompletionCallback* c = user_callback_;
  user_callback_ = NULL;
  c->Run(rv);
}

void HttpNetworkTransaction::OnIOComplete(int result) {
  int rv = DoLoop(result);
  if (rv != ERR_IO_PENDING)
    DoCallback(rv);
}

int HttpNetworkTransaction::DoLoop(int result) {
  DCHECK(next_state_ != STATE_NONE);

  int rv = result;
  do {
    State state = next_state_;
    next_state_ = STATE_NONE;
    switch (state) {
      case STATE_RESOLVE_PROXY:
        DCHECK_EQ(OK, rv);
        rv = DoResolveProxy();
        break;
      case STATE_RESOLVE_PROXY_COMPLETE:
        rv = DoResolveProxyComplete(rv);
        break;
      case STATE_INIT_CONNECTION:
        DCHECK_EQ(OK, rv);
        rv = DoInitConnection();
        break;
      case STATE_INIT_CONNECTION_COMPLETE:
        rv = DoInitConnectionComplete(rv);
        break;
      case STATE_TUNNEL_GENERATE_AUTH_TOKEN:
        DCHECK_EQ(OK, rv);
        rv = DoTunnelGenerateAuthToken();
        break;
      case STATE_TUNNEL_GENERATE_AUTH_TOKEN_COMPLETE:
        rv = DoTunnelGenerateAuthTokenComplete(rv);
        break;
      case STATE_TUNNEL_SEND_REQUEST:
        DCHECK_EQ(OK, rv);
        net_log_.BeginEvent(NetLog::TYPE_HTTP_TRANSACTION_TUNNEL_SEND_REQUEST,
            NULL);
        rv = DoTunnelSendRequest();
        break;
      case STATE_TUNNEL_SEND_REQUEST_COMPLETE:
        rv = DoTunnelSendRequestComplete(rv);
        net_log_.EndEvent(NetLog::TYPE_HTTP_TRANSACTION_TUNNEL_SEND_REQUEST,
            NULL);
        break;
      case STATE_TUNNEL_READ_HEADERS:
        DCHECK_EQ(OK, rv);
        net_log_.BeginEvent(NetLog::TYPE_HTTP_TRANSACTION_TUNNEL_READ_HEADERS,
            NULL);
        rv = DoTunnelReadHeaders();
        break;
      case STATE_TUNNEL_READ_HEADERS_COMPLETE:
        rv = DoTunnelReadHeadersComplete(rv);
        net_log_.EndEvent(NetLog::TYPE_HTTP_TRANSACTION_TUNNEL_READ_HEADERS,
            NULL);
        break;
      case STATE_SSL_CONNECT:
        DCHECK_EQ(OK, rv);
        rv = DoSSLConnect();
        break;
      case STATE_SSL_CONNECT_COMPLETE:
        rv = DoSSLConnectComplete(rv);
        break;
      case STATE_GENERATE_PROXY_AUTH_TOKEN:
        DCHECK_EQ(OK, rv);
        rv = DoGenerateProxyAuthToken();
        break;
      case STATE_GENERATE_PROXY_AUTH_TOKEN_COMPLETE:
        rv = DoGenerateProxyAuthTokenComplete(rv);
        break;
      case STATE_GENERATE_SERVER_AUTH_TOKEN:
        DCHECK_EQ(OK, rv);
        rv = DoGenerateServerAuthToken();
        break;
      case STATE_GENERATE_SERVER_AUTH_TOKEN_COMPLETE:
        rv = DoGenerateServerAuthTokenComplete(rv);
        break;
      case STATE_SEND_REQUEST:
        DCHECK_EQ(OK, rv);
        net_log_.BeginEvent(NetLog::TYPE_HTTP_TRANSACTION_SEND_REQUEST, NULL);
        rv = DoSendRequest();
        break;
      case STATE_SEND_REQUEST_COMPLETE:
        rv = DoSendRequestComplete(rv);
        net_log_.EndEvent(NetLog::TYPE_HTTP_TRANSACTION_SEND_REQUEST, NULL);
        break;
      case STATE_READ_HEADERS:
        DCHECK_EQ(OK, rv);
        net_log_.BeginEvent(NetLog::TYPE_HTTP_TRANSACTION_READ_HEADERS, NULL);
        rv = DoReadHeaders();
        break;
      case STATE_READ_HEADERS_COMPLETE:
        rv = DoReadHeadersComplete(rv);
        net_log_.EndEvent(NetLog::TYPE_HTTP_TRANSACTION_READ_HEADERS, NULL);
        break;
      case STATE_RESOLVE_CANONICAL_NAME:
        DCHECK_EQ(OK, rv);
        net_log_.BeginEvent(
            NetLog::TYPE_HTTP_TRANSACTION_RESOLVE_CANONICAL_NAME, NULL);
        rv = DoResolveCanonicalName();
        break;
      case STATE_RESOLVE_CANONICAL_NAME_COMPLETE:
        rv = DoResolveCanonicalNameComplete(rv);
        net_log_.EndEvent(
            NetLog::TYPE_HTTP_TRANSACTION_RESOLVE_CANONICAL_NAME, NULL);
        break;
      case STATE_READ_BODY:
        DCHECK_EQ(OK, rv);
        net_log_.BeginEvent(NetLog::TYPE_HTTP_TRANSACTION_READ_BODY, NULL);
        rv = DoReadBody();
        break;
      case STATE_READ_BODY_COMPLETE:
        rv = DoReadBodyComplete(rv);
        net_log_.EndEvent(NetLog::TYPE_HTTP_TRANSACTION_READ_BODY, NULL);
        break;
      case STATE_DRAIN_BODY_FOR_AUTH_RESTART:
        DCHECK_EQ(OK, rv);
        net_log_.BeginEvent(
            NetLog::TYPE_HTTP_TRANSACTION_DRAIN_BODY_FOR_AUTH_RESTART, NULL);
        rv = DoDrainBodyForAuthRestart();
        break;
      case STATE_DRAIN_BODY_FOR_AUTH_RESTART_COMPLETE:
        rv = DoDrainBodyForAuthRestartComplete(rv);
        net_log_.EndEvent(
            NetLog::TYPE_HTTP_TRANSACTION_DRAIN_BODY_FOR_AUTH_RESTART, NULL);
        break;
      case STATE_SPDY_SEND_REQUEST:
        DCHECK_EQ(OK, rv);
        net_log_.BeginEvent(NetLog::TYPE_SPDY_TRANSACTION_SEND_REQUEST, NULL);
        rv = DoSpdySendRequest();
        break;
      case STATE_SPDY_SEND_REQUEST_COMPLETE:
        rv = DoSpdySendRequestComplete(rv);
        net_log_.EndEvent(NetLog::TYPE_SPDY_TRANSACTION_SEND_REQUEST, NULL);
        break;
      case STATE_SPDY_READ_HEADERS:
        DCHECK_EQ(OK, rv);
        net_log_.BeginEvent(NetLog::TYPE_SPDY_TRANSACTION_READ_HEADERS, NULL);
        rv = DoSpdyReadHeaders();
        break;
      case STATE_SPDY_READ_HEADERS_COMPLETE:
        rv = DoSpdyReadHeadersComplete(rv);
        net_log_.EndEvent(NetLog::TYPE_SPDY_TRANSACTION_READ_HEADERS, NULL);
        break;
      case STATE_SPDY_READ_BODY:
        DCHECK_EQ(OK, rv);
        net_log_.BeginEvent(NetLog::TYPE_SPDY_TRANSACTION_READ_BODY, NULL);
        rv = DoSpdyReadBody();
        break;
      case STATE_SPDY_READ_BODY_COMPLETE:
        rv = DoSpdyReadBodyComplete(rv);
        net_log_.EndEvent(NetLog::TYPE_SPDY_TRANSACTION_READ_BODY, NULL);
        break;
      default:
        NOTREACHED() << "bad state";
        rv = ERR_FAILED;
        break;
    }
  } while (rv != ERR_IO_PENDING && next_state_ != STATE_NONE);

  return rv;
}

int HttpNetworkTransaction::DoResolveProxy() {
  DCHECK(!pac_request_);

  next_state_ = STATE_RESOLVE_PROXY_COMPLETE;

  // |endpoint_| indicates the final destination endpoint.
  endpoint_ = HostPortPair(request_->url.HostNoBrackets(),
                           request_->url.EffectiveIntPort());

  // Extra URL we might be attempting to resolve to.
  GURL alternate_endpoint_url;

  // Tracks whether we are using |request_->url| or |alternate_endpoint_url|.
  const GURL *curr_endpoint_url = &request_->url;

  if (g_host_mapping_rules && g_host_mapping_rules->RewriteHost(&endpoint_)) {
    url_canon::Replacements<char> replacements;
    const std::string port_str = IntToString(endpoint_.port);
    replacements.SetPort(port_str.c_str(),
                         url_parse::Component(0, port_str.size()));
    replacements.SetHost(endpoint_.host.c_str(),
                         url_parse::Component(0, endpoint_.host.size()));
    alternate_endpoint_url = curr_endpoint_url->ReplaceComponents(replacements);
    curr_endpoint_url = &alternate_endpoint_url;
  }

  if (alternate_protocol_mode_ == kUnspecified) {
    const HttpAlternateProtocols& alternate_protocols =
        session_->alternate_protocols();
    if (alternate_protocols.HasAlternateProtocolFor(endpoint_)) {
      HttpAlternateProtocols::PortProtocolPair alternate =
          alternate_protocols.GetAlternateProtocolFor(endpoint_);
      if (alternate.protocol != HttpAlternateProtocols::BROKEN) {
        DCHECK_EQ(HttpAlternateProtocols::NPN_SPDY_1, alternate.protocol);
        endpoint_.port = alternate.port;
        alternate_protocol_ = HttpAlternateProtocols::NPN_SPDY_1;
        alternate_protocol_mode_ = kUsingAlternateProtocol;

        url_canon::Replacements<char> replacements;
        replacements.SetScheme("https",
                               url_parse::Component(0, strlen("https")));
        const std::string port_str = IntToString(endpoint_.port);
        replacements.SetPort(port_str.c_str(),
                             url_parse::Component(0, port_str.size()));
        alternate_endpoint_url =
            curr_endpoint_url->ReplaceComponents(replacements);
        curr_endpoint_url = &alternate_endpoint_url;
      }
    }
  }

  if (request_->load_flags & LOAD_BYPASS_PROXY) {
    proxy_info_.UseDirect();
    return OK;
  }

  return session_->proxy_service()->ResolveProxy(
      *curr_endpoint_url, &proxy_info_, &io_callback_, &pac_request_, net_log_);
}

int HttpNetworkTransaction::DoResolveProxyComplete(int result) {
  pac_request_ = NULL;

  if (result != OK)
    return result;

  // Remove unsupported proxies from the list.
  proxy_info_.RemoveProxiesWithoutScheme(
      ProxyServer::SCHEME_DIRECT | ProxyServer::SCHEME_HTTP |
      ProxyServer::SCHEME_SOCKS4 | ProxyServer::SCHEME_SOCKS5);

  if (proxy_info_.is_empty()) {
    // No proxies/direct to choose from. This happens when we don't support any
    // of the proxies in the returned list.
    return ERR_NO_SUPPORTED_PROXIES;
  }

  next_state_ = STATE_INIT_CONNECTION;
  return OK;
}

int HttpNetworkTransaction::DoInitConnection() {
  DCHECK(!connection_->is_initialized());
  DCHECK(proxy_info_.proxy_server().is_valid());

  // Now that the proxy server has been resolved, create the auth_controllers_.
  for (int i = 0; i < HttpAuth::AUTH_NUM_TARGETS; i++) {
    HttpAuth::Target target = static_cast<HttpAuth::Target>(i);
    if (!auth_controllers_[target].get())
      auth_controllers_[target].reset(new HttpAuthController(target,
                                                             AuthURL(target),
                                                             session_,
                                                             net_log_));
  }

  next_state_ = STATE_INIT_CONNECTION_COMPLETE;

  using_ssl_ = request_->url.SchemeIs("https") ||
      (alternate_protocol_mode_ == kUsingAlternateProtocol &&
       alternate_protocol_ == HttpAlternateProtocols::NPN_SPDY_1);

  using_spdy_ = false;

  // Build the string used to uniquely identify connections of this type.
  // Determine the host and port to connect to.
  std::string connection_group;

  // Use the fixed testing ports if they've been provided.
  if (using_ssl_) {
    if (session_->fixed_https_port() != 0)
      endpoint_.port = session_->fixed_https_port();
  } else if (session_->fixed_http_port() != 0) {
    endpoint_.port = session_->fixed_http_port();
  }

  response_.was_fetched_via_proxy = !proxy_info_.is_direct();

  // Check first if we have a spdy session for this group.  If so, then go
  // straight to using that.
  if (session_->spdy_session_pool()->HasSession(endpoint_)) {
    using_spdy_ = true;
    reused_socket_ = true;
    return OK;
  }

  connection_group = endpoint_.ToString();
  DCHECK(!connection_group.empty());

  if (using_ssl_)
    connection_group = StringPrintf("ssl/%s", connection_group.c_str());

  // If the user is refreshing the page, bypass the host cache.
  bool disable_resolver_cache = request_->load_flags & LOAD_BYPASS_CACHE ||
                                request_->load_flags & LOAD_VALIDATE_CACHE ||
                                request_->load_flags & LOAD_DISABLE_CACHE;

  int rv;
  if (!proxy_info_.is_direct()) {
    ProxyServer proxy_server = proxy_info_.proxy_server();
    HostPortPair proxy_host_port_pair(proxy_server.HostNoBrackets(),
                                      proxy_server.port());

    TCPSocketParams tcp_params(proxy_host_port_pair, request_->priority,
                               request_->referrer, disable_resolver_cache);

    if (proxy_info_.is_socks()) {
      const char* socks_version;
      bool socks_v5;
      if (proxy_info_.proxy_server().scheme() == ProxyServer::SCHEME_SOCKS5) {
        socks_version = "5";
        socks_v5 = true;
      } else {
        socks_version = "4";
        socks_v5 = false;
      }

      connection_group =
          StringPrintf("socks%s/%s", socks_version, connection_group.c_str());

      SOCKSSocketParams socks_params(tcp_params, socks_v5, endpoint_,
                                     request_->priority, request_->referrer);

      rv = connection_->Init(
          connection_group, socks_params, request_->priority,
          &io_callback_,
          session_->GetSocketPoolForSOCKSProxy(proxy_host_port_pair), net_log_);
    } else {
      rv = connection_->Init(
          connection_group, tcp_params, request_->priority,
          &io_callback_,
          session_->GetSocketPoolForHTTPProxy(proxy_host_port_pair), net_log_);
    }
  } else {
    TCPSocketParams tcp_params(endpoint_, request_->priority,
                               request_->referrer, disable_resolver_cache);
    rv = connection_->Init(connection_group, tcp_params, request_->priority,
                           &io_callback_, session_->tcp_socket_pool(),
                           net_log_);
  }

  return rv;
}

int HttpNetworkTransaction::DoInitConnectionComplete(int result) {
  if (result < 0) {
    if (alternate_protocol_mode_ == kUsingAlternateProtocol) {
      // Mark the alternate protocol as broken and fallback.
      MarkBrokenAlternateProtocolAndFallback();
      return OK;
    }

    return ReconsiderProxyAfterError(result);
  }

  DCHECK_EQ(OK, result);

  if (using_spdy_) {
    DCHECK(!connection_->is_initialized());
    // TODO(cbentzel): Add auth support to spdy. See http://crbug.com/46620
    next_state_ = STATE_SPDY_SEND_REQUEST;
    return OK;
  }

  LogHttpConnectedMetrics(*connection_);

  // Set the reused_socket_ flag to indicate that we are using a keep-alive
  // connection.  This flag is used to handle errors that occur while we are
  // trying to reuse a keep-alive connection.
  reused_socket_ = connection_->is_reused();
  if (reused_socket_) {
    if (using_ssl_) {
      SSLClientSocket* ssl_socket =
          reinterpret_cast<SSLClientSocket*>(connection_->socket());
      response_.was_npn_negotiated = ssl_socket->wasNpnNegotiated();
    }
    next_state_ = STATE_GENERATE_PROXY_AUTH_TOKEN;
  } else {
    // Now we have a TCP connected socket.  Perform other connection setup as
    // needed.
    UpdateConnectionTypeHistograms(CONNECTION_HTTP);
    if (using_ssl_) {
      if (proxy_info_.is_direct() || proxy_info_.is_socks())
        next_state_ = STATE_SSL_CONNECT;
      else
        next_state_ = STATE_TUNNEL_GENERATE_AUTH_TOKEN;
    } else {
      next_state_ = STATE_GENERATE_PROXY_AUTH_TOKEN;
    }
  }

  return OK;
}

void HttpNetworkTransaction::ClearTunnelState() {
  http_stream_.reset();
  request_headers_.clear();
  response_ = HttpResponseInfo();
  headers_valid_ = false;
}

int HttpNetworkTransaction::DoTunnelGenerateAuthToken() {
  next_state_ = STATE_TUNNEL_GENERATE_AUTH_TOKEN_COMPLETE;
  return auth_controllers_[HttpAuth::AUTH_PROXY]->MaybeGenerateAuthToken(
      request_, &io_callback_);
}

int HttpNetworkTransaction::DoTunnelGenerateAuthTokenComplete(int rv) {
  DCHECK_NE(ERR_IO_PENDING, rv);
  if (rv == OK)
    next_state_ = STATE_TUNNEL_SEND_REQUEST;
  return rv;
}

int HttpNetworkTransaction::DoTunnelSendRequest() {
  next_state_ = STATE_TUNNEL_SEND_REQUEST_COMPLETE;

  // This is constructed lazily (instead of within our Start method), so that
  // we have proxy info available.
  if (request_headers_.empty()) {
    HttpRequestHeaders authorization_headers;
    if (HaveAuth(HttpAuth::AUTH_PROXY))
      auth_controllers_[HttpAuth::AUTH_PROXY]->AddAuthorizationHeader(
          &authorization_headers);
    std::string request_line;
    HttpRequestHeaders request_headers;
    BuildTunnelRequest(request_, authorization_headers, endpoint_,
                       &request_line, &request_headers);
    if (net_log_.HasListener()) {
      net_log_.AddEvent(
          NetLog::TYPE_HTTP_TRANSACTION_SEND_TUNNEL_HEADERS,
          new NetLogHttpRequestParameter(
              request_line, request_headers));
    }
    request_headers_ = request_line + request_headers.ToString();
  }

  http_stream_.reset(new HttpBasicStream(connection_.get(), net_log_));

  return http_stream_->SendRequest(request_, request_headers_, NULL, &response_,
                                   &io_callback_);
}

int HttpNetworkTransaction::DoTunnelSendRequestComplete(int result) {
  if (result < 0)
    return result;

  next_state_ = STATE_TUNNEL_READ_HEADERS;
  return OK;
}

int HttpNetworkTransaction::DoTunnelReadHeaders() {
  next_state_ = STATE_TUNNEL_READ_HEADERS_COMPLETE;

  return http_stream_->ReadResponseHeaders(&io_callback_);
}

int HttpNetworkTransaction::DoTunnelReadHeadersComplete(int result) {
  if (result < 0) {
    if (result == ERR_CONNECTION_CLOSED)
      result = ERR_TUNNEL_CONNECTION_FAILED;
    ClearTunnelState();
    return result;
  }

  // Require the "HTTP/1.x" status line for SSL CONNECT.
  if (response_.headers->GetParsedHttpVersion() < HttpVersion(1, 0)) {
    ClearTunnelState();
    return ERR_TUNNEL_CONNECTION_FAILED;
  }

  if (net_log_.HasListener()) {
    net_log_.AddEvent(
        NetLog::TYPE_HTTP_TRANSACTION_READ_TUNNEL_RESPONSE_HEADERS,
        new NetLogHttpResponseParameter(response_.headers));
  }

  int rv = result;
  switch (response_.headers->response_code()) {
    case 200:  // OK
      if (http_stream_->IsMoreDataBuffered()) {
        // The proxy sent extraneous data after the headers.
        rv = ERR_TUNNEL_CONNECTION_FAILED;
      } else {
        next_state_ = STATE_SSL_CONNECT;
        rv = OK;
      }
      break;

      // We aren't able to CONNECT to the remote host through the proxy.  We
      // need to be very suspicious about the response because an active network
      // attacker can force us into this state by masquerading as the proxy.
      // The only safe thing to do here is to fail the connection because our
      // client is expecting an SSL protected response.
      // See http://crbug.com/7338.
    case 407:  // Proxy Authentication Required
      // We need this status code to allow proxy authentication.  Our
      // authentication code is smart enough to avoid being tricked by an
      // active network attacker.
      headers_valid_ = true;
      return HandleAuthChallenge(true);

    default:
      // For all other status codes, we conservatively fail the CONNECT
      // request.
      // We lose something by doing this.  We have seen proxy 403, 404, and
      // 501 response bodies that contain a useful error message.  For
      // example, Squid uses a 404 response to report the DNS error: "The
      // domain name does not exist."
      LogBlockedTunnelResponse(response_.headers->response_code());
      rv = ERR_TUNNEL_CONNECTION_FAILED;
  }
  ClearTunnelState();
  return rv;
}

int HttpNetworkTransaction::DoSSLConnect() {
  next_state_ = STATE_SSL_CONNECT_COMPLETE;

  if (ContainsKey(*g_tls_intolerant_servers, GetHostAndPort(request_->url))) {
    LOG(WARNING) << "Falling back to SSLv3 because host is TLS intolerant: "
                 << GetHostAndPort(request_->url);
    ssl_config_.tls1_enabled = false;
  }

  if (request_->load_flags & LOAD_VERIFY_EV_CERT)
    ssl_config_.verify_ev_cert = true;

  ssl_connect_start_time_ = base::TimeTicks::Now();

  // Add a SSL socket on top of our existing transport socket.
  ClientSocket* s = connection_->release_socket();
  s = session_->socket_factory()->CreateSSLClientSocket(
      s, request_->url.HostNoBrackets(), ssl_config_);
  connection_->set_socket(s);
  return connection_->socket()->Connect(&io_callback_);
}

int HttpNetworkTransaction::DoSSLConnectComplete(int result) {
  SSLClientSocket* ssl_socket =
      reinterpret_cast<SSLClientSocket*>(connection_->socket());

  SSLClientSocket::NextProtoStatus status =
      SSLClientSocket::kNextProtoUnsupported;
  std::string proto;
  // GetNextProto will fail and and trigger a NOTREACHED if we pass in a socket
  // that hasn't had SSL_ImportFD called on it. If we get a certificate error
  // here, then we know that we called SSL_ImportFD.
  if (result == OK || IsCertificateError(result))
    status = ssl_socket->GetNextProto(&proto);

  if (status == SSLClientSocket::kNextProtoNegotiated) {
    ssl_socket->setWasNpnNegotiated(true);
    response_.was_npn_negotiated = true;
    if (SSLClientSocket::NextProtoFromString(proto) ==
        SSLClientSocket::kProtoSPDY1) {
          using_spdy_ = true;
    }
  }

  if (alternate_protocol_mode_ == kUsingAlternateProtocol &&
      alternate_protocol_ == HttpAlternateProtocols::NPN_SPDY_1 &&
      !using_spdy_) {
    // We tried using the NPN_SPDY_1 alternate protocol, but failed, so we
    // fallback.
    MarkBrokenAlternateProtocolAndFallback();
    return OK;
  }

  if (IsCertificateError(result)) {
    result = HandleCertificateError(result);
    if (result == OK && !connection_->socket()->IsConnectedAndIdle()) {
      connection_->socket()->Disconnect();
      connection_->Reset();
      next_state_ = STATE_INIT_CONNECTION;
      return result;
    }
  }

  if (result == OK) {
    DCHECK(ssl_connect_start_time_ != base::TimeTicks());
    base::TimeDelta connect_duration =
        base::TimeTicks::Now() - ssl_connect_start_time_;

    if (using_spdy_) {
      UMA_HISTOGRAM_CUSTOM_TIMES("Net.SpdyConnectionLatency",
                                 connect_duration,
                                 base::TimeDelta::FromMilliseconds(1),
                                 base::TimeDelta::FromMinutes(10),
                                 100);

      UpdateConnectionTypeHistograms(CONNECTION_SPDY);
      // TODO(cbentzel): Add auth support to spdy. See http://crbug.com/46620
      next_state_ = STATE_SPDY_SEND_REQUEST;
    } else {
      UMA_HISTOGRAM_CUSTOM_TIMES("Net.SSL_Connection_Latency",
                                 connect_duration,
                                 base::TimeDelta::FromMilliseconds(1),
                                 base::TimeDelta::FromMinutes(10),
                                 100);

      next_state_ = STATE_GENERATE_PROXY_AUTH_TOKEN;
    }
  } else if (result == ERR_SSL_CLIENT_AUTH_CERT_NEEDED) {
    result = HandleCertificateRequest(result);
  } else {
    result = HandleSSLHandshakeError(result);
  }
  return result;
}

int HttpNetworkTransaction::DoGenerateProxyAuthToken() {
  next_state_ = STATE_GENERATE_PROXY_AUTH_TOKEN_COMPLETE;
  if (!ShouldApplyProxyAuth())
    return OK;
  return auth_controllers_[HttpAuth::AUTH_PROXY]->MaybeGenerateAuthToken(
      request_, &io_callback_);
}

int HttpNetworkTransaction::DoGenerateProxyAuthTokenComplete(int rv) {
  DCHECK_NE(ERR_IO_PENDING, rv);
  if (rv == OK)
    next_state_ = STATE_GENERATE_SERVER_AUTH_TOKEN;
  return rv;
}

int HttpNetworkTransaction::DoGenerateServerAuthToken() {
  next_state_ = STATE_GENERATE_SERVER_AUTH_TOKEN_COMPLETE;
  if (!ShouldApplyServerAuth())
    return OK;
  return auth_controllers_[HttpAuth::AUTH_SERVER]->MaybeGenerateAuthToken(
      request_, &io_callback_);
}

int HttpNetworkTransaction::DoGenerateServerAuthTokenComplete(int rv) {
  DCHECK_NE(ERR_IO_PENDING, rv);
  if (rv == OK)
    next_state_ = STATE_SEND_REQUEST;
  return rv;
}

int HttpNetworkTransaction::DoSendRequest() {
  next_state_ = STATE_SEND_REQUEST_COMPLETE;

  UploadDataStream* request_body = NULL;
  if (request_->upload_data) {
    int error_code;
    request_body = UploadDataStream::Create(request_->upload_data, &error_code);
    if (!request_body)
      return error_code;
  }

  // This is constructed lazily (instead of within our Start method), so that
  // we have proxy info available.
  if (request_headers_.empty()) {
    // Figure out if we can/should add Proxy-Authentication & Authentication
    // headers.
    HttpRequestHeaders authorization_headers;
    bool have_proxy_auth = (ShouldApplyProxyAuth() &&
                            HaveAuth(HttpAuth::AUTH_PROXY));
    bool have_server_auth = (ShouldApplyServerAuth() &&
                             HaveAuth(HttpAuth::AUTH_SERVER));
    if (have_proxy_auth)
      auth_controllers_[HttpAuth::AUTH_PROXY]->AddAuthorizationHeader(
          &authorization_headers);
    if (have_server_auth)
      auth_controllers_[HttpAuth::AUTH_SERVER]->AddAuthorizationHeader(
          &authorization_headers);
    std::string request_line;
    HttpRequestHeaders request_headers;
    BuildRequestHeaders(request_, authorization_headers, request_body,
                        !using_ssl_ && proxy_info_.is_http(), &request_line,
                        &request_headers);

    if (session_->network_delegate())
      session_->network_delegate()->OnSendHttpRequest(&request_headers);

    if (net_log_.HasListener()) {
      net_log_.AddEvent(
          NetLog::TYPE_HTTP_TRANSACTION_SEND_REQUEST_HEADERS,
          new NetLogHttpRequestParameter(
              request_line, request_headers));
    }

    request_headers_ = request_line + request_headers.ToString();
  }

  headers_valid_ = false;
  http_stream_.reset(new HttpBasicStream(connection_.get(), net_log_));

  return http_stream_->SendRequest(request_, request_headers_,
                                   request_body, &response_, &io_callback_);
}

int HttpNetworkTransaction::DoSendRequestComplete(int result) {
  if (result < 0)
    return HandleIOError(result);
  next_state_ = STATE_READ_HEADERS;
  return OK;
}

int HttpNetworkTransaction::DoReadHeaders() {
  next_state_ = STATE_READ_HEADERS_COMPLETE;
  return http_stream_->ReadResponseHeaders(&io_callback_);
}

int HttpNetworkTransaction::HandleConnectionClosedBeforeEndOfHeaders() {
  if (!response_.headers) {
    // The connection was closed before any data was sent. Likely an error
    // rather than empty HTTP/0.9 response.
    return ERR_EMPTY_RESPONSE;
  }

  return OK;
}

int HttpNetworkTransaction::DoReadHeadersComplete(int result) {
  // We can get a certificate error or ERR_SSL_CLIENT_AUTH_CERT_NEEDED here
  // due to SSL renegotiation.
  if (using_ssl_) {
    if (IsCertificateError(result)) {
      // We don't handle a certificate error during SSL renegotiation, so we
      // have to return an error that's not in the certificate error range
      // (-2xx).
      LOG(ERROR) << "Got a server certificate with error " << result
                 << " during SSL renegotiation";
      result = ERR_CERT_ERROR_IN_SSL_RENEGOTIATION;
    } else if (result == ERR_SSL_CLIENT_AUTH_CERT_NEEDED) {
      result = HandleCertificateRequest(result);
      if (result == OK)
        return result;
    } else if ((result == ERR_SSL_DECOMPRESSION_FAILURE_ALERT ||
                result == ERR_SSL_BAD_RECORD_MAC_ALERT) &&
               ssl_config_.tls1_enabled) {
      // Some buggy servers select DEFLATE compression when offered and then
      // fail to ever decompress anything. They will send a fatal alert telling
      // us this. Normally we would pick this up during the handshake because
      // our Finished message is compressed and we'll never get the server's
      // Finished if it fails to process ours.
      //
      // However, with False Start, we'll believe that the handshake is
      // complete as soon as we've /sent/ our Finished message. In this case,
      // we only find out that the server is buggy here, when we try to read
      // the initial reply.
      g_tls_intolerant_servers->insert(GetHostAndPort(request_->url));
      ResetConnectionAndRequestForResend();
      return OK;
    }
  }

  if (result < 0 && result != ERR_CONNECTION_CLOSED)
    return HandleIOError(result);

  if (result == ERR_CONNECTION_CLOSED && ShouldResendRequest(result)) {
    ResetConnectionAndRequestForResend();
    return OK;
  }

  // After we call RestartWithAuth a new response_time will be recorded, and
  // we need to be cautious about incorrectly logging the duration across the
  // authentication activity.
  LogTransactionConnectedMetrics();

  if (result == ERR_CONNECTION_CLOSED) {
    // For now, if we get at least some data, we do the best we can to make
    // sense of it and send it back up the stack.
    int rv = HandleConnectionClosedBeforeEndOfHeaders();
    if (rv != OK)
      return rv;
  }

  if (net_log_.HasListener()) {
    net_log_.AddEvent(
        NetLog::TYPE_HTTP_TRANSACTION_READ_RESPONSE_HEADERS,
        new NetLogHttpResponseParameter(response_.headers));
  }

  if (response_.headers->GetParsedHttpVersion() < HttpVersion(1, 0)) {
    // HTTP/0.9 doesn't support the PUT method, so lack of response headers
    // indicates a buggy server.  See:
    // https://bugzilla.mozilla.org/show_bug.cgi?id=193921
    if (request_->method == "PUT")
      return ERR_METHOD_NOT_SUPPORTED;
  }

  // Check for an intermediate 100 Continue response.  An origin server is
  // allowed to send this response even if we didn't ask for it, so we just
  // need to skip over it.
  // We treat any other 1xx in this same way (although in practice getting
  // a 1xx that isn't a 100 is rare).
  if (response_.headers->response_code() / 100 == 1) {
    response_.headers = new HttpResponseHeaders("");
    next_state_ = STATE_READ_HEADERS;
    return OK;
  }

  ProcessAlternateProtocol(*response_.headers,
                           endpoint_,
                           session_->mutable_alternate_protocols());

  int rv = HandleAuthChallenge(false);
  if (rv != OK)
    return rv;

  if (using_ssl_) {
    SSLClientSocket* ssl_socket =
        reinterpret_cast<SSLClientSocket*>(connection_->socket());
    ssl_socket->GetSSLInfo(&response_.ssl_info);
  }

  headers_valid_ = true;
  return OK;
}

int HttpNetworkTransaction::DoResolveCanonicalName() {
  DCHECK(auth_controllers_[pending_auth_target_].get());
  next_state_ = STATE_RESOLVE_CANONICAL_NAME_COMPLETE;
  return auth_controllers_[pending_auth_target_]->ResolveCanonicalName(
      &io_callback_);
}

int HttpNetworkTransaction::DoResolveCanonicalNameComplete(int result) {
  // The STATE_RESOLVE_CANONICAL_NAME state ends the Start sequence when the
  // canonical name of the server needs to be determined. Normally
  // DoReadHeadersComplete completes the sequence. The next state is
  // intentionally not set as it should be STATE_NONE;
  DCHECK_EQ(STATE_NONE, next_state_);
  return result;
}

int HttpNetworkTransaction::DoReadBody() {
  DCHECK(read_buf_);
  DCHECK_GT(read_buf_len_, 0);
  DCHECK(connection_->is_initialized());

  next_state_ = STATE_READ_BODY_COMPLETE;
  return http_stream_->ReadResponseBody(read_buf_, read_buf_len_,
                                        &io_callback_);
}

int HttpNetworkTransaction::DoReadBodyComplete(int result) {
  // We are done with the Read call.
  bool done = false, keep_alive = false;
  if (result <= 0)
    done = true;

  if (http_stream_->IsResponseBodyComplete()) {
    done = true;
    if (http_stream_->CanFindEndOfResponse())
      keep_alive = GetResponseHeaders()->IsKeepAlive();
  }

  // Clean up connection_->if we are done.
  if (done) {
    LogTransactionMetrics();
    if (!keep_alive)
      connection_->socket()->Disconnect();
    connection_->Reset();
    // The next Read call will return 0 (EOF).
  }

  // Clear these to avoid leaving around old state.
  read_buf_ = NULL;
  read_buf_len_ = 0;

  return result;
}

int HttpNetworkTransaction::DoDrainBodyForAuthRestart() {
  // This method differs from DoReadBody only in the next_state_.  So we just
  // call DoReadBody and override the next_state_.  Perhaps there is a more
  // elegant way for these two methods to share code.
  int rv = DoReadBody();
  DCHECK(next_state_ == STATE_READ_BODY_COMPLETE);
  next_state_ = STATE_DRAIN_BODY_FOR_AUTH_RESTART_COMPLETE;
  return rv;
}

// TODO(wtc): This method and the DoReadBodyComplete method are almost
// the same.  Figure out a good way for these two methods to share code.
int HttpNetworkTransaction::DoDrainBodyForAuthRestartComplete(int result) {
  // keep_alive defaults to true because the very reason we're draining the
  // response body is to reuse the connection for auth restart.
  bool done = false, keep_alive = true;
  if (result < 0) {
    // Error or closed connection while reading the socket.
    done = true;
    keep_alive = false;
  } else if (http_stream_->IsResponseBodyComplete()) {
    done = true;
  }

  if (done) {
    DidDrainBodyForAuthRestart(keep_alive);
  } else {
    // Keep draining.
    next_state_ = STATE_DRAIN_BODY_FOR_AUTH_RESTART;
  }

  return OK;
}

int HttpNetworkTransaction::DoSpdySendRequest() {
  next_state_ = STATE_SPDY_SEND_REQUEST_COMPLETE;
  CHECK(!spdy_http_stream_.get());

  // First we get a SPDY session.  Theoretically, we've just negotiated one, but
  // if one already exists, then screw it, use the existing one!  Otherwise,
  // use the existing TCP socket.

  const scoped_refptr<SpdySessionPool> spdy_pool =
      session_->spdy_session_pool();
  scoped_refptr<SpdySession> spdy_session;

  if (spdy_pool->HasSession(endpoint_)) {
    spdy_session = spdy_pool->Get(endpoint_, session_, net_log_);
  } else {
    // SPDY is negotiated using the TLS next protocol negotiation (NPN)
    // extension, so |connection_| must contain an SSLClientSocket.
    DCHECK(using_ssl_);
    CHECK(connection_->socket());
    int error = spdy_pool->GetSpdySessionFromSSLSocket(
        endpoint_, session_, connection_.release(), net_log_, spdy_session);
    if (error != OK)
      return error;
  }

  CHECK(spdy_session.get());

  UploadDataStream* upload_data = NULL;
  if (request_->upload_data) {
    int error_code = OK;
    upload_data = UploadDataStream::Create(request_->upload_data, &error_code);
    if (!upload_data)
      return error_code;
  }
  headers_valid_ = false;
  scoped_refptr<SpdyStream> spdy_stream;
  if (request_->method == "GET")
    spdy_stream = spdy_session->GetPushStream(request_->url, net_log_);
  if (spdy_stream.get()) {
    DCHECK(spdy_stream->pushed());
    CHECK(spdy_stream->GetDelegate() == NULL);
    spdy_http_stream_.reset(new SpdyHttpStream(spdy_stream));
    spdy_http_stream_->InitializeRequest(*request_, base::Time::Now(), NULL);
  } else {
    spdy_stream = spdy_session->CreateStream(request_->url,
                                             request_->priority,
                                             net_log_);
    DCHECK(!spdy_stream->pushed());
    CHECK(spdy_stream->GetDelegate() == NULL);
    spdy_http_stream_.reset(new SpdyHttpStream(spdy_stream));
    spdy_http_stream_->InitializeRequest(
        *request_, base::Time::Now(), upload_data);
  }
  return spdy_http_stream_->SendRequest(&response_, &io_callback_);
}

int HttpNetworkTransaction::DoSpdySendRequestComplete(int result) {
  if (result < 0)
    return result;

  next_state_ = STATE_SPDY_READ_HEADERS;
  return OK;
}

int HttpNetworkTransaction::DoSpdyReadHeaders() {
  next_state_ = STATE_SPDY_READ_HEADERS_COMPLETE;
  return spdy_http_stream_->ReadResponseHeaders(&io_callback_);
}

int HttpNetworkTransaction::DoSpdyReadHeadersComplete(int result) {
  // TODO(willchan): Flesh out the support for HTTP authentication here.
  if (result == OK)
    headers_valid_ = true;

  LogTransactionConnectedMetrics();

  return result;
}

int HttpNetworkTransaction::DoSpdyReadBody() {
  next_state_ = STATE_SPDY_READ_BODY_COMPLETE;

  return spdy_http_stream_->ReadResponseBody(
      read_buf_, read_buf_len_, &io_callback_);
}

int HttpNetworkTransaction::DoSpdyReadBodyComplete(int result) {
  read_buf_ = NULL;
  read_buf_len_ = 0;

  if (result <= 0)
    spdy_http_stream_.reset() ;

  return result;
}

void HttpNetworkTransaction::LogHttpConnectedMetrics(
    const ClientSocketHandle& handle) {
  UMA_HISTOGRAM_ENUMERATION("Net.HttpSocketType", handle.reuse_type(),
                            ClientSocketHandle::NUM_TYPES);

  switch (handle.reuse_type()) {
    case ClientSocketHandle::UNUSED:
      UMA_HISTOGRAM_CUSTOM_TIMES("Net.HttpConnectionLatency",
                                 handle.setup_time(),
                                 base::TimeDelta::FromMilliseconds(1),
                                 base::TimeDelta::FromMinutes(10),
                                 100);
      break;
    case ClientSocketHandle::UNUSED_IDLE:
      UMA_HISTOGRAM_CUSTOM_TIMES("Net.SocketIdleTimeBeforeNextUse_UnusedSocket",
                                 handle.idle_time(),
                                 base::TimeDelta::FromMilliseconds(1),
                                 base::TimeDelta::FromMinutes(6),
                                 100);
      break;
    case ClientSocketHandle::REUSED_IDLE:
      UMA_HISTOGRAM_CUSTOM_TIMES("Net.SocketIdleTimeBeforeNextUse_ReusedSocket",
                                 handle.idle_time(),
                                 base::TimeDelta::FromMilliseconds(1),
                                 base::TimeDelta::FromMinutes(6),
                                 100);
      break;
    default:
      NOTREACHED();
      break;
  }
}

void HttpNetworkTransaction::LogIOErrorMetrics(
    const ClientSocketHandle& handle) {
  UMA_HISTOGRAM_ENUMERATION("Net.IOError_SocketReuseType",
                            handle.reuse_type(), ClientSocketHandle::NUM_TYPES);

  switch (handle.reuse_type()) {
    case ClientSocketHandle::UNUSED:
      break;
    case ClientSocketHandle::UNUSED_IDLE:
      UMA_HISTOGRAM_CUSTOM_TIMES(
          "Net.SocketIdleTimeOnIOError2_UnusedSocket",
          handle.idle_time(), base::TimeDelta::FromMilliseconds(1),
          base::TimeDelta::FromMinutes(6), 100);
      break;
    case ClientSocketHandle::REUSED_IDLE:
      UMA_HISTOGRAM_CUSTOM_TIMES(
          "Net.SocketIdleTimeOnIOError2_ReusedSocket",
          handle.idle_time(), base::TimeDelta::FromMilliseconds(1),
          base::TimeDelta::FromMinutes(6), 100);
      break;
    default:
      NOTREACHED();
      break;
  }
}

void HttpNetworkTransaction::LogTransactionConnectedMetrics() {
  if (logged_response_time_)
    return;

  logged_response_time_ = true;

  base::TimeDelta total_duration = response_.response_time - start_time_;

  UMA_HISTOGRAM_CLIPPED_TIMES(
      "Net.Transaction_Connected_Under_10",
      total_duration,
      base::TimeDelta::FromMilliseconds(1), base::TimeDelta::FromMinutes(10),
      100);

  if (!reused_socket_) {
    UMA_HISTOGRAM_CLIPPED_TIMES(
        "Net.Transaction_Connected_New",
        total_duration,
        base::TimeDelta::FromMilliseconds(1), base::TimeDelta::FromMinutes(10),
        100);

  static bool use_conn_impact_histogram(
      FieldTrialList::Find("ConnCountImpact") &&
      !FieldTrialList::Find("ConnCountImpact")->group_name().empty());
  if (use_conn_impact_histogram) {
    UMA_HISTOGRAM_CLIPPED_TIMES(
        FieldTrial::MakeName("Net.Transaction_Connected_New",
            "ConnCountImpact"),
        total_duration,
        base::TimeDelta::FromMilliseconds(1), base::TimeDelta::FromMinutes(10),
        100);
    }
  }

  static bool use_spdy_histogram(FieldTrialList::Find("SpdyImpact") &&
      !FieldTrialList::Find("SpdyImpact")->group_name().empty());
  if (use_spdy_histogram && response_.was_npn_negotiated) {
    UMA_HISTOGRAM_CLIPPED_TIMES(
      FieldTrial::MakeName("Net.Transaction_Connected_Under_10", "SpdyImpact"),
        total_duration, base::TimeDelta::FromMilliseconds(1),
        base::TimeDelta::FromMinutes(10), 100);

    if (!reused_socket_) {
      UMA_HISTOGRAM_CLIPPED_TIMES(
          FieldTrial::MakeName("Net.Transaction_Connected_New", "SpdyImpact"),
          total_duration, base::TimeDelta::FromMilliseconds(1),
          base::TimeDelta::FromMinutes(10), 100);
    }
  }

  // Currently, non-zero priority requests are frame or sub-frame resource
  // types.  This will change when we also prioritize certain subresources like
  // css, js, etc.
  if (request_->priority) {
    UMA_HISTOGRAM_CLIPPED_TIMES(
        "Net.Priority_High_Latency",
        total_duration,
        base::TimeDelta::FromMilliseconds(1), base::TimeDelta::FromMinutes(10),
        100);
  } else {
    UMA_HISTOGRAM_CLIPPED_TIMES(
        "Net.Priority_Low_Latency",
        total_duration,
        base::TimeDelta::FromMilliseconds(1), base::TimeDelta::FromMinutes(10),
        100);
  }
}

void HttpNetworkTransaction::LogTransactionMetrics() const {
  base::TimeDelta duration = base::Time::Now() -
                             response_.request_time;
  if (60 < duration.InMinutes())
    return;

  base::TimeDelta total_duration = base::Time::Now() - start_time_;

  UMA_HISTOGRAM_LONG_TIMES("Net.Transaction_Latency", duration);
  UMA_HISTOGRAM_CLIPPED_TIMES("Net.Transaction_Latency_Under_10", duration,
                              base::TimeDelta::FromMilliseconds(1),
                              base::TimeDelta::FromMinutes(10),
                              100);
  UMA_HISTOGRAM_CLIPPED_TIMES("Net.Transaction_Latency_Total_Under_10",
                              total_duration,
                              base::TimeDelta::FromMilliseconds(1),
                              base::TimeDelta::FromMinutes(10), 100);
  if (!reused_socket_) {
    UMA_HISTOGRAM_CLIPPED_TIMES(
        "Net.Transaction_Latency_Total_New_Connection_Under_10",
        total_duration, base::TimeDelta::FromMilliseconds(1),
        base::TimeDelta::FromMinutes(10), 100);
  }
}

void HttpNetworkTransaction::LogBlockedTunnelResponse(
    int response_code) const {
  LOG(WARNING) << "Blocked proxy response with status " << response_code
               << " to CONNECT request for "
               << GetHostAndPort(request_->url) << ".";
}

int HttpNetworkTransaction::HandleCertificateError(int error) {
  DCHECK(using_ssl_);
  DCHECK(IsCertificateError(error));

  SSLClientSocket* ssl_socket =
      reinterpret_cast<SSLClientSocket*>(connection_->socket());
  ssl_socket->GetSSLInfo(&response_.ssl_info);

  // Add the bad certificate to the set of allowed certificates in the
  // SSL info object. This data structure will be consulted after calling
  // RestartIgnoringLastError(). And the user will be asked interactively
  // before RestartIgnoringLastError() is ever called.
  SSLConfig::CertAndStatus bad_cert;
  bad_cert.cert = response_.ssl_info.cert;
  bad_cert.cert_status = response_.ssl_info.cert_status;
  ssl_config_.allowed_bad_certs.push_back(bad_cert);

  if (g_ignore_certificate_errors)
    return OK;

  const int kCertFlags = LOAD_IGNORE_CERT_COMMON_NAME_INVALID |
                         LOAD_IGNORE_CERT_DATE_INVALID |
                         LOAD_IGNORE_CERT_AUTHORITY_INVALID |
                         LOAD_IGNORE_CERT_WRONG_USAGE;
  if (request_->load_flags & kCertFlags) {
    switch (error) {
      case ERR_CERT_COMMON_NAME_INVALID:
        if (request_->load_flags & LOAD_IGNORE_CERT_COMMON_NAME_INVALID)
          error = OK;
        break;
      case ERR_CERT_DATE_INVALID:
        if (request_->load_flags & LOAD_IGNORE_CERT_DATE_INVALID)
          error = OK;
        break;
      case ERR_CERT_AUTHORITY_INVALID:
        if (request_->load_flags & LOAD_IGNORE_CERT_AUTHORITY_INVALID)
          error = OK;
        break;
    }
  }
  return error;
}

int HttpNetworkTransaction::HandleCertificateRequest(int error) {
  // Assert that the socket did not send a client certificate.
  // Note: If we got a reused socket, it was created with some other
  // transaction's ssl_config_, so we need to disable this assertion.  We can
  // get a certificate request on a reused socket when the server requested
  // renegotiation (rehandshake).
  // TODO(wtc): add a GetSSLParams method to SSLClientSocket so we can query
  // the SSL parameters it was created with and get rid of the reused_socket_
  // test.
  DCHECK(reused_socket_ || !ssl_config_.send_client_cert);

  response_.cert_request_info = new SSLCertRequestInfo;
  SSLClientSocket* ssl_socket =
      reinterpret_cast<SSLClientSocket*>(connection_->socket());
  ssl_socket->GetSSLCertRequestInfo(response_.cert_request_info);

  // Close the connection while the user is selecting a certificate to send
  // to the server.
  connection_->socket()->Disconnect();
  connection_->Reset();

  // If the user selected one of the certificate in client_certs for this
  // server before, use it automatically.
  X509Certificate* client_cert = session_->ssl_client_auth_cache()->
                                 Lookup(GetHostAndPort(request_->url));
  if (client_cert) {
    const std::vector<scoped_refptr<X509Certificate> >& client_certs =
        response_.cert_request_info->client_certs;
    for (size_t i = 0; i < client_certs.size(); ++i) {
      if (client_cert->fingerprint().Equals(client_certs[i]->fingerprint())) {
        ssl_config_.client_cert = client_cert;
        ssl_config_.send_client_cert = true;
        next_state_ = STATE_INIT_CONNECTION;
        // Reset the other member variables.
        // Note: this is necessary only with SSL renegotiation.
        ResetStateForRestart();
        return OK;
      }
    }
  }
  return error;
}

int HttpNetworkTransaction::HandleSSLHandshakeError(int error) {
  if (ssl_config_.send_client_cert &&
      (error == ERR_SSL_PROTOCOL_ERROR ||
       error == ERR_BAD_SSL_CLIENT_AUTH_CERT)) {
    session_->ssl_client_auth_cache()->Remove(GetHostAndPort(request_->url));
  }

  switch (error) {
    case ERR_SSL_PROTOCOL_ERROR:
    case ERR_SSL_VERSION_OR_CIPHER_MISMATCH:
    case ERR_SSL_DECOMPRESSION_FAILURE_ALERT:
    case ERR_SSL_BAD_RECORD_MAC_ALERT:
      if (ssl_config_.tls1_enabled) {
        // This could be a TLS-intolerant server, an SSL 3.0 server that
        // chose a TLS-only cipher suite or a server with buggy DEFLATE
        // support. Turn off TLS 1.0, DEFLATE support and retry.
        g_tls_intolerant_servers->insert(GetHostAndPort(request_->url));
        ResetConnectionAndRequestForResend();
        error = OK;
      }
      break;
  }
  return error;
}

// This method determines whether it is safe to resend the request after an
// IO error.  It can only be called in response to request header or body
// write errors or response header read errors.  It should not be used in
// other cases, such as a Connect error.
int HttpNetworkTransaction::HandleIOError(int error) {
  switch (error) {
    // If we try to reuse a connection that the server is in the process of
    // closing, we may end up successfully writing out our request (or a
    // portion of our request) only to find a connection error when we try to
    // read from (or finish writing to) the socket.
    case ERR_CONNECTION_RESET:
    case ERR_CONNECTION_CLOSED:
    case ERR_CONNECTION_ABORTED:
      LogIOErrorMetrics(*connection_);
      if (ShouldResendRequest(error)) {
        ResetConnectionAndRequestForResend();
        error = OK;
      }
      break;
  }
  return error;
}

void HttpNetworkTransaction::ResetStateForRestart() {
  pending_auth_target_ = HttpAuth::AUTH_NONE;
  read_buf_ = NULL;
  read_buf_len_ = 0;
  http_stream_.reset();
  headers_valid_ = false;
  request_headers_.clear();
  response_ = HttpResponseInfo();
}

HttpResponseHeaders* HttpNetworkTransaction::GetResponseHeaders() const {
  return response_.headers;
}

bool HttpNetworkTransaction::ShouldResendRequest(int error) const {
  // NOTE: we resend a request only if we reused a keep-alive connection.
  // This automatically prevents an infinite resend loop because we'll run
  // out of the cached keep-alive connections eventually.
  if (!connection_->ShouldResendFailedRequest(error) ||
      GetResponseHeaders()) {  // We have received some response headers.
    return false;
  }
  return true;
}

void HttpNetworkTransaction::ResetConnectionAndRequestForResend() {
  connection_->socket()->Disconnect();
  connection_->Reset();
  // We need to clear request_headers_ because it contains the real request
  // headers, but we may need to resend the CONNECT request first to recreate
  // the SSL tunnel.
  request_headers_.clear();
  next_state_ = STATE_INIT_CONNECTION;  // Resend the request.
}

int HttpNetworkTransaction::ReconsiderProxyAfterError(int error) {
  DCHECK(!pac_request_);

  // A failure to resolve the hostname or any error related to establishing a
  // TCP connection could be grounds for trying a new proxy configuration.
  //
  // Why do this when a hostname cannot be resolved?  Some URLs only make sense
  // to proxy servers.  The hostname in those URLs might fail to resolve if we
  // are still using a non-proxy config.  We need to check if a proxy config
  // now exists that corresponds to a proxy server that could load the URL.
  //
  switch (error) {
    case ERR_NAME_NOT_RESOLVED:
    case ERR_INTERNET_DISCONNECTED:
    case ERR_ADDRESS_UNREACHABLE:
    case ERR_CONNECTION_CLOSED:
    case ERR_CONNECTION_RESET:
    case ERR_CONNECTION_REFUSED:
    case ERR_CONNECTION_ABORTED:
    case ERR_TIMED_OUT:
    case ERR_TUNNEL_CONNECTION_FAILED:
    case ERR_SOCKS_CONNECTION_FAILED:
      break;
    case ERR_SOCKS_CONNECTION_HOST_UNREACHABLE:
      // Remap the SOCKS-specific "host unreachable" error to a more
      // generic error code (this way consumers like the link doctor
      // know to substitute their error page).
      //
      // Note that if the host resolving was done by the SOCSK5 proxy, we can't
      // differentiate between a proxy-side "host not found" versus a proxy-side
      // "address unreachable" error, and will report both of these failures as
      // ERR_ADDRESS_UNREACHABLE.
      return ERR_ADDRESS_UNREACHABLE;
    default:
      return error;
  }

  if (request_->load_flags & LOAD_BYPASS_PROXY) {
    return error;
  }

  int rv = session_->proxy_service()->ReconsiderProxyAfterError(
      request_->url, &proxy_info_, &io_callback_, &pac_request_, net_log_);
  if (rv == OK || rv == ERR_IO_PENDING) {
    // If the error was during connection setup, there is no socket to
    // disconnect.
    if (connection_->socket())
      connection_->socket()->Disconnect();
    connection_->Reset();
    next_state_ = STATE_RESOLVE_PROXY_COMPLETE;
  } else {
    // If ReconsiderProxyAfterError() failed synchronously, it means
    // there was nothing left to fall-back to, so fail the transaction
    // with the last connection error we got.
    // TODO(eroman): This is a confusing contract, make it more obvious.
    rv = error;
  }

  return rv;
}

bool HttpNetworkTransaction::ShouldApplyProxyAuth() const {
  return !using_ssl_ && proxy_info_.is_http();
}

bool HttpNetworkTransaction::ShouldApplyServerAuth() const {
  return !(request_->load_flags & LOAD_DO_NOT_SEND_AUTH_DATA);
}

int HttpNetworkTransaction::HandleAuthChallenge(bool establishing_tunnel) {
  scoped_refptr<HttpResponseHeaders> headers = GetResponseHeaders();
  DCHECK(headers);

  int status = headers->response_code();
  if (status != 401 && status != 407)
    return OK;
  HttpAuth::Target target = status == 407 ?
                            HttpAuth::AUTH_PROXY : HttpAuth::AUTH_SERVER;
  if (target == HttpAuth::AUTH_PROXY && proxy_info_.is_direct())
    return ERR_UNEXPECTED_PROXY_AUTH;

  int rv = auth_controllers_[target]->HandleAuthChallenge(headers,
                                                          request_->load_flags,
                                                          establishing_tunnel);
  if (auth_controllers_[target]->HaveAuthHandler())
      pending_auth_target_ = target;

  scoped_refptr<AuthChallengeInfo> auth_info =
      auth_controllers_[target]->auth_info();
  if (auth_info.get())
      response_.auth_challenge = auth_info;

  if (rv == ERR_AUTH_NEEDS_CANONICAL_NAME) {
      next_state_ = STATE_RESOLVE_CANONICAL_NAME;
      rv = OK;
  }

  return rv;
}

GURL HttpNetworkTransaction::AuthURL(HttpAuth::Target target) const {
  switch (target) {
    case HttpAuth::AUTH_PROXY:
      if (!proxy_info_.proxy_server().is_valid() ||
          proxy_info_.proxy_server().is_direct()) {
        return GURL();  // There is no proxy server.
      }
      return GURL("http://" + proxy_info_.proxy_server().host_and_port());
    case HttpAuth::AUTH_SERVER:
      return request_->url;
    default:
     return GURL();
  }
}

void HttpNetworkTransaction::MarkBrokenAlternateProtocolAndFallback() {
  // We have to:
  // * Reset the endpoint to be the unmodified URL specified destination.
  // * Mark the endpoint as broken so we don't try again.
  // * Set the alternate protocol mode to kDoNotUseAlternateProtocol so we
  // ignore future Alternate-Protocol headers from the HostPortPair.
  // * Reset the connection and go back to STATE_INIT_CONNECTION.

  endpoint_ = HostPortPair(request_->url.HostNoBrackets(),
                           request_->url.EffectiveIntPort());

  session_->mutable_alternate_protocols()->MarkBrokenAlternateProtocolFor(
      endpoint_);

  alternate_protocol_mode_ = kDoNotUseAlternateProtocol;
  if (connection_->socket())
    connection_->socket()->Disconnect();
  connection_->Reset();
  next_state_ = STATE_INIT_CONNECTION;
}

}  // namespace net
