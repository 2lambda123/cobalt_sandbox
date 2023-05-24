; NOTE: Assertions have been autogenerated by utils/update_llc_test_checks.py
; RUN: llc < %s -mtriple=x86_64-- | FileCheck %s

; Do eliminate the zero-extension instruction and rely on
; x86-64's implicit zero-extension!

define i64 @foo(i32* %p) nounwind {
; CHECK-LABEL: foo:
; CHECK:       # %bb.0:
; CHECK-NEXT:    movl (%rdi), %eax
; CHECK-NEXT:    incl %eax
; CHECK-NEXT:    retq
  %t = load i32, i32* %p
  %n = add i32 %t, 1
  %z = zext i32 %n to i64
  ret i64 %z
}
