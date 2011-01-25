// Copyright (c) 2010 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <set>
#include <string>

#include "base/ref_counted.h"
#include "base/scoped_ptr.h"
#include "base/string_util.h"
#include "net/base/mock_host_resolver.h"
#include "net/base/net_errors.h"
#include "net/http/http_auth.h"
#include "net/http/http_auth_filter.h"
#include "net/http/http_auth_handler.h"
#include "net/http/http_auth_handler_factory.h"
#include "net/http/http_auth_handler_mock.h"
#include "net/http/http_response_headers.h"
#include "net/http/http_util.h"
#include "net/http/mock_allow_url_security_manager.h"
#include "testing/gtest/include/gtest/gtest.h"

namespace net {

namespace {

HttpAuthHandlerMock* CreateMockHandler(bool connection_based) {
  HttpAuthHandlerMock* auth_handler = new HttpAuthHandlerMock();
  auth_handler->set_connection_based(connection_based);
  std::string challenge_text = "Basic";
  HttpAuth::ChallengeTokenizer challenge(challenge_text.begin(),
                                         challenge_text.end());
  GURL origin("www.example.com");
  EXPECT_TRUE(auth_handler->InitFromChallenge(&challenge,
                                              HttpAuth::AUTH_SERVER,
                                              origin,
                                              BoundNetLog()));
  return auth_handler;
}

HttpResponseHeaders* HeadersFromResponseText(const std::string& response) {
  return new HttpResponseHeaders(
      HttpUtil::AssembleRawHeaders(response.c_str(), response.length()));
}

HttpAuth::AuthorizationResult HandleChallengeResponse(
    bool connection_based,
    const std::string& headers_text,
    std::string* challenge_used) {
  scoped_ptr<HttpAuthHandlerMock> mock_handler(
      CreateMockHandler(connection_based));
  std::set<HttpAuth::Scheme> disabled_schemes;
  scoped_refptr<HttpResponseHeaders> headers(
      HeadersFromResponseText(headers_text));
  return HttpAuth::HandleChallengeResponse(
      mock_handler.get(),
      headers.get(),
      HttpAuth::AUTH_SERVER,
      disabled_schemes,
      challenge_used);
}

}  // namespace

TEST(HttpAuthTest, ChooseBestChallenge) {
  static const struct {
    const char* headers;
    HttpAuth::Scheme challenge_scheme;
    const char* challenge_realm;
  } tests[] = {
    {
      // Basic is the only challenge type, pick it.
      "Y: Digest realm=\"X\", nonce=\"aaaaaaaaaa\"\n"
      "www-authenticate: Basic realm=\"BasicRealm\"\n",

      HttpAuth::AUTH_SCHEME_BASIC,
      "BasicRealm",
    },
    {
      // Fake is the only challenge type, but it is unsupported.
      "Y: Digest realm=\"FooBar\", nonce=\"aaaaaaaaaa\"\n"
      "www-authenticate: Fake realm=\"FooBar\"\n",

      HttpAuth::AUTH_SCHEME_MAX,
      "",
    },
    {
      // Pick Digest over Basic.
      "www-authenticate: Basic realm=\"FooBar\"\n"
      "www-authenticate: Fake realm=\"FooBar\"\n"
      "www-authenticate: nonce=\"aaaaaaaaaa\"\n"
      "www-authenticate: Digest realm=\"DigestRealm\", nonce=\"aaaaaaaaaa\"\n",

      HttpAuth::AUTH_SCHEME_DIGEST,
      "DigestRealm",
    },
    {
      // Handle an empty header correctly.
      "Y: Digest realm=\"X\", nonce=\"aaaaaaaaaa\"\n"
      "www-authenticate:\n",

      HttpAuth::AUTH_SCHEME_MAX,
      "",
    },
    {
      // Choose Negotiate over NTLM on all platforms.
      // TODO(ahendrickson): This may be flaky on Linux and OSX as it
      // relies on being able to load one of the known .so files
      // for gssapi.
      "WWW-Authenticate: Negotiate\n"
      "WWW-Authenticate: NTLM\n",

      HttpAuth::AUTH_SCHEME_NEGOTIATE,
      "",
    }
  };
  GURL origin("http://www.example.com");
  std::set<HttpAuth::Scheme> disabled_schemes;
  MockAllowURLSecurityManager url_security_manager;
  scoped_ptr<HostResolver> host_resolver(new MockHostResolver());
  scoped_ptr<HttpAuthHandlerRegistryFactory> http_auth_handler_factory(
      HttpAuthHandlerFactory::CreateDefault(host_resolver.get()));
  http_auth_handler_factory->SetURLSecurityManager(
      "negotiate", &url_security_manager);

  for (size_t i = 0; i < ARRAYSIZE_UNSAFE(tests); ++i) {
    // Make a HttpResponseHeaders object.
    std::string headers_with_status_line("HTTP/1.1 401 Unauthorized\n");
    headers_with_status_line += tests[i].headers;
    scoped_refptr<HttpResponseHeaders> headers(
        HeadersFromResponseText(headers_with_status_line));

    scoped_ptr<HttpAuthHandler> handler;
    HttpAuth::ChooseBestChallenge(http_auth_handler_factory.get(),
                                  headers.get(),
                                  HttpAuth::AUTH_SERVER,
                                  origin,
                                  disabled_schemes,
                                  BoundNetLog(),
                                  &handler);

    if (handler.get()) {
      EXPECT_EQ(tests[i].challenge_scheme, handler->auth_scheme());
      EXPECT_STREQ(tests[i].challenge_realm, handler->realm().c_str());
    } else {
      EXPECT_EQ(HttpAuth::AUTH_SCHEME_MAX, tests[i].challenge_scheme);
      EXPECT_STREQ("", tests[i].challenge_realm);
    }
  }
}

TEST(HttpAuthTest, HandleChallengeResponse) {
  std::string challenge_used;
  const char* const kMockChallenge =
      "HTTP/1.1 401 Unauthorized\n"
      "WWW-Authenticate: Mock token_here\n";
  const char* const kBasicChallenge =
      "HTTP/1.1 401 Unauthorized\n"
      "WWW-Authenticate: Basic realm=\"happy\"\n";
  const char* const kMissingChallenge =
      "HTTP/1.1 401 Unauthorized\n";
  const char* const kEmptyChallenge =
      "HTTP/1.1 401 Unauthorized\n"
      "WWW-Authenticate: \n";
  const char* const kBasicAndMockChallenges =
      "HTTP/1.1 401 Unauthorized\n"
      "WWW-Authenticate: Basic realm=\"happy\"\n"
      "WWW-Authenticate: Mock token_here\n";
  const char* const kTwoMockChallenges =
      "HTTP/1.1 401 Unauthorized\n"
      "WWW-Authenticate: Mock token_a\n"
      "WWW-Authenticate: Mock token_b\n";

  // Request based schemes should treat any new challenges as rejections of the
  // previous authentication attempt. (There is a slight exception for digest
  // authentication and the stale parameter, but that is covered in the
  // http_auth_handler_digest_unittests).
  EXPECT_EQ(
      HttpAuth::AUTHORIZATION_RESULT_REJECT,
      HandleChallengeResponse(false, kMockChallenge, &challenge_used));
  EXPECT_EQ("Mock token_here", challenge_used);

  EXPECT_EQ(
      HttpAuth::AUTHORIZATION_RESULT_REJECT,
      HandleChallengeResponse(false, kBasicChallenge, &challenge_used));
  EXPECT_EQ("", challenge_used);

  EXPECT_EQ(
      HttpAuth::AUTHORIZATION_RESULT_REJECT,
      HandleChallengeResponse(false, kMissingChallenge, &challenge_used));
  EXPECT_EQ("", challenge_used);

  EXPECT_EQ(
      HttpAuth::AUTHORIZATION_RESULT_REJECT,
      HandleChallengeResponse(false, kEmptyChallenge, &challenge_used));
  EXPECT_EQ("", challenge_used);

  EXPECT_EQ(
      HttpAuth::AUTHORIZATION_RESULT_REJECT,
      HandleChallengeResponse(false, kBasicAndMockChallenges, &challenge_used));
  EXPECT_EQ("Mock token_here", challenge_used);

  EXPECT_EQ(
      HttpAuth::AUTHORIZATION_RESULT_REJECT,
      HandleChallengeResponse(false, kTwoMockChallenges, &challenge_used));
  EXPECT_EQ("Mock token_a", challenge_used);

  // Connection based schemes will treat new auth challenges for the same scheme
  // as acceptance (and continuance) of the current approach. If there are
  // no auth challenges for the same scheme, the response will be treated as
  // a rejection.
  EXPECT_EQ(
      HttpAuth::AUTHORIZATION_RESULT_ACCEPT,
      HandleChallengeResponse(true, kMockChallenge, &challenge_used));
  EXPECT_EQ("Mock token_here", challenge_used);

  EXPECT_EQ(
      HttpAuth::AUTHORIZATION_RESULT_REJECT,
      HandleChallengeResponse(true, kBasicChallenge, &challenge_used));
  EXPECT_EQ("", challenge_used);

  EXPECT_EQ(
      HttpAuth::AUTHORIZATION_RESULT_REJECT,
      HandleChallengeResponse(true, kMissingChallenge, &challenge_used));
  EXPECT_EQ("", challenge_used);

  EXPECT_EQ(
      HttpAuth::AUTHORIZATION_RESULT_REJECT,
      HandleChallengeResponse(true, kEmptyChallenge, &challenge_used));
  EXPECT_EQ("", challenge_used);

  EXPECT_EQ(
      HttpAuth::AUTHORIZATION_RESULT_ACCEPT,
      HandleChallengeResponse(true, kBasicAndMockChallenges, &challenge_used));
  EXPECT_EQ("Mock token_here", challenge_used);

  EXPECT_EQ(
      HttpAuth::AUTHORIZATION_RESULT_ACCEPT,
      HandleChallengeResponse(true, kTwoMockChallenges, &challenge_used));
  EXPECT_EQ("Mock token_a", challenge_used);
}

TEST(HttpAuthTest, ChallengeTokenizer) {
  std::string challenge_str = "Basic realm=\"foobar\"";
  HttpAuth::ChallengeTokenizer challenge(challenge_str.begin(),
                                         challenge_str.end());
  HttpUtil::NameValuePairsIterator parameters = challenge.param_pairs();

  EXPECT_TRUE(parameters.valid());
  EXPECT_EQ(std::string("Basic"), challenge.scheme());
  EXPECT_TRUE(parameters.GetNext());
  EXPECT_TRUE(parameters.valid());
  EXPECT_EQ(std::string("realm"), parameters.name());
  EXPECT_EQ(std::string("foobar"), parameters.value());
  EXPECT_FALSE(parameters.GetNext());
}

// Use a name=value property with no quote marks.
TEST(HttpAuthTest, ChallengeTokenizerNoQuotes) {
  std::string challenge_str = "Basic realm=foobar@baz.com";
  HttpAuth::ChallengeTokenizer challenge(challenge_str.begin(),
                                         challenge_str.end());
  HttpUtil::NameValuePairsIterator parameters = challenge.param_pairs();

  EXPECT_TRUE(parameters.valid());
  EXPECT_EQ(std::string("Basic"), challenge.scheme());
  EXPECT_TRUE(parameters.GetNext());
  EXPECT_TRUE(parameters.valid());
  EXPECT_EQ(std::string("realm"), parameters.name());
  EXPECT_EQ(std::string("foobar@baz.com"), parameters.value());
  EXPECT_FALSE(parameters.GetNext());
}

// Use a name=value property with mismatching quote marks.
TEST(HttpAuthTest, ChallengeTokenizerMismatchedQuotes) {
  std::string challenge_str = "Basic realm=\"foobar@baz.com";
  HttpAuth::ChallengeTokenizer challenge(challenge_str.begin(),
                                         challenge_str.end());
  HttpUtil::NameValuePairsIterator parameters = challenge.param_pairs();

  EXPECT_TRUE(parameters.valid());
  EXPECT_EQ(std::string("Basic"), challenge.scheme());
  EXPECT_TRUE(parameters.GetNext());
  EXPECT_TRUE(parameters.valid());
  EXPECT_EQ(std::string("realm"), parameters.name());
  EXPECT_EQ(std::string("foobar@baz.com"), parameters.value());
  EXPECT_FALSE(parameters.GetNext());
}

// Use a name= property without a value and with mismatching quote marks.
TEST(HttpAuthTest, ChallengeTokenizerMismatchedQuotesNoValue) {
  std::string challenge_str = "Basic realm=\"";
  HttpAuth::ChallengeTokenizer challenge(challenge_str.begin(),
                                         challenge_str.end());
  HttpUtil::NameValuePairsIterator parameters = challenge.param_pairs();

  EXPECT_TRUE(parameters.valid());
  EXPECT_EQ(std::string("Basic"), challenge.scheme());
  EXPECT_TRUE(parameters.GetNext());
  EXPECT_TRUE(parameters.valid());
  EXPECT_EQ(std::string("realm"), parameters.name());
  EXPECT_EQ(std::string(""), parameters.value());
  EXPECT_FALSE(parameters.GetNext());
}

// Use a name=value property with mismatching quote marks and spaces in the
// value.
TEST(HttpAuthTest, ChallengeTokenizerMismatchedQuotesSpaces) {
  std::string challenge_str = "Basic realm=\"foo bar";
  HttpAuth::ChallengeTokenizer challenge(challenge_str.begin(),
                                         challenge_str.end());
  HttpUtil::NameValuePairsIterator parameters = challenge.param_pairs();

  EXPECT_TRUE(parameters.valid());
  EXPECT_EQ(std::string("Basic"), challenge.scheme());
  EXPECT_TRUE(parameters.GetNext());
  EXPECT_TRUE(parameters.valid());
  EXPECT_EQ(std::string("realm"), parameters.name());
  EXPECT_EQ(std::string("foo bar"), parameters.value());
  EXPECT_FALSE(parameters.GetNext());
}

// Use multiple name=value properties with mismatching quote marks in the last
// value.
TEST(HttpAuthTest, ChallengeTokenizerMismatchedQuotesMultiple) {
  std::string challenge_str = "Digest qop=auth-int, algorithm=md5, realm=\"foo";
  HttpAuth::ChallengeTokenizer challenge(challenge_str.begin(),
                                         challenge_str.end());
  HttpUtil::NameValuePairsIterator parameters = challenge.param_pairs();

  EXPECT_TRUE(parameters.valid());
  EXPECT_EQ(std::string("Digest"), challenge.scheme());
  EXPECT_TRUE(parameters.GetNext());
  EXPECT_TRUE(parameters.valid());
  EXPECT_EQ(std::string("qop"), parameters.name());
  EXPECT_EQ(std::string("auth-int"), parameters.value());
  EXPECT_TRUE(parameters.GetNext());
  EXPECT_TRUE(parameters.valid());
  EXPECT_EQ(std::string("algorithm"), parameters.name());
  EXPECT_EQ(std::string("md5"), parameters.value());
  EXPECT_TRUE(parameters.GetNext());
  EXPECT_TRUE(parameters.valid());
  EXPECT_EQ(std::string("realm"), parameters.name());
  EXPECT_EQ(std::string("foo"), parameters.value());
  EXPECT_FALSE(parameters.GetNext());
}

// Use a name= property which has no value.
TEST(HttpAuthTest, ChallengeTokenizerNoValue) {
  std::string challenge_str = "Digest qop=";
  HttpAuth::ChallengeTokenizer challenge(
      challenge_str.begin(), challenge_str.end());
  HttpUtil::NameValuePairsIterator parameters = challenge.param_pairs();

  EXPECT_TRUE(parameters.valid());
  EXPECT_EQ(std::string("Digest"), challenge.scheme());
  EXPECT_FALSE(parameters.GetNext());
  EXPECT_FALSE(parameters.valid());
}

// Specify multiple properties, comma separated.
TEST(HttpAuthTest, ChallengeTokenizerMultiple) {
  std::string challenge_str =
      "Digest algorithm=md5, realm=\"Oblivion\", qop=auth-int";
  HttpAuth::ChallengeTokenizer challenge(challenge_str.begin(),
                                         challenge_str.end());
  HttpUtil::NameValuePairsIterator parameters = challenge.param_pairs();

  EXPECT_TRUE(parameters.valid());
  EXPECT_EQ(std::string("Digest"), challenge.scheme());
  EXPECT_TRUE(parameters.GetNext());
  EXPECT_TRUE(parameters.valid());
  EXPECT_EQ(std::string("algorithm"), parameters.name());
  EXPECT_EQ(std::string("md5"), parameters.value());
  EXPECT_TRUE(parameters.GetNext());
  EXPECT_TRUE(parameters.valid());
  EXPECT_EQ(std::string("realm"), parameters.name());
  EXPECT_EQ(std::string("Oblivion"), parameters.value());
  EXPECT_TRUE(parameters.GetNext());
  EXPECT_TRUE(parameters.valid());
  EXPECT_EQ(std::string("qop"), parameters.name());
  EXPECT_EQ(std::string("auth-int"), parameters.value());
  EXPECT_FALSE(parameters.GetNext());
  EXPECT_TRUE(parameters.valid());
}

// Use a challenge which has no property.
TEST(HttpAuthTest, ChallengeTokenizerNoProperty) {
  std::string challenge_str = "NTLM";
  HttpAuth::ChallengeTokenizer challenge(
      challenge_str.begin(), challenge_str.end());
  HttpUtil::NameValuePairsIterator parameters = challenge.param_pairs();

  EXPECT_TRUE(parameters.valid());
  EXPECT_EQ(std::string("NTLM"), challenge.scheme());
  EXPECT_FALSE(parameters.GetNext());
}

// Use a challenge with Base64 encoded token.
TEST(HttpAuthTest, ChallengeTokenizerBase64) {
  std::string challenge_str = "NTLM  SGVsbG8sIFdvcmxkCg===";
  HttpAuth::ChallengeTokenizer challenge(challenge_str.begin(),
                                         challenge_str.end());

  EXPECT_EQ(std::string("NTLM"), challenge.scheme());
  // Notice the two equal statements below due to padding removal.
  EXPECT_EQ(std::string("SGVsbG8sIFdvcmxkCg=="), challenge.base64_param());
}

TEST(HttpAuthTest, GetChallengeHeaderName) {
  std::string name;

  name = HttpAuth::GetChallengeHeaderName(HttpAuth::AUTH_SERVER);
  EXPECT_STREQ("WWW-Authenticate", name.c_str());

  name = HttpAuth::GetChallengeHeaderName(HttpAuth::AUTH_PROXY);
  EXPECT_STREQ("Proxy-Authenticate", name.c_str());
}

TEST(HttpAuthTest, GetAuthorizationHeaderName) {
  std::string name;

  name = HttpAuth::GetAuthorizationHeaderName(HttpAuth::AUTH_SERVER);
  EXPECT_STREQ("Authorization", name.c_str());

  name = HttpAuth::GetAuthorizationHeaderName(HttpAuth::AUTH_PROXY);
  EXPECT_STREQ("Proxy-Authorization", name.c_str());
}

}  // namespace net
