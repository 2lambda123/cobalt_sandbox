; NOTE: Assertions have been autogenerated by utils/update_llc_test_checks.py
; RUN: llc < %s -mtriple=i686-- | FileCheck %s

define void @foo(x86_fp80 %a, x86_fp80 %b, float* %fp) {
; CHECK-LABEL: foo:
; CHECK:       # %bb.0:
; CHECK-NEXT:    movl {{[0-9]+}}(%esp), %eax
; CHECK-NEXT:    fldt {{[0-9]+}}(%esp)
; CHECK-NEXT:    fldt {{[0-9]+}}(%esp)
; CHECK-NEXT:    faddp %st, %st(1)
; CHECK-NEXT:    fstps (%eax)
; CHECK-NEXT:    retl
	%c = fadd x86_fp80 %a, %b
	%d = fptrunc x86_fp80 %c to float
	store float %d, float* %fp
	ret void
}
