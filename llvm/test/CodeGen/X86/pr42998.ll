; NOTE: Assertions have been autogenerated by utils/update_llc_test_checks.py
; RUN: llc < %s -mtriple=x86_64-unknown-unknown -mcpu=skx | FileCheck %s --check-prefixes=CHECK,FAST-INCDEC
; RUN: llc < %s -mtriple=x86_64-unknown-unknown -mcpu=goldmont | FileCheck %s --check-prefixes=CHECK,SLOW-INCDEC

define i64 @imm1_Oz(i32 %x, i32 %y) minsize nounwind {
; CHECK-LABEL: imm1_Oz:
; CHECK:       # %bb.0:
; CHECK-NEXT:    # kill: def $esi killed $esi def $rsi
; CHECK-NEXT:    # kill: def $edi killed $edi def $rdi
; CHECK-NEXT:    leal 1(%rdi), %eax
; CHECK-NEXT:    incl %esi
; CHECK-NEXT:    addq %rsi, %rax
; CHECK-NEXT:    retq
  %x1 = add i32 %x, 1
  %y1 = add i32 %y, 1
  %x1z = zext i32 %x1 to i64
  %y1z = zext i32 %y1 to i64
  %r = add i64 %x1z, %y1z
  ret i64 %r
}

define i64 @imm1_Os(i32 %x, i32 %y) optsize nounwind {
; CHECK-LABEL: imm1_Os:
; CHECK:       # %bb.0:
; CHECK-NEXT:    # kill: def $esi killed $esi def $rsi
; CHECK-NEXT:    # kill: def $edi killed $edi def $rdi
; CHECK-NEXT:    leal 1(%rdi), %eax
; CHECK-NEXT:    incl %esi
; CHECK-NEXT:    addq %rsi, %rax
; CHECK-NEXT:    retq
  %x1 = add i32 %x, 1
  %y1 = add i32 %y, 1
  %x1z = zext i32 %x1 to i64
  %y1z = zext i32 %y1 to i64
  %r = add i64 %x1z, %y1z
  ret i64 %r
}

define i64 @imm1_O2(i32 %x, i32 %y) nounwind {
; FAST-INCDEC-LABEL: imm1_O2:
; FAST-INCDEC:       # %bb.0:
; FAST-INCDEC-NEXT:    # kill: def $esi killed $esi def $rsi
; FAST-INCDEC-NEXT:    # kill: def $edi killed $edi def $rdi
; FAST-INCDEC-NEXT:    leal 1(%rdi), %eax
; FAST-INCDEC-NEXT:    incl %esi
; FAST-INCDEC-NEXT:    addq %rsi, %rax
; FAST-INCDEC-NEXT:    retq
;
; SLOW-INCDEC-LABEL: imm1_O2:
; SLOW-INCDEC:       # %bb.0:
; SLOW-INCDEC-NEXT:    # kill: def $esi killed $esi def $rsi
; SLOW-INCDEC-NEXT:    # kill: def $edi killed $edi def $rdi
; SLOW-INCDEC-NEXT:    leal 1(%rdi), %eax
; SLOW-INCDEC-NEXT:    addl $1, %esi
; SLOW-INCDEC-NEXT:    addq %rsi, %rax
; SLOW-INCDEC-NEXT:    retq
  %x1 = add i32 %x, 1
  %y1 = add i32 %y, 1
  %x1z = zext i32 %x1 to i64
  %y1z = zext i32 %y1 to i64
  %r = add i64 %x1z, %y1z
  ret i64 %r
}
