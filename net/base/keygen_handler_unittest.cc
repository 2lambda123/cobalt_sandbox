// Copyright (c) 2010 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "net/base/keygen_handler.h"

#include "build/build_config.h" // Needs to be imported early for USE_NSS

#if defined(USE_NSS)
#include <private/pprthred.h>  // PR_DetachThread
#endif

#include <string>

#include "base/base64.h"
#include "base/logging.h"
#include "base/nss_util.h"
#include "base/task.h"
#include "base/waitable_event.h"
#include "base/worker_pool.h"
#include "testing/gtest/include/gtest/gtest.h"

namespace net {

namespace {

class KeygenHandlerTest : public ::testing::Test {
 public:
  KeygenHandlerTest() {}
  virtual ~KeygenHandlerTest() {}

  virtual void SetUp() {
#if defined(OS_CHROMEOS)
  base::OpenPersistentNSSDB();
#endif
  }
};

// Assert that |result| is a valid output for KeygenHandler given challenge
// string of |challenge|.
void AssertValidSignedPublicKeyAndChallenge(const std::string& result,
                                            const std::string& challenge) {
  ASSERT_GT(result.length(), 0U);

  // Verify it's valid base64:
  std::string spkac;
  ASSERT_TRUE(base::Base64Decode(result, &spkac));
  // In lieu of actually parsing and validating the DER data,
  // just check that it exists and has a reasonable length.
  // (It's almost always 590 bytes, but the DER encoding of the random key
  // and signature could sometimes be a few bytes different.)
  ASSERT_GE(spkac.length(), 580U);
  ASSERT_LE(spkac.length(), 600U);

  // NOTE:
  // The value of |result| can be validated by prefixing 'SPKAC=' to it
  // and piping it through
  //   openssl spkac -verify
  // whose output should look like:
  //   Netscape SPKI:
  //     Public Key Algorithm: rsaEncryption
  //     RSA Public Key: (2048 bit)
  //     Modulus (2048 bit):
  //         00:b6:cc:14:c9:43:b5:2d:51:65:7e:11:8b:80:9e: .....
  //     Exponent: 65537 (0x10001)
  //     Challenge String: some challenge
  //     Signature Algorithm: md5WithRSAEncryption
  //         92:f3:cc:ff:0b:d3:d0:4a:3a:4c:ba:ff:d6:38:7f:a5:4b:b5: .....
  //   Signature OK
  //
  // The value of |spkac| can be ASN.1-parsed with:
  //    openssl asn1parse -inform DER
}

TEST_F(KeygenHandlerTest, FLAKY_SmokeTest) {
  KeygenHandler handler(2048, "some challenge");
  handler.set_stores_key(false);  // Don't leave the key-pair behind
  std::string result = handler.GenKeyAndSignChallenge();
  LOG(INFO) << "KeygenHandler produced: " << result;
  AssertValidSignedPublicKeyAndChallenge(result, "some challenge");
}

class ConcurrencyTestTask : public Task {
 public:
  ConcurrencyTestTask(base::WaitableEvent* event,
                      const std::string& challenge, std::string* result)
      : event_(event),
        challenge_(challenge),
        result_(result) {
  }

  virtual void Run() {
    KeygenHandler handler(2048, "some challenge");
    handler.set_stores_key(false); // Don't leave the key-pair behind.
    *result_ = handler.GenKeyAndSignChallenge();
    event_->Signal();
#if defined(USE_NSS)
    // Detach the thread from NSPR.
    // Calling NSS functions attaches the thread to NSPR, which stores
    // the NSPR thread ID in thread-specific data.
    // The threads in our thread pool terminate after we have called
    // PR_Cleanup.  Unless we detach them from NSPR, net_unittests gets
    // segfaults on shutdown when the threads' thread-specific data
    // destructors run.
    PR_DetachThread();
#endif
  }

 private:
  base::WaitableEvent* event_;
  std::string challenge_;
  std::string* result_;
};

// We asynchronously generate the keys so as not to hang up the IO thread. This
// test tries to catch concurrency problems in the keygen implementation.
TEST_F(KeygenHandlerTest, ConcurrencyTest) {
  const int NUM_HANDLERS = 5;
  base::WaitableEvent* events[NUM_HANDLERS] = { NULL };
  std::string results[NUM_HANDLERS];
  for (int i = 0; i < NUM_HANDLERS; i++) {
    events[i] = new base::WaitableEvent(false, false);
    WorkerPool::PostTask(FROM_HERE,
                         new ConcurrencyTestTask(events[i], "some challenge",
                                                 &results[i]),
                         true);
  }

  for (int i = 0; i < NUM_HANDLERS; i++) {
    // Make sure the job completed
    bool signaled = events[i]->Wait();
    EXPECT_TRUE(signaled);
    delete events[i];
    events[i] = NULL;

    LOG(INFO) << "KeygenHandler " << i << " produced: " << results[i];
    AssertValidSignedPublicKeyAndChallenge(results[i], "some challenge");
  }
}

}  // namespace

}  // namespace net
