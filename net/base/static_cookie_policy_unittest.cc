// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "net/base/net_errors.h"
#include "net/base/static_cookie_policy.h"
#include "testing/gtest/include/gtest/gtest.h"
#include "googleurl/src/gurl.h"

namespace net {

class StaticCookiePolicyTest : public testing::Test {
 public:
  StaticCookiePolicyTest()
      : url_google_("http://www.google.izzle"),
        url_google_secure_("https://www.google.izzle"),
        url_google_mail_("http://mail.google.izzle"),
        url_google_analytics_("http://www.googleanalytics.izzle") {
  }
  void SetPolicyType(StaticCookiePolicy::Type type) {
    policy_.set_type(type);
  }
  int CanGetCookies(const GURL& url, const GURL& first_party) {
    return policy_.CanGetCookies(url, first_party, NULL);
  }
  int CanSetCookie(const GURL& url, const GURL& first_party) {
    return policy_.CanSetCookie(url, first_party, std::string(), NULL);
  }
 protected:
  StaticCookiePolicy policy_;
  GURL url_google_;
  GURL url_google_secure_;
  GURL url_google_mail_;
  GURL url_google_analytics_;
};

TEST_F(StaticCookiePolicyTest, DefaultPolicyTest) {
  EXPECT_EQ(OK, CanGetCookies(url_google_, url_google_));
  EXPECT_EQ(OK, CanGetCookies(url_google_, url_google_secure_));
  EXPECT_EQ(OK, CanGetCookies(url_google_, url_google_mail_));
  EXPECT_EQ(OK, CanGetCookies(url_google_, url_google_analytics_));
  EXPECT_EQ(OK, CanGetCookies(url_google_, GURL()));

  EXPECT_EQ(OK, CanSetCookie(url_google_, url_google_));
  EXPECT_EQ(OK, CanSetCookie(url_google_, url_google_secure_));
  EXPECT_EQ(OK, CanSetCookie(url_google_, url_google_mail_));
  EXPECT_EQ(OK, CanSetCookie(url_google_, url_google_analytics_));
  EXPECT_EQ(OK, CanSetCookie(url_google_, GURL()));
}

TEST_F(StaticCookiePolicyTest, AllowAllCookiesTest) {
  SetPolicyType(StaticCookiePolicy::ALLOW_ALL_COOKIES);

  EXPECT_EQ(OK, CanGetCookies(url_google_, url_google_));
  EXPECT_EQ(OK, CanGetCookies(url_google_, url_google_secure_));
  EXPECT_EQ(OK, CanGetCookies(url_google_, url_google_mail_));
  EXPECT_EQ(OK, CanGetCookies(url_google_, url_google_analytics_));
  EXPECT_EQ(OK, CanGetCookies(url_google_, GURL()));

  EXPECT_EQ(OK, CanSetCookie(url_google_, url_google_));
  EXPECT_EQ(OK, CanSetCookie(url_google_, url_google_secure_));
  EXPECT_EQ(OK, CanSetCookie(url_google_, url_google_mail_));
  EXPECT_EQ(OK, CanSetCookie(url_google_, url_google_analytics_));
  EXPECT_EQ(OK, CanSetCookie(url_google_, GURL()));
}

TEST_F(StaticCookiePolicyTest, BlockSettingThirdPartyCookiesTest) {
  SetPolicyType(StaticCookiePolicy::BLOCK_SETTING_THIRD_PARTY_COOKIES);

  EXPECT_EQ(OK, CanGetCookies(url_google_, url_google_));
  EXPECT_EQ(OK, CanGetCookies(url_google_, url_google_secure_));
  EXPECT_EQ(OK, CanGetCookies(url_google_, url_google_mail_));
  EXPECT_EQ(OK, CanGetCookies(url_google_, url_google_analytics_));
  EXPECT_EQ(OK, CanGetCookies(url_google_, GURL()));

  EXPECT_EQ(OK, CanSetCookie(url_google_, url_google_));
  EXPECT_EQ(OK, CanSetCookie(url_google_, url_google_secure_));
  EXPECT_EQ(OK, CanSetCookie(url_google_, url_google_mail_));
  EXPECT_NE(OK, CanSetCookie(url_google_, url_google_analytics_));
  EXPECT_EQ(OK, CanSetCookie(url_google_, GURL()));
}

TEST_F(StaticCookiePolicyTest, BlockAllThirdPartyCookiesTest) {
  SetPolicyType(StaticCookiePolicy::BLOCK_ALL_THIRD_PARTY_COOKIES);

  EXPECT_EQ(OK, CanGetCookies(url_google_, url_google_));
  EXPECT_EQ(OK, CanGetCookies(url_google_, url_google_secure_));
  EXPECT_EQ(OK, CanGetCookies(url_google_, url_google_mail_));
  EXPECT_NE(OK, CanGetCookies(url_google_, url_google_analytics_));
  EXPECT_EQ(OK, CanGetCookies(url_google_, GURL()));

  EXPECT_EQ(OK, CanSetCookie(url_google_, url_google_));
  EXPECT_EQ(OK, CanSetCookie(url_google_, url_google_secure_));
  EXPECT_EQ(OK, CanSetCookie(url_google_, url_google_mail_));
  EXPECT_NE(OK, CanSetCookie(url_google_, url_google_analytics_));
  EXPECT_EQ(OK, CanSetCookie(url_google_, GURL()));
}

TEST_F(StaticCookiePolicyTest, BlockAllCookiesTest) {
  SetPolicyType(StaticCookiePolicy::BLOCK_ALL_COOKIES);

  EXPECT_NE(OK, CanGetCookies(url_google_, url_google_));
  EXPECT_NE(OK, CanGetCookies(url_google_, url_google_secure_));
  EXPECT_NE(OK, CanGetCookies(url_google_, url_google_mail_));
  EXPECT_NE(OK, CanGetCookies(url_google_, url_google_analytics_));
  EXPECT_NE(OK, CanGetCookies(url_google_, GURL()));

  EXPECT_NE(OK, CanSetCookie(url_google_, url_google_));
  EXPECT_NE(OK, CanSetCookie(url_google_, url_google_secure_));
  EXPECT_NE(OK, CanSetCookie(url_google_, url_google_mail_));
  EXPECT_NE(OK, CanSetCookie(url_google_, url_google_analytics_));
  EXPECT_NE(OK, CanSetCookie(url_google_, GURL()));
}

}  // namespace net
