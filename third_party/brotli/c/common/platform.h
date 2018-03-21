/* Copyright 2016 Google Inc. All Rights Reserved.

   Distributed under MIT license.
   See file LICENSE for detail or copy at https://opensource.org/licenses/MIT
*/

/* Macros for compiler / platform specific features and build options. */

#ifndef BROTLI_COMMON_PLATFORM_H_
#define BROTLI_COMMON_PLATFORM_H_

#if !defined(STARBOARD)
#include <string.h>  /* memcpy*/
#define MEMCPY_PLATFORM memcpy
#else
#include "starboard/memory.h"
#define MEMCPY_PLATFORM SbMemoryCopy
#endif

#include <brotli/port.h>
#include <brotli/types.h>

#if defined OS_LINUX || defined OS_CYGWIN
#include <endian.h>
#elif defined OS_FREEBSD
#include <machine/endian.h>
#elif defined OS_MACOSX
#include <machine/endian.h>
/* Let's try and follow the Linux convention */
#define BROTLI_X_BYTE_ORDER BYTE_ORDER
#define BROTLI_X_LITTLE_ENDIAN LITTLE_ENDIAN
#define BROTLI_X_BIG_ENDIAN BIG_ENDIAN
#endif

#if defined(BROTLI_ENABLE_LOG) || defined(BROTLI_DEBUG)
#include <assert.h>
#include <stdio.h>
#endif

/* Macros for compiler / platform specific features and build options.

   Build options are:
    * BROTLI_BUILD_32_BIT disables 64-bit optimizations
    * BROTLI_BUILD_64_BIT forces to use 64-bit optimizations
    * BROTLI_BUILD_BIG_ENDIAN forces to use big-endian optimizations
    * BROTLI_BUILD_ENDIAN_NEUTRAL disables endian-aware optimizations
    * BROTLI_BUILD_LITTLE_ENDIAN forces to use little-endian optimizations
    * BROTLI_BUILD_PORTABLE disables dangerous optimizations, like unaligned
      read and overlapping memcpy; this reduces decompression speed by 5%
    * BROTLI_BUILD_NO_RBIT disables "rbit" optimization for ARM CPUs
    * BROTLI_DEBUG dumps file name and line number when decoder detects stream
      or memory error
    * BROTLI_ENABLE_LOG enables asserts and dumps various state information
*/

#if BROTLI_MODERN_COMPILER || __has_attribute(always_inline)
#define BROTLI_ATTRIBUTE_ALWAYS_INLINE __attribute__ ((always_inline))
#else
#define BROTLI_ATTRIBUTE_ALWAYS_INLINE
#endif

#if defined(_WIN32) || defined(__CYGWIN__)
#define BROTLI_ATTRIBUTE_VISIBILITY_HIDDEN
#elif BROTLI_MODERN_COMPILER || __has_attribute(visibility)
#define BROTLI_ATTRIBUTE_VISIBILITY_HIDDEN \
    __attribute__ ((visibility ("hidden")))
#else
#define BROTLI_ATTRIBUTE_VISIBILITY_HIDDEN
#endif

#ifndef BROTLI_INTERNAL
#define BROTLI_INTERNAL BROTLI_ATTRIBUTE_VISIBILITY_HIDDEN
#endif

#ifndef _MSC_VER
#if defined(__cplusplus) || !defined(__STRICT_ANSI__) || \
    (defined(__STDC_VERSION__) && __STDC_VERSION__ >= 199901L)
#define BROTLI_INLINE inline BROTLI_ATTRIBUTE_ALWAYS_INLINE
#else
#define BROTLI_INLINE
#endif
#else  /* _MSC_VER */
#define BROTLI_INLINE __forceinline
#endif  /* _MSC_VER */

#if BROTLI_MODERN_COMPILER || __has_attribute(unused)
#define BROTLI_UNUSED_FUNCTION static BROTLI_INLINE __attribute__ ((unused))
#else
#define BROTLI_UNUSED_FUNCTION static BROTLI_INLINE
#endif

#if !defined(__cplusplus) && !defined(c_plusplus) && \
    (defined(__STDC_VERSION__) && __STDC_VERSION__ >= 199901L)
#define BROTLI_RESTRICT restrict
#elif BROTLI_GCC_VERSION > 295 || defined(__llvm__)
#define BROTLI_RESTRICT __restrict
#else
#define BROTLI_RESTRICT
#endif

#if BROTLI_MODERN_COMPILER || __has_attribute(noinline)
#define BROTLI_NOINLINE __attribute__((noinline))
#else
#define BROTLI_NOINLINE
#endif

#if defined(__arm__) || defined(__thumb__) || \
    defined(_M_ARM) || defined(_M_ARMT) || defined(__ARM64_ARCH_8__)
#define BROTLI_TARGET_ARM
#if (defined(__ARM_ARCH) && (__ARM_ARCH == 7)) || \
    (defined(M_ARM) && (M_ARM == 7))
#define BROTLI_TARGET_ARMV7
#endif  /* ARMv7 */
#if defined(__aarch64__) || defined(__ARM64_ARCH_8__)
#define BROTLI_TARGET_ARMV8
#endif  /* ARMv8 */
#endif  /* ARM */

#if defined(__i386) || defined(_M_IX86)
#define BROTLI_TARGET_X86
#endif

#if defined(__x86_64__) || defined(_M_X64)
#define BROTLI_TARGET_X64
#endif

#if defined(__PPC64__)
#define BROTLI_TARGET_POWERPC64
#endif

#if defined(BROTLI_BUILD_64_BIT)
#define BROTLI_64_BITS 1
#elif defined(BROTLI_BUILD_32_BIT)
#define BROTLI_64_BITS 0
#elif defined(BROTLI_TARGET_X64) || defined(BROTLI_TARGET_ARMV8) || \
    defined(BROTLI_TARGET_POWERPC64)
#define BROTLI_64_BITS 1
#else
#define BROTLI_64_BITS 0
#endif

#if (BROTLI_64_BITS)
#define brotli_reg_t uint64_t
#else
#define brotli_reg_t uint32_t
#endif

#if defined(STARBOARD)
#include "starboard/configuration.h"
#if SB_IS(BIG_ENDIAN)
#define BROTLI_BIG_ENDIAN 1
#else
#define BROTLI_LITTLE_ENDIAN 1
#endif
#else  /* not defined STARBOARD */
#if defined(BROTLI_BUILD_BIG_ENDIAN)
#define BROTLI_BIG_ENDIAN 1
#elif defined(BROTLI_BUILD_LITTLE_ENDIAN)
#define BROTLI_LITTLE_ENDIAN 1
#elif defined(BROTLI_BUILD_ENDIAN_NEUTRAL)
/* Just break elif chain. */
#elif defined(__BYTE_ORDER__) && (__BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__)
#define BROTLI_LITTLE_ENDIAN 1
#elif defined(_WIN32) || defined(BROTLI_TARGET_X64)
/* Win32 & x64 can currently always be assumed to be little endian */
#define BROTLI_LITTLE_ENDIAN 1
#elif defined(__BYTE_ORDER__) && (__BYTE_ORDER__ == __ORDER_BIG_ENDIAN__)
#define BROTLI_BIG_ENDIAN 1
#elif defined(BROTLI_X_BYTE_ORDER)
#if BROTLI_X_BYTE_ORDER == BROTLI_X_LITTLE_ENDIAN
#define BROTLI_LITTLE_ENDIAN 1
#elif BROTLI_X_BYTE_ORDER == BROTLI_X_BIG_ENDIAN
#define BROTLI_BIG_ENDIAN 1
#endif
#endif  /* BROTLI_X_BYTE_ORDER */
#endif  /* STARBOARD */

#if !defined(BROTLI_LITTLE_ENDIAN)
#define BROTLI_LITTLE_ENDIAN 0
#endif

#if !defined(BROTLI_BIG_ENDIAN)
#define BROTLI_BIG_ENDIAN 0
#endif

#ifdef BROTLI_X_BYTE_ORDER
#undef BROTLI_X_BYTE_ORDER
#undef BROTLI_X_LITTLE_ENDIAN
#undef BROTLI_X_BIG_ENDIAN
#endif

#ifdef BROTLI_BUILD_PORTABLE
#define BROTLI_ALIGNED_READ (!!1)
#elif defined(BROTLI_TARGET_X86) || defined(BROTLI_TARGET_X64) || \
     defined(BROTLI_TARGET_ARMV7) || defined(BROTLI_TARGET_ARMV8)
/* Allow unaligned read only for white-listed CPUs. */
#define BROTLI_ALIGNED_READ (!!0)
#else
#define BROTLI_ALIGNED_READ (!!1)
#endif

#if BROTLI_ALIGNED_READ
/* Portable unaligned memory access: read / write values via memcpy. */
static BROTLI_INLINE uint16_t BrotliUnalignedRead16(const void* p) {
  uint16_t t;
  MEMCPY_PLATFORM(&t, p, sizeof t);
  return t;
}
static BROTLI_INLINE uint32_t BrotliUnalignedRead32(const void* p) {
  uint32_t t;
  MEMCPY_PLATFORM(&t, p, sizeof t);
  return t;
}
static BROTLI_INLINE uint64_t BrotliUnalignedRead64(const void* p) {
  uint64_t t;
  MEMCPY_PLATFORM(&t, p, sizeof t);
  return t;
}
static BROTLI_INLINE void BrotliUnalignedWrite64(void* p, uint64_t v) {
  MEMCPY_PLATFORM(p, &v, sizeof v);
}
#else /* BROTLI_ALIGNED_READ */
/* Unaligned memory access is allowed: just cast pointer to requested type. */
static BROTLI_INLINE uint16_t BrotliUnalignedRead16(const void* p) {
  return *(const uint16_t*)p;
}
static BROTLI_INLINE uint32_t BrotliUnalignedRead32(const void* p) {
  return *(const uint32_t*)p;
}
static BROTLI_INLINE uint64_t BrotliUnalignedRead64(const void* p) {
  return *(const uint64_t*)p;
}
static BROTLI_INLINE void BrotliUnalignedWrite64(void* p, uint64_t v) {
  *(uint64_t*)p = v;
}
#endif /* BROTLI_ALIGNED_READ */

#if BROTLI_LITTLE_ENDIAN
/* Straight endianness. Just read / write values. */
#define BROTLI_UNALIGNED_LOAD16LE BrotliUnalignedRead16
#define BROTLI_UNALIGNED_LOAD32LE BrotliUnalignedRead32
#define BROTLI_UNALIGNED_LOAD64LE BrotliUnalignedRead64
#define BROTLI_UNALIGNED_STORE64LE BrotliUnalignedWrite64
#elif BROTLI_BIG_ENDIAN  /* BROTLI_LITTLE_ENDIAN */
/* Explain compiler to byte-swap values. */
#define BROTLI_BSWAP16_(V) ((uint16_t)( \
  (((V) & 0xFFU) << 8) | \
  (((V) >> 8) & 0xFFU)))
static BROTLI_INLINE uint16_t BROTLI_UNALIGNED_LOAD16LE(const void* p) {
  uint16_t value = BrotliUnalignedRead16(p);
  return BROTLI_BSWAP16_(value);
}
#define BROTLI_BSWAP32_(V) ( \
  (((V) & 0xFFU) << 24) | (((V) & 0xFF00U) << 8) | \
  (((V) >> 8) & 0xFF00U) | (((V) >> 24) & 0xFFU))
static BROTLI_INLINE uint32_t BROTLI_UNALIGNED_LOAD32LE(const void* p) {
  uint32_t value = BrotliUnalignedRead32(p);
  return BROTLI_BSWAP32_(value);
}
#define BROTLI_BSWAP64_(V) ( \
  (((V) & 0xFFU) << 56) | (((V) & 0xFF00U) << 40) | \
  (((V) & 0xFF0000U) << 24) | (((V) & 0xFF000000U) << 8) | \
  (((V) >> 8) & 0xFF000000U) | (((V) >> 24) & 0xFF0000U) | \
  (((V) >> 40) & 0xFF00U) | (((V) >> 56) & 0xFFU))
static BROTLI_INLINE uint64_t BROTLI_UNALIGNED_LOAD64LE(const void* p) {
  uint64_t value = BrotliUnalignedRead64(p);
  return BROTLI_BSWAP64_(value);
}
static BROTLI_INLINE void BROTLI_UNALIGNED_STORE64LE(void* p, uint64_t v) {
  uint64_t value = BROTLI_BSWAP64_(v);
  BrotliUnalignedWrite64(p, value);
}
#else  /* BROTLI_LITTLE_ENDIAN */
/* Read / store values byte-wise; hopefully compiler will understand. */
static BROTLI_INLINE uint16_t BROTLI_UNALIGNED_LOAD16LE(const void* p) {
  const uint8_t* in = (const uint8_t*)p;
  return (uint16_t)(in[0] | (in[1] << 8));
}
static BROTLI_INLINE uint32_t BROTLI_UNALIGNED_LOAD32LE(const void* p) {
  const uint8_t* in = (const uint8_t*)p;
  uint32_t value = (uint32_t)(in[0]);
  value |= (uint32_t)(in[1]) << 8;
  value |= (uint32_t)(in[2]) << 16;
  value |= (uint32_t)(in[3]) << 24;
  return value;
}
static BROTLI_INLINE uint64_t BROTLI_UNALIGNED_LOAD64LE(const void* p) {
  const uint8_t* in = (const uint8_t*)p;
  uint64_t value = (uint64_t)(in[0]);
  value |= (uint64_t)(in[1]) << 8;
  value |= (uint64_t)(in[2]) << 16;
  value |= (uint64_t)(in[3]) << 24;
  value |= (uint64_t)(in[4]) << 32;
  value |= (uint64_t)(in[5]) << 40;
  value |= (uint64_t)(in[6]) << 48;
  value |= (uint64_t)(in[7]) << 56;
  return value;
}
static BROTLI_INLINE void BROTLI_UNALIGNED_STORE64LE(void* p, uint64_t v) {
  uint8_t* out = (uint8_t*)p;
  out[0] = (uint8_t)v;
  out[1] = (uint8_t)(v >> 8);
  out[2] = (uint8_t)(v >> 16);
  out[3] = (uint8_t)(v >> 24);
  out[4] = (uint8_t)(v >> 32);
  out[5] = (uint8_t)(v >> 40);
  out[6] = (uint8_t)(v >> 48);
  out[7] = (uint8_t)(v >> 56);
}
#endif  /* BROTLI_LITTLE_ENDIAN */

/* Define "BROTLI_PREDICT_TRUE" and "BROTLI_PREDICT_FALSE" macros for capable
   compilers.

To apply compiler hint, enclose the branching condition into macros, like this:

  if (BROTLI_PREDICT_TRUE(zero == 0)) {
    // main execution path
  } else {
    // compiler should place this code outside of main execution path
  }

OR:

  if (BROTLI_PREDICT_FALSE(something_rare_or_unexpected_happens)) {
    // compiler should place this code outside of main execution path
  }

*/
#if BROTLI_MODERN_COMPILER || __has_builtin(__builtin_expect)
#define BROTLI_PREDICT_TRUE(x) (__builtin_expect(!!(x), 1))
#define BROTLI_PREDICT_FALSE(x) (__builtin_expect(x, 0))
#else
#define BROTLI_PREDICT_FALSE(x) (x)
#define BROTLI_PREDICT_TRUE(x) (x)
#endif

/* BROTLI_IS_CONSTANT macros returns true for compile-time constants. */
#if BROTLI_MODERN_COMPILER || __has_builtin(__builtin_constant_p)
#define BROTLI_IS_CONSTANT(x) (!!__builtin_constant_p(x))
#else
#define BROTLI_IS_CONSTANT(x) (!!0)
#endif

#if defined(BROTLI_TARGET_ARM)
#define BROTLI_HAS_UBFX (!!1)
#else
#define BROTLI_HAS_UBFX (!!0)
#endif

#ifdef BROTLI_ENABLE_LOG
#define BROTLI_DCHECK(x) assert(x)
#define BROTLI_LOG(x) printf x
#else
#define BROTLI_DCHECK(x)
#define BROTLI_LOG(x)
#endif

#if defined(BROTLI_DEBUG) || defined(BROTLI_ENABLE_LOG)
static BROTLI_INLINE void BrotliDump(const char* f, int l, const char* fn) {
  fprintf(stderr, "%s:%d (%s)\n", f, l, fn);
  fflush(stderr);
}
#define BROTLI_DUMP() BrotliDump(__FILE__, __LINE__, __FUNCTION__)
#else
#define BROTLI_DUMP() (void)(0)
#endif

#if (BROTLI_MODERN_COMPILER || defined(__llvm__)) && \
    !defined(BROTLI_BUILD_NO_RBIT)
#if defined(BROTLI_TARGET_ARMV7) || defined(BROTLI_TARGET_ARMV8)
/* TODO: detect ARMv6T2 and enable this code for it. */
static BROTLI_INLINE brotli_reg_t BrotliRBit(brotli_reg_t input) {
  brotli_reg_t output;
  __asm__("rbit %0, %1\n" : "=r"(output) : "r"(input));
  return output;
}
#define BROTLI_RBIT(x) BrotliRBit(x)
#endif  /* armv7 */
#endif  /* gcc || clang */
#if !defined(BROTLI_RBIT)
static BROTLI_INLINE void BrotliRBit(void) { /* Should break build if used. */ }
#endif  /* BROTLI_RBIT */

#define BROTLI_REPEAT(N, X) {     \
  if ((N & 1) != 0) {X;}          \
  if ((N & 2) != 0) {X; X;}       \
  if ((N & 4) != 0) {X; X; X; X;} \
}

#define BROTLI_UNUSED(X) (void)(X)

#define BROTLI_MIN_MAX(T)                                                      \
  static BROTLI_INLINE T brotli_min_ ## T (T a, T b) { return a < b ? a : b; } \
  static BROTLI_INLINE T brotli_max_ ## T (T a, T b) { return a > b ? a : b; }
BROTLI_MIN_MAX(double) BROTLI_MIN_MAX(float) BROTLI_MIN_MAX(int)
BROTLI_MIN_MAX(size_t) BROTLI_MIN_MAX(uint32_t) BROTLI_MIN_MAX(uint8_t)
#undef BROTLI_MIN_MAX
#define BROTLI_MIN(T, A, B) (brotli_min_ ## T((A), (B)))
#define BROTLI_MAX(T, A, B) (brotli_max_ ## T((A), (B)))

#define BROTLI_SWAP(T, A, I, J) { \
  T __brotli_swap_tmp = (A)[(I)]; \
  (A)[(I)] = (A)[(J)];            \
  (A)[(J)] = __brotli_swap_tmp;   \
}

BROTLI_UNUSED_FUNCTION void BrotliSuppressUnusedFunctions(void) {
  BROTLI_UNUSED(BrotliSuppressUnusedFunctions);
  BROTLI_UNUSED(BrotliUnalignedRead16);
  BROTLI_UNUSED(BrotliUnalignedRead32);
  BROTLI_UNUSED(BrotliUnalignedRead64);
  BROTLI_UNUSED(BrotliUnalignedWrite64);
  BROTLI_UNUSED(BROTLI_UNALIGNED_LOAD16LE);
  BROTLI_UNUSED(BROTLI_UNALIGNED_LOAD32LE);
  BROTLI_UNUSED(BROTLI_UNALIGNED_LOAD64LE);
  BROTLI_UNUSED(BROTLI_UNALIGNED_STORE64LE);
  BROTLI_UNUSED(BrotliRBit);
  BROTLI_UNUSED(brotli_min_double);
  BROTLI_UNUSED(brotli_max_double);
  BROTLI_UNUSED(brotli_min_float);
  BROTLI_UNUSED(brotli_max_float);
  BROTLI_UNUSED(brotli_min_int);
  BROTLI_UNUSED(brotli_max_int);
  BROTLI_UNUSED(brotli_min_size_t);
  BROTLI_UNUSED(brotli_max_size_t);
  BROTLI_UNUSED(brotli_min_uint32_t);
  BROTLI_UNUSED(brotli_max_uint32_t);
  BROTLI_UNUSED(brotli_min_uint8_t);
  BROTLI_UNUSED(brotli_max_uint8_t);
}

#undef MEMCPY_PLATFORM

#endif  /* BROTLI_COMMON_PLATFORM_H_ */
