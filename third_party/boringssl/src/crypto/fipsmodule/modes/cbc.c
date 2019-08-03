/* ====================================================================
 * Copyright (c) 2008 The OpenSSL Project.  All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 *
 * 3. All advertising materials mentioning features or use of this
 *    software must display the following acknowledgment:
 *    "This product includes software developed by the OpenSSL Project
 *    for use in the OpenSSL Toolkit. (http://www.openssl.org/)"
 *
 * 4. The names "OpenSSL Toolkit" and "OpenSSL Project" must not be used to
 *    endorse or promote products derived from this software without
 *    prior written permission. For written permission, please contact
 *    openssl-core@openssl.org.
 *
 * 5. Products derived from this software may not be called "OpenSSL"
 *    nor may "OpenSSL" appear in their names without prior written
 *    permission of the OpenSSL Project.
 *
 * 6. Redistributions of any form whatsoever must retain the following
 *    acknowledgment:
 *    "This product includes software developed by the OpenSSL Project
 *    for use in the OpenSSL Toolkit (http://www.openssl.org/)"
 *
 * THIS SOFTWARE IS PROVIDED BY THE OpenSSL PROJECT ``AS IS'' AND ANY
 * EXPRESSED OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE OpenSSL PROJECT OR
 * ITS CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
 * NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
 * STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED
 * OF THE POSSIBILITY OF SUCH DAMAGE.
 * ==================================================================== */

#include <assert.h>
#include <string.h>

#include "internal.h"


void CRYPTO_cbc128_encrypt(const uint8_t *in, uint8_t *out, size_t len,
                           const void *key, uint8_t ivec[16],
                           block128_f block) {
  size_t n;
  const uint8_t *iv = ivec;

  assert(key != NULL && ivec != NULL);
  assert(len == 0 || (in != NULL && out != NULL));

  if (STRICT_ALIGNMENT &&
      ((uintptr_t)in | (uintptr_t)out | (uintptr_t)ivec) % sizeof(size_t) !=
          0) {
    while (len >= 16) {
      for (n = 0; n < 16; ++n) {
        out[n] = in[n] ^ iv[n];
      }
      (*block)(out, out, key);
      iv = out;
      len -= 16;
      in += 16;
      out += 16;
    }
  } else {
    while (len >= 16) {
      for (n = 0; n < 16; n += sizeof(size_t)) {
        store_word_le(out + n, load_word_le(in + n) ^ load_word_le(iv + n));
      }
      (*block)(out, out, key);
      iv = out;
      len -= 16;
      in += 16;
      out += 16;
    }
  }

  while (len) {
    for (n = 0; n < 16 && n < len; ++n) {
      out[n] = in[n] ^ iv[n];
    }
    for (; n < 16; ++n) {
      out[n] = iv[n];
    }
    (*block)(out, out, key);
    iv = out;
    if (len <= 16) {
      break;
    }
    len -= 16;
    in += 16;
    out += 16;
  }

  OPENSSL_memcpy(ivec, iv, 16);
}

void CRYPTO_cbc128_decrypt(const uint8_t *in, uint8_t *out, size_t len,
                           const void *key, uint8_t ivec[16],
                           block128_f block) {
  size_t n;
  union {
    size_t t[16 / sizeof(size_t)];
    uint8_t c[16];
  } tmp;

  assert(key != NULL && ivec != NULL);
  assert(len == 0 || (in != NULL && out != NULL));

  const uintptr_t inptr = (uintptr_t) in;
  const uintptr_t outptr = (uintptr_t) out;
  // If |in| and |out| alias, |in| must be ahead.
  assert(inptr >= outptr || inptr + len <= outptr);

  if ((inptr >= 32 && outptr <= inptr - 32) || inptr < outptr) {
    // If |out| is at least two blocks behind |in| or completely disjoint, there
    // is no need to decrypt to a temporary block.
    const uint8_t *iv = ivec;

    if (STRICT_ALIGNMENT &&
        ((uintptr_t)in | (uintptr_t)out | (uintptr_t)ivec) % sizeof(size_t) !=
            0) {
      while (len >= 16) {
        (*block)(in, out, key);
        for (n = 0; n < 16; ++n) {
          out[n] ^= iv[n];
        }
        iv = in;
        len -= 16;
        in += 16;
        out += 16;
      }
    } else if (16 % sizeof(size_t) == 0) {  // always true
      while (len >= 16) {
        (*block)(in, out, key);
        for (n = 0; n < 16; n += sizeof(size_t)) {
          store_word_le(out + n, load_word_le(out + n) ^ load_word_le(iv + n));
        }
        iv = in;
        len -= 16;
        in += 16;
        out += 16;
      }
    }
    OPENSSL_memcpy(ivec, iv, 16);
  } else {
    // |out| is less than two blocks behind |in|. Decrypting an input block
    // directly to |out| would overwrite a ciphertext block before it is used as
    // the next block's IV. Decrypt to a temporary block instead.
    if (STRICT_ALIGNMENT &&
        ((uintptr_t)in | (uintptr_t)out | (uintptr_t)ivec) % sizeof(size_t) !=
            0) {
      uint8_t c;
      while (len >= 16) {
        (*block)(in, tmp.c, key);
        for (n = 0; n < 16; ++n) {
          c = in[n];
          out[n] = tmp.c[n] ^ ivec[n];
          ivec[n] = c;
        }
        len -= 16;
        in += 16;
        out += 16;
      }
    } else if (16 % sizeof(size_t) == 0) {  // always true
      while (len >= 16) {
        (*block)(in, tmp.c, key);
        for (n = 0; n < 16; n += sizeof(size_t)) {
          size_t c = load_word_le(in + n);
          store_word_le(out + n,
                        tmp.t[n / sizeof(size_t)] ^ load_word_le(ivec + n));
          store_word_le(ivec + n, c);
        }
        len -= 16;
        in += 16;
        out += 16;
      }
    }
  }

  while (len) {
    uint8_t c;
    (*block)(in, tmp.c, key);
    for (n = 0; n < 16 && n < len; ++n) {
      c = in[n];
      out[n] = tmp.c[n] ^ ivec[n];
      ivec[n] = c;
    }
    if (len <= 16) {
      for (; n < 16; ++n) {
        ivec[n] = in[n];
      }
      break;
    }
    len -= 16;
    in += 16;
    out += 16;
  }
}
