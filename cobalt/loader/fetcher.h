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

#ifndef LOADER_FETCHER_H_
#define LOADER_FETCHER_H_

#include <string>

namespace cobalt {
namespace loader {

class Fetcher {
 public:
  class Handler {
   public:
    virtual void OnReceived(const char* data, size_t size) = 0;
    virtual void OnDone() = 0;
    virtual void OnError(const std::string& error) = 0;
  };

  // Concrete Fetcher subclass should start fetching immediately in constructor.
  explicit Fetcher(Handler* handler) : handler_(handler) {}

  // Concrete Fetcher subclass should cancel fetching in destructor.
  virtual ~Fetcher() = 0;

 protected:
  Handler* handler() const { return handler_; }

 private:
  Handler* handler_;
};

}  // namespace loader
}  // namespace cobalt

#endif  // LOADER_FETCHER_H_
