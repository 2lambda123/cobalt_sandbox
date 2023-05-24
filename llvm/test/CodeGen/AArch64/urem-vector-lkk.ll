; NOTE: Assertions have been autogenerated by utils/update_llc_test_checks.py
; RUN: llc -mtriple=aarch64-unknown-linux-gnu < %s | FileCheck %s

define <4 x i16> @fold_urem_vec_1(<4 x i16> %x) {
; CHECK-LABEL: fold_urem_vec_1:
; CHECK:       // %bb.0:
; CHECK-NEXT:    mov w11, #33437
; CHECK-NEXT:    // kill: def $d0 killed $d0 def $q0
; CHECK-NEXT:    umov w10, v0.h[2]
; CHECK-NEXT:    movk w11, #21399, lsl #16
; CHECK-NEXT:    umull x11, w10, w11
; CHECK-NEXT:    umov w8, v0.h[1]
; CHECK-NEXT:    mov w9, #16913
; CHECK-NEXT:    mov w12, #98
; CHECK-NEXT:    lsr x11, x11, #37
; CHECK-NEXT:    movk w9, #8456, lsl #16
; CHECK-NEXT:    msub w10, w11, w12, w10
; CHECK-NEXT:    ubfx w12, w8, #2, #14
; CHECK-NEXT:    umull x9, w12, w9
; CHECK-NEXT:    mov w11, #124
; CHECK-NEXT:    lsr x9, x9, #34
; CHECK-NEXT:    msub w8, w9, w11, w8
; CHECK-NEXT:    mov w9, #8969
; CHECK-NEXT:    umov w12, v0.h[0]
; CHECK-NEXT:    movk w9, #22765, lsl #16
; CHECK-NEXT:    umull x9, w12, w9
; CHECK-NEXT:    lsr x9, x9, #32
; CHECK-NEXT:    sub w11, w12, w9
; CHECK-NEXT:    add w9, w9, w11, lsr #1
; CHECK-NEXT:    mov w11, #95
; CHECK-NEXT:    lsr w9, w9, #6
; CHECK-NEXT:    msub w9, w9, w11, w12
; CHECK-NEXT:    umov w11, v0.h[3]
; CHECK-NEXT:    fmov s0, w9
; CHECK-NEXT:    mov w9, #2287
; CHECK-NEXT:    movk w9, #16727, lsl #16
; CHECK-NEXT:    umull x9, w11, w9
; CHECK-NEXT:    mov v0.h[1], w8
; CHECK-NEXT:    mov w8, #1003
; CHECK-NEXT:    lsr x9, x9, #40
; CHECK-NEXT:    mov v0.h[2], w10
; CHECK-NEXT:    msub w8, w9, w8, w11
; CHECK-NEXT:    mov v0.h[3], w8
; CHECK-NEXT:    // kill: def $d0 killed $d0 killed $q0
; CHECK-NEXT:    ret
  %1 = urem <4 x i16> %x, <i16 95, i16 124, i16 98, i16 1003>
  ret <4 x i16> %1
}

define <4 x i16> @fold_urem_vec_2(<4 x i16> %x) {
; CHECK-LABEL: fold_urem_vec_2:
; CHECK:       // %bb.0:
; CHECK-NEXT:    mov w9, #8969
; CHECK-NEXT:    // kill: def $d0 killed $d0 def $q0
; CHECK-NEXT:    umov w8, v0.h[1]
; CHECK-NEXT:    movk w9, #22765, lsl #16
; CHECK-NEXT:    umov w10, v0.h[0]
; CHECK-NEXT:    umull x13, w8, w9
; CHECK-NEXT:    umov w11, v0.h[2]
; CHECK-NEXT:    umull x14, w10, w9
; CHECK-NEXT:    lsr x13, x13, #32
; CHECK-NEXT:    umov w12, v0.h[3]
; CHECK-NEXT:    umull x15, w11, w9
; CHECK-NEXT:    lsr x14, x14, #32
; CHECK-NEXT:    sub w16, w8, w13
; CHECK-NEXT:    umull x9, w12, w9
; CHECK-NEXT:    lsr x15, x15, #32
; CHECK-NEXT:    add w13, w13, w16, lsr #1
; CHECK-NEXT:    sub w16, w10, w14
; CHECK-NEXT:    lsr x9, x9, #32
; CHECK-NEXT:    add w14, w14, w16, lsr #1
; CHECK-NEXT:    sub w16, w11, w15
; CHECK-NEXT:    add w15, w15, w16, lsr #1
; CHECK-NEXT:    sub w16, w12, w9
; CHECK-NEXT:    add w9, w9, w16, lsr #1
; CHECK-NEXT:    mov w16, #95
; CHECK-NEXT:    lsr w13, w13, #6
; CHECK-NEXT:    msub w8, w13, w16, w8
; CHECK-NEXT:    lsr w13, w14, #6
; CHECK-NEXT:    msub w10, w13, w16, w10
; CHECK-NEXT:    lsr w13, w15, #6
; CHECK-NEXT:    fmov s0, w10
; CHECK-NEXT:    msub w11, w13, w16, w11
; CHECK-NEXT:    lsr w9, w9, #6
; CHECK-NEXT:    mov v0.h[1], w8
; CHECK-NEXT:    mov v0.h[2], w11
; CHECK-NEXT:    msub w8, w9, w16, w12
; CHECK-NEXT:    mov v0.h[3], w8
; CHECK-NEXT:    // kill: def $d0 killed $d0 killed $q0
; CHECK-NEXT:    ret
  %1 = urem <4 x i16> %x, <i16 95, i16 95, i16 95, i16 95>
  ret <4 x i16> %1
}


; Don't fold if we can combine urem with udiv.
define <4 x i16> @combine_urem_udiv(<4 x i16> %x) {
; CHECK-LABEL: combine_urem_udiv:
; CHECK:       // %bb.0:
; CHECK-NEXT:    mov w8, #8969
; CHECK-NEXT:    // kill: def $d0 killed $d0 def $q0
; CHECK-NEXT:    movk w8, #22765, lsl #16
; CHECK-NEXT:    umov w9, v0.h[1]
; CHECK-NEXT:    umov w10, v0.h[0]
; CHECK-NEXT:    umull x13, w9, w8
; CHECK-NEXT:    umov w11, v0.h[2]
; CHECK-NEXT:    umull x14, w10, w8
; CHECK-NEXT:    lsr x13, x13, #32
; CHECK-NEXT:    umov w12, v0.h[3]
; CHECK-NEXT:    umull x15, w11, w8
; CHECK-NEXT:    lsr x14, x14, #32
; CHECK-NEXT:    sub w16, w9, w13
; CHECK-NEXT:    umull x8, w12, w8
; CHECK-NEXT:    lsr x15, x15, #32
; CHECK-NEXT:    add w13, w13, w16, lsr #1
; CHECK-NEXT:    sub w16, w10, w14
; CHECK-NEXT:    lsr x8, x8, #32
; CHECK-NEXT:    add w14, w14, w16, lsr #1
; CHECK-NEXT:    sub w16, w11, w15
; CHECK-NEXT:    add w15, w15, w16, lsr #1
; CHECK-NEXT:    sub w16, w12, w8
; CHECK-NEXT:    add w8, w8, w16, lsr #1
; CHECK-NEXT:    mov w16, #95
; CHECK-NEXT:    lsr w14, w14, #6
; CHECK-NEXT:    lsr w13, w13, #6
; CHECK-NEXT:    msub w10, w14, w16, w10
; CHECK-NEXT:    lsr w15, w15, #6
; CHECK-NEXT:    msub w9, w13, w16, w9
; CHECK-NEXT:    fmov s0, w14
; CHECK-NEXT:    fmov s1, w10
; CHECK-NEXT:    lsr w8, w8, #6
; CHECK-NEXT:    msub w11, w15, w16, w11
; CHECK-NEXT:    mov v0.h[1], w13
; CHECK-NEXT:    mov v1.h[1], w9
; CHECK-NEXT:    msub w12, w8, w16, w12
; CHECK-NEXT:    mov v0.h[2], w15
; CHECK-NEXT:    mov v1.h[2], w11
; CHECK-NEXT:    mov v1.h[3], w12
; CHECK-NEXT:    mov v0.h[3], w8
; CHECK-NEXT:    add v0.4h, v1.4h, v0.4h
; CHECK-NEXT:    ret
  %1 = urem <4 x i16> %x, <i16 95, i16 95, i16 95, i16 95>
  %2 = udiv <4 x i16> %x, <i16 95, i16 95, i16 95, i16 95>
  %3 = add <4 x i16> %1, %2
  ret <4 x i16> %3
}


; Don't fold for divisors that are a power of two.
define <4 x i16> @dont_fold_urem_power_of_two(<4 x i16> %x) {
; CHECK-LABEL: dont_fold_urem_power_of_two:
; CHECK:       // %bb.0:
; CHECK-NEXT:    mov w9, #8969
; CHECK-NEXT:    // kill: def $d0 killed $d0 def $q0
; CHECK-NEXT:    umov w8, v0.h[3]
; CHECK-NEXT:    movk w9, #22765, lsl #16
; CHECK-NEXT:    umull x9, w8, w9
; CHECK-NEXT:    lsr x9, x9, #32
; CHECK-NEXT:    sub w10, w8, w9
; CHECK-NEXT:    add w9, w9, w10, lsr #1
; CHECK-NEXT:    mov w10, #95
; CHECK-NEXT:    lsr w9, w9, #6
; CHECK-NEXT:    msub w8, w9, w10, w8
; CHECK-NEXT:    umov w9, v0.h[0]
; CHECK-NEXT:    and w9, w9, #0x3f
; CHECK-NEXT:    umov w10, v0.h[1]
; CHECK-NEXT:    fmov s1, w9
; CHECK-NEXT:    umov w9, v0.h[2]
; CHECK-NEXT:    and w10, w10, #0x1f
; CHECK-NEXT:    and w9, w9, #0x7
; CHECK-NEXT:    mov v1.h[1], w10
; CHECK-NEXT:    mov v1.h[2], w9
; CHECK-NEXT:    mov v1.h[3], w8
; CHECK-NEXT:    mov v0.16b, v1.16b
; CHECK-NEXT:    ret
  %1 = urem <4 x i16> %x, <i16 64, i16 32, i16 8, i16 95>
  ret <4 x i16> %1
}

; Don't fold if the divisor is one.
define <4 x i16> @dont_fold_srem_one(<4 x i16> %x) {
; CHECK-LABEL: dont_fold_srem_one:
; CHECK:       // %bb.0:
; CHECK-NEXT:    mov w9, #17097
; CHECK-NEXT:    // kill: def $d0 killed $d0 def $q0
; CHECK-NEXT:    umov w8, v0.h[2]
; CHECK-NEXT:    movk w9, #45590, lsl #16
; CHECK-NEXT:    umull x9, w8, w9
; CHECK-NEXT:    mov w10, #23
; CHECK-NEXT:    lsr x9, x9, #36
; CHECK-NEXT:    umov w11, v0.h[1]
; CHECK-NEXT:    msub w8, w9, w10, w8
; CHECK-NEXT:    mov w9, #30865
; CHECK-NEXT:    movk w9, #51306, lsl #16
; CHECK-NEXT:    ubfx w10, w11, #1, #15
; CHECK-NEXT:    umull x9, w10, w9
; CHECK-NEXT:    mov w10, #654
; CHECK-NEXT:    lsr x9, x9, #40
; CHECK-NEXT:    msub w9, w9, w10, w11
; CHECK-NEXT:    mov w11, #47143
; CHECK-NEXT:    umov w10, v0.h[3]
; CHECK-NEXT:    movk w11, #24749, lsl #16
; CHECK-NEXT:    movi d1, #0000000000000000
; CHECK-NEXT:    umull x11, w10, w11
; CHECK-NEXT:    mov v1.h[1], w9
; CHECK-NEXT:    mov w9, #5423
; CHECK-NEXT:    lsr x11, x11, #43
; CHECK-NEXT:    mov v1.h[2], w8
; CHECK-NEXT:    msub w8, w11, w9, w10
; CHECK-NEXT:    mov v1.h[3], w8
; CHECK-NEXT:    mov v0.16b, v1.16b
; CHECK-NEXT:    ret
  %1 = urem <4 x i16> %x, <i16 1, i16 654, i16 23, i16 5423>
  ret <4 x i16> %1
}

; Don't fold if the divisor is 2^16.
define <4 x i16> @dont_fold_urem_i16_smax(<4 x i16> %x) {
; CHECK-LABEL: dont_fold_urem_i16_smax:
; CHECK:       // %bb.0:
; CHECK-NEXT:    ret
  %1 = urem <4 x i16> %x, <i16 1, i16 65536, i16 23, i16 5423>
  ret <4 x i16> %1
}

; Don't fold i64 urem.
define <4 x i64> @dont_fold_urem_i64(<4 x i64> %x) {
; CHECK-LABEL: dont_fold_urem_i64:
; CHECK:       // %bb.0:
; CHECK-NEXT:    mov x10, #12109
; CHECK-NEXT:    movk x10, #52170, lsl #16
; CHECK-NEXT:    movk x10, #28749, lsl #32
; CHECK-NEXT:    mov x8, v1.d[1]
; CHECK-NEXT:    movk x10, #49499, lsl #48
; CHECK-NEXT:    umulh x10, x8, x10
; CHECK-NEXT:    mov w11, #5423
; CHECK-NEXT:    lsr x10, x10, #12
; CHECK-NEXT:    msub x8, x10, x11, x8
; CHECK-NEXT:    mov x10, #21445
; CHECK-NEXT:    movk x10, #1603, lsl #16
; CHECK-NEXT:    mov x12, v0.d[1]
; CHECK-NEXT:    movk x10, #15432, lsl #32
; CHECK-NEXT:    movk x10, #25653, lsl #48
; CHECK-NEXT:    lsr x11, x12, #1
; CHECK-NEXT:    umulh x10, x11, x10
; CHECK-NEXT:    mov w11, #654
; CHECK-NEXT:    lsr x10, x10, #7
; CHECK-NEXT:    msub x10, x10, x11, x12
; CHECK-NEXT:    mov x11, #17097
; CHECK-NEXT:    movk x11, #45590, lsl #16
; CHECK-NEXT:    movk x11, #34192, lsl #32
; CHECK-NEXT:    fmov x9, d1
; CHECK-NEXT:    movk x11, #25644, lsl #48
; CHECK-NEXT:    umulh x11, x9, x11
; CHECK-NEXT:    sub x12, x9, x11
; CHECK-NEXT:    add x11, x11, x12, lsr #1
; CHECK-NEXT:    mov w12, #23
; CHECK-NEXT:    lsr x11, x11, #4
; CHECK-NEXT:    msub x9, x11, x12, x9
; CHECK-NEXT:    movi v0.2d, #0000000000000000
; CHECK-NEXT:    fmov d1, x9
; CHECK-NEXT:    mov v1.d[1], x8
; CHECK-NEXT:    mov v0.d[1], x10
; CHECK-NEXT:    ret
  %1 = urem <4 x i64> %x, <i64 1, i64 654, i64 23, i64 5423>
  ret <4 x i64> %1
}
