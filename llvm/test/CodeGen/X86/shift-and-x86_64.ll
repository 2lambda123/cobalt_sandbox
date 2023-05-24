; NOTE: Assertions have been autogenerated by utils/update_llc_test_checks.py
; RUN: llc < %s -mtriple=x86_64-unknown-unknown | FileCheck %s

define { i64, i64 } @PR36721_u8(i64, i64, i8 zeroext) nounwind {
; CHECK-LABEL: PR36721_u8:
; CHECK:       # %bb.0:
; CHECK-NEXT:    movl %edx, %ecx
; CHECK-NEXT:    movq %rsi, %rdx
; CHECK-NEXT:    movq %rdi, %rax
; CHECK-NEXT:    shldq %cl, %rdi, %rdx
; CHECK-NEXT:    # kill: def $cl killed $cl killed $ecx
; CHECK-NEXT:    shlq %cl, %rax
; CHECK-NEXT:    retq
  %4 = zext i64 %1 to i128
  %5 = shl nuw i128 %4, 64
  %6 = zext i64 %0 to i128
  %7 = or i128 %5, %6
  %8 = and i8 %2, 63
  %9 = zext i8 %8 to i128
  %10 = shl i128 %7, %9
  %11 = trunc i128 %10 to i64
  %12 = lshr i128 %10, 64
  %13 = trunc i128 %12 to i64
  %14 = insertvalue { i64, i64 } undef, i64 %11, 0
  %15 = insertvalue { i64, i64 } %14, i64 %13, 1
  ret { i64, i64 } %15
}

define { i64, i64 } @PR36721_u32(i64, i64, i32) nounwind {
; CHECK-LABEL: PR36721_u32:
; CHECK:       # %bb.0:
; CHECK-NEXT:    movl %edx, %ecx
; CHECK-NEXT:    movq %rsi, %rdx
; CHECK-NEXT:    movq %rdi, %rax
; CHECK-NEXT:    shldq %cl, %rdi, %rdx
; CHECK-NEXT:    # kill: def $cl killed $cl killed $ecx
; CHECK-NEXT:    shlq %cl, %rax
; CHECK-NEXT:    retq
  %4 = zext i64 %1 to i128
  %5 = shl nuw i128 %4, 64
  %6 = zext i64 %0 to i128
  %7 = or i128 %5, %6
  %8 = and i32 %2, 63
  %9 = zext i32 %8 to i128
  %10 = shl i128 %7, %9
  %11 = trunc i128 %10 to i64
  %12 = lshr i128 %10, 64
  %13 = trunc i128 %12 to i64
  %14 = insertvalue { i64, i64 } undef, i64 %11, 0
  %15 = insertvalue { i64, i64 } %14, i64 %13, 1
  ret { i64, i64 } %15
}
