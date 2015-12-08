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

#ifndef NETWORK_COOKIE_JAR_IMPL_H_
#define NETWORK_COOKIE_JAR_IMPL_H_

#include <string>

#include "base/compiler_specific.h"
#include "cobalt/network_bridge/cookie_jar.h"
#include "net/cookies/cookie_store.h"

namespace cobalt {
namespace network {

class CookieJarImpl : public network_bridge::CookieJar {
 public:
  explicit CookieJarImpl(net::CookieStore* cookie_store);

  std::string GetCookies(const GURL& origin) OVERRIDE;
  void SetCookie(const GURL& origin, const std::string& cookie_line) OVERRIDE;

 private:
  net::CookieStore* cookie_store_;

  DISALLOW_COPY_AND_ASSIGN(CookieJarImpl);
};

}  // namespace network
}  // namespace cobalt

#endif  // NETWORK_COOKIE_JAR_IMPL_H_
