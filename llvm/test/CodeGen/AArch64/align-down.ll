; NOTE: Assertions have been autogenerated by utils/update_llc_test_checks.py
; RUN: llc -mtriple=aarch64-unknown-linux-gnu < %s | FileCheck %s

; Fold
;   ptr - (ptr & (alignment-1))
; To
;   ptr & (0 - alignment)
;
; This needs to be a backend-level fold because only by now pointers
; are just registers; in middle-end IR this can only be done via @llvm.ptrmask()
; intrinsic which is not sufficiently widely-spread yet.
;
; https://bugs.llvm.org/show_bug.cgi?id=44448

; The basic positive tests

define i32 @t0_32(i32 %ptr, i32 %alignment) nounwind {
; CHECK-LABEL: t0_32:
; CHECK:       // %bb.0:
; CHECK-NEXT:    neg w8, w1
; CHECK-NEXT:    and w0, w0, w8
; CHECK-NEXT:    ret
  %mask = add i32 %alignment, -1
  %bias = and i32 %ptr, %mask
  %r = sub i32 %ptr, %bias
  ret i32 %r
}
define i64 @t1_64(i64 %ptr, i64 %alignment) nounwind {
; CHECK-LABEL: t1_64:
; CHECK:       // %bb.0:
; CHECK-NEXT:    neg x8, x1
; CHECK-NEXT:    and x0, x0, x8
; CHECK-NEXT:    ret
  %mask = add i64 %alignment, -1
  %bias = and i64 %ptr, %mask
  %r = sub i64 %ptr, %bias
  ret i64 %r
}

define i32 @t2_commutative(i32 %ptr, i32 %alignment) nounwind {
; CHECK-LABEL: t2_commutative:
; CHECK:       // %bb.0:
; CHECK-NEXT:    neg w8, w1
; CHECK-NEXT:    and w0, w0, w8
; CHECK-NEXT:    ret
  %mask = add i32 %alignment, -1
  %bias = and i32 %mask, %ptr ; swapped
  %r = sub i32 %ptr, %bias
  ret i32 %r
}

; Extra use tests

define i32 @t3_extrause0(i32 %ptr, i32 %alignment, i32* %mask_storage) nounwind {
; CHECK-LABEL: t3_extrause0:
; CHECK:       // %bb.0:
; CHECK-NEXT:    neg w9, w1
; CHECK-NEXT:    sub w8, w1, #1 // =1
; CHECK-NEXT:    and w0, w0, w9
; CHECK-NEXT:    str w8, [x2]
; CHECK-NEXT:    ret
  %mask = add i32 %alignment, -1
  store i32 %mask, i32* %mask_storage
  %bias = and i32 %ptr, %mask
  %r = sub i32 %ptr, %bias
  ret i32 %r
}
define i32 @n4_extrause1(i32 %ptr, i32 %alignment, i32* %bias_storage) nounwind {
; CHECK-LABEL: n4_extrause1:
; CHECK:       // %bb.0:
; CHECK-NEXT:    sub w8, w1, #1 // =1
; CHECK-NEXT:    and w8, w0, w8
; CHECK-NEXT:    sub w0, w0, w8
; CHECK-NEXT:    str w8, [x2]
; CHECK-NEXT:    ret
  %mask = add i32 %alignment, -1
  %bias = and i32 %ptr, %mask ; has extra uses, can't fold
  store i32 %bias, i32* %bias_storage
  %r = sub i32 %ptr, %bias
  ret i32 %r
}
define i32 @n5_extrause2(i32 %ptr, i32 %alignment, i32* %mask_storage, i32* %bias_storage) nounwind {
; CHECK-LABEL: n5_extrause2:
; CHECK:       // %bb.0:
; CHECK-NEXT:    sub w8, w1, #1 // =1
; CHECK-NEXT:    str w8, [x2]
; CHECK-NEXT:    and w8, w0, w8
; CHECK-NEXT:    sub w0, w0, w8
; CHECK-NEXT:    str w8, [x3]
; CHECK-NEXT:    ret
  %mask = add i32 %alignment, -1
  store i32 %mask, i32* %mask_storage
  %bias = and i32 %ptr, %mask ; has extra uses, can't fold
  store i32 %bias, i32* %bias_storage
  %r = sub i32 %ptr, %bias
  ret i32 %r
}

; Negative tests

define i32 @n6_different_ptrs(i32 %ptr0, i32 %ptr1, i32 %alignment) nounwind {
; CHECK-LABEL: n6_different_ptrs:
; CHECK:       // %bb.0:
; CHECK-NEXT:    sub w8, w2, #1 // =1
; CHECK-NEXT:    and w8, w1, w8
; CHECK-NEXT:    sub w0, w0, w8
; CHECK-NEXT:    ret
  %mask = add i32 %alignment, -1
  %bias = and i32 %ptr1, %mask ; not %ptr0
  %r = sub i32 %ptr0, %bias ; not %ptr1
  ret i32 %r
}
define i32 @n7_different_ptrs_commutative(i32 %ptr0, i32 %ptr1, i32 %alignment) nounwind {
; CHECK-LABEL: n7_different_ptrs_commutative:
; CHECK:       // %bb.0:
; CHECK-NEXT:    sub w8, w2, #1 // =1
; CHECK-NEXT:    and w8, w8, w1
; CHECK-NEXT:    sub w0, w0, w8
; CHECK-NEXT:    ret
  %mask = add i32 %alignment, -1
  %bias = and i32 %mask, %ptr1 ; swapped, not %ptr0
  %r = sub i32 %ptr0, %bias ; not %ptr1
  ret i32 %r
}

define i32 @n8_not_lowbit_mask(i32 %ptr, i32 %alignment) nounwind {
; CHECK-LABEL: n8_not_lowbit_mask:
; CHECK:       // %bb.0:
; CHECK-NEXT:    add w8, w1, #1 // =1
; CHECK-NEXT:    bic w0, w0, w8
; CHECK-NEXT:    ret
  %mask = add i32 %alignment, 1 ; not -1
  %bias = and i32 %ptr, %mask
  %r = sub i32 %ptr, %bias
  ret i32 %r
}

define i32 @n9_sub_is_not_commutative(i32 %ptr, i32 %alignment) nounwind {
; CHECK-LABEL: n9_sub_is_not_commutative:
; CHECK:       // %bb.0:
; CHECK-NEXT:    sub w8, w1, #1 // =1
; CHECK-NEXT:    and w8, w0, w8
; CHECK-NEXT:    sub w0, w8, w0
; CHECK-NEXT:    ret
  %mask = add i32 %alignment, -1
  %bias = and i32 %ptr, %mask
  %r = sub i32 %bias, %ptr ; wrong order
  ret i32 %r
}
