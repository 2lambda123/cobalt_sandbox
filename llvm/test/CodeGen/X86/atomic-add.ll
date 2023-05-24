; NOTE: Assertions have been autogenerated by utils/update_llc_test_checks.py
; RUN: llc < %s -mtriple=x86_64-- -verify-machineinstrs | FileCheck %s
; RUN: llc < %s -mtriple=x86_64-- -mattr=slow-incdec -verify-machineinstrs | FileCheck %s --check-prefix SLOW_INC

; rdar://7103704

define void @sub1(i32* nocapture %p, i32 %v) nounwind ssp {
; CHECK-LABEL: sub1:
; CHECK:       # %bb.0: # %entry
; CHECK-NEXT:    lock subl %esi, (%rdi)
; CHECK-NEXT:    retq
;
; SLOW_INC-LABEL: sub1:
; SLOW_INC:       # %bb.0: # %entry
; SLOW_INC-NEXT:    lock subl %esi, (%rdi)
; SLOW_INC-NEXT:    retq
entry:
  %0 = atomicrmw sub i32* %p, i32 %v monotonic
  ret void
}

define void @inc4(i64* nocapture %p) nounwind ssp {
; CHECK-LABEL: inc4:
; CHECK:       # %bb.0: # %entry
; CHECK-NEXT:    lock incq (%rdi)
; CHECK-NEXT:    retq
;
; SLOW_INC-LABEL: inc4:
; SLOW_INC:       # %bb.0: # %entry
; SLOW_INC-NEXT:    lock addq $1, (%rdi)
; SLOW_INC-NEXT:    retq
entry:
  %0 = atomicrmw add i64* %p, i64 1 monotonic
  ret void
}

define void @add8(i64* nocapture %p) nounwind ssp {
; CHECK-LABEL: add8:
; CHECK:       # %bb.0: # %entry
; CHECK-NEXT:    lock addq $2, (%rdi)
; CHECK-NEXT:    retq
;
; SLOW_INC-LABEL: add8:
; SLOW_INC:       # %bb.0: # %entry
; SLOW_INC-NEXT:    lock addq $2, (%rdi)
; SLOW_INC-NEXT:    retq
entry:
  %0 = atomicrmw add i64* %p, i64 2 monotonic
  ret void
}

define void @add4(i64* nocapture %p, i32 %v) nounwind ssp {
; CHECK-LABEL: add4:
; CHECK:       # %bb.0: # %entry
; CHECK-NEXT:    movslq %esi, %rax
; CHECK-NEXT:    lock addq %rax, (%rdi)
; CHECK-NEXT:    retq
;
; SLOW_INC-LABEL: add4:
; SLOW_INC:       # %bb.0: # %entry
; SLOW_INC-NEXT:    movslq %esi, %rax
; SLOW_INC-NEXT:    lock addq %rax, (%rdi)
; SLOW_INC-NEXT:    retq
entry:
  %0 = sext i32 %v to i64		; <i64> [#uses=1]
  %1 = atomicrmw add i64* %p, i64 %0 monotonic
  ret void
}

define void @inc3(i8* nocapture %p) nounwind ssp {
; CHECK-LABEL: inc3:
; CHECK:       # %bb.0: # %entry
; CHECK-NEXT:    lock incb (%rdi)
; CHECK-NEXT:    retq
;
; SLOW_INC-LABEL: inc3:
; SLOW_INC:       # %bb.0: # %entry
; SLOW_INC-NEXT:    lock addb $1, (%rdi)
; SLOW_INC-NEXT:    retq
entry:
  %0 = atomicrmw add i8* %p, i8 1 monotonic
  ret void
}

define void @add7(i8* nocapture %p) nounwind ssp {
; CHECK-LABEL: add7:
; CHECK:       # %bb.0: # %entry
; CHECK-NEXT:    lock addb $2, (%rdi)
; CHECK-NEXT:    retq
;
; SLOW_INC-LABEL: add7:
; SLOW_INC:       # %bb.0: # %entry
; SLOW_INC-NEXT:    lock addb $2, (%rdi)
; SLOW_INC-NEXT:    retq
entry:
  %0 = atomicrmw add i8* %p, i8 2 monotonic
  ret void
}

define void @add3(i8* nocapture %p, i32 %v) nounwind ssp {
; CHECK-LABEL: add3:
; CHECK:       # %bb.0: # %entry
; CHECK-NEXT:    lock addb %sil, (%rdi)
; CHECK-NEXT:    retq
;
; SLOW_INC-LABEL: add3:
; SLOW_INC:       # %bb.0: # %entry
; SLOW_INC-NEXT:    lock addb %sil, (%rdi)
; SLOW_INC-NEXT:    retq
entry:
  %0 = trunc i32 %v to i8		; <i8> [#uses=1]
  %1 = atomicrmw add i8* %p, i8 %0 monotonic
  ret void
}

define void @inc2(i16* nocapture %p) nounwind ssp {
; CHECK-LABEL: inc2:
; CHECK:       # %bb.0: # %entry
; CHECK-NEXT:    lock incw (%rdi)
; CHECK-NEXT:    retq
;
; SLOW_INC-LABEL: inc2:
; SLOW_INC:       # %bb.0: # %entry
; SLOW_INC-NEXT:    lock addw $1, (%rdi)
; SLOW_INC-NEXT:    retq
entry:
  %0 = atomicrmw add i16* %p, i16 1 monotonic
  ret void
}

define void @add6(i16* nocapture %p) nounwind ssp {
; CHECK-LABEL: add6:
; CHECK:       # %bb.0: # %entry
; CHECK-NEXT:    lock addw $2, (%rdi)
; CHECK-NEXT:    retq
;
; SLOW_INC-LABEL: add6:
; SLOW_INC:       # %bb.0: # %entry
; SLOW_INC-NEXT:    lock addw $2, (%rdi)
; SLOW_INC-NEXT:    retq
entry:
  %0 = atomicrmw add i16* %p, i16 2 monotonic
  ret void
}

define void @add2(i16* nocapture %p, i32 %v) nounwind ssp {
; CHECK-LABEL: add2:
; CHECK:       # %bb.0: # %entry
; CHECK-NEXT:    lock addw %si, (%rdi)
; CHECK-NEXT:    retq
;
; SLOW_INC-LABEL: add2:
; SLOW_INC:       # %bb.0: # %entry
; SLOW_INC-NEXT:    lock addw %si, (%rdi)
; SLOW_INC-NEXT:    retq
entry:
	%0 = trunc i32 %v to i16		; <i16> [#uses=1]
  %1 = atomicrmw add i16* %p, i16 %0 monotonic
  ret void
}

define void @inc1(i32* nocapture %p) nounwind ssp {
; CHECK-LABEL: inc1:
; CHECK:       # %bb.0: # %entry
; CHECK-NEXT:    lock incl (%rdi)
; CHECK-NEXT:    retq
;
; SLOW_INC-LABEL: inc1:
; SLOW_INC:       # %bb.0: # %entry
; SLOW_INC-NEXT:    lock addl $1, (%rdi)
; SLOW_INC-NEXT:    retq
entry:
  %0 = atomicrmw add i32* %p, i32 1 monotonic
  ret void
}

define void @add5(i32* nocapture %p) nounwind ssp {
; CHECK-LABEL: add5:
; CHECK:       # %bb.0: # %entry
; CHECK-NEXT:    lock addl $2, (%rdi)
; CHECK-NEXT:    retq
;
; SLOW_INC-LABEL: add5:
; SLOW_INC:       # %bb.0: # %entry
; SLOW_INC-NEXT:    lock addl $2, (%rdi)
; SLOW_INC-NEXT:    retq
entry:
  %0 = atomicrmw add i32* %p, i32 2 monotonic
  ret void
}

define void @add1(i32* nocapture %p, i32 %v) nounwind ssp {
; CHECK-LABEL: add1:
; CHECK:       # %bb.0: # %entry
; CHECK-NEXT:    lock addl %esi, (%rdi)
; CHECK-NEXT:    retq
;
; SLOW_INC-LABEL: add1:
; SLOW_INC:       # %bb.0: # %entry
; SLOW_INC-NEXT:    lock addl %esi, (%rdi)
; SLOW_INC-NEXT:    retq
entry:
  %0 = atomicrmw add i32* %p, i32 %v monotonic
  ret void
}

define void @dec4(i64* nocapture %p) nounwind ssp {
; CHECK-LABEL: dec4:
; CHECK:       # %bb.0: # %entry
; CHECK-NEXT:    lock decq (%rdi)
; CHECK-NEXT:    retq
;
; SLOW_INC-LABEL: dec4:
; SLOW_INC:       # %bb.0: # %entry
; SLOW_INC-NEXT:    lock subq $1, (%rdi)
; SLOW_INC-NEXT:    retq
entry:
  %0 = atomicrmw sub i64* %p, i64 1 monotonic
  ret void
}

define void @sub8(i64* nocapture %p) nounwind ssp {
; CHECK-LABEL: sub8:
; CHECK:       # %bb.0: # %entry
; CHECK-NEXT:    lock subq $2, (%rdi)
; CHECK-NEXT:    retq
;
; SLOW_INC-LABEL: sub8:
; SLOW_INC:       # %bb.0: # %entry
; SLOW_INC-NEXT:    lock subq $2, (%rdi)
; SLOW_INC-NEXT:    retq
entry:
  %0 = atomicrmw sub i64* %p, i64 2 monotonic
  ret void
}

define void @sub4(i64* nocapture %p, i32 %v) nounwind ssp {
; CHECK-LABEL: sub4:
; CHECK:       # %bb.0: # %entry
; CHECK-NEXT:    movslq %esi, %rax
; CHECK-NEXT:    lock subq %rax, (%rdi)
; CHECK-NEXT:    retq
;
; SLOW_INC-LABEL: sub4:
; SLOW_INC:       # %bb.0: # %entry
; SLOW_INC-NEXT:    movslq %esi, %rax
; SLOW_INC-NEXT:    lock subq %rax, (%rdi)
; SLOW_INC-NEXT:    retq
entry:
	%0 = sext i32 %v to i64		; <i64> [#uses=1]
  %1 = atomicrmw sub i64* %p, i64 %0 monotonic
  ret void
}

define void @dec3(i8* nocapture %p) nounwind ssp {
; CHECK-LABEL: dec3:
; CHECK:       # %bb.0: # %entry
; CHECK-NEXT:    lock decb (%rdi)
; CHECK-NEXT:    retq
;
; SLOW_INC-LABEL: dec3:
; SLOW_INC:       # %bb.0: # %entry
; SLOW_INC-NEXT:    lock subb $1, (%rdi)
; SLOW_INC-NEXT:    retq
entry:
  %0 = atomicrmw sub i8* %p, i8 1 monotonic
  ret void
}

define void @sub7(i8* nocapture %p) nounwind ssp {
; CHECK-LABEL: sub7:
; CHECK:       # %bb.0: # %entry
; CHECK-NEXT:    lock subb $2, (%rdi)
; CHECK-NEXT:    retq
;
; SLOW_INC-LABEL: sub7:
; SLOW_INC:       # %bb.0: # %entry
; SLOW_INC-NEXT:    lock subb $2, (%rdi)
; SLOW_INC-NEXT:    retq
entry:
  %0 = atomicrmw sub i8* %p, i8 2 monotonic
  ret void
}

define void @sub3(i8* nocapture %p, i32 %v) nounwind ssp {
; CHECK-LABEL: sub3:
; CHECK:       # %bb.0: # %entry
; CHECK-NEXT:    lock subb %sil, (%rdi)
; CHECK-NEXT:    retq
;
; SLOW_INC-LABEL: sub3:
; SLOW_INC:       # %bb.0: # %entry
; SLOW_INC-NEXT:    lock subb %sil, (%rdi)
; SLOW_INC-NEXT:    retq
entry:
	%0 = trunc i32 %v to i8		; <i8> [#uses=1]
  %1 = atomicrmw sub i8* %p, i8 %0 monotonic
  ret void
}

define void @dec2(i16* nocapture %p) nounwind ssp {
; CHECK-LABEL: dec2:
; CHECK:       # %bb.0: # %entry
; CHECK-NEXT:    lock decw (%rdi)
; CHECK-NEXT:    retq
;
; SLOW_INC-LABEL: dec2:
; SLOW_INC:       # %bb.0: # %entry
; SLOW_INC-NEXT:    lock subw $1, (%rdi)
; SLOW_INC-NEXT:    retq
entry:
  %0 = atomicrmw sub i16* %p, i16 1 monotonic
  ret void
}

define void @sub6(i16* nocapture %p) nounwind ssp {
; CHECK-LABEL: sub6:
; CHECK:       # %bb.0: # %entry
; CHECK-NEXT:    lock subw $2, (%rdi)
; CHECK-NEXT:    retq
;
; SLOW_INC-LABEL: sub6:
; SLOW_INC:       # %bb.0: # %entry
; SLOW_INC-NEXT:    lock subw $2, (%rdi)
; SLOW_INC-NEXT:    retq
entry:
  %0 = atomicrmw sub i16* %p, i16 2 monotonic
  ret void
}

define void @sub2(i16* nocapture %p, i32 %v) nounwind ssp {
; CHECK-LABEL: sub2:
; CHECK:       # %bb.0: # %entry
; CHECK-NEXT:    lock subw %si, (%rdi)
; CHECK-NEXT:    retq
;
; SLOW_INC-LABEL: sub2:
; SLOW_INC:       # %bb.0: # %entry
; SLOW_INC-NEXT:    lock subw %si, (%rdi)
; SLOW_INC-NEXT:    retq
entry:
	%0 = trunc i32 %v to i16		; <i16> [#uses=1]
  %1 = atomicrmw sub i16* %p, i16 %0 monotonic
  ret void
}

define void @dec1(i32* nocapture %p) nounwind ssp {
; CHECK-LABEL: dec1:
; CHECK:       # %bb.0: # %entry
; CHECK-NEXT:    lock decl (%rdi)
; CHECK-NEXT:    retq
;
; SLOW_INC-LABEL: dec1:
; SLOW_INC:       # %bb.0: # %entry
; SLOW_INC-NEXT:    lock subl $1, (%rdi)
; SLOW_INC-NEXT:    retq
entry:
  %0 = atomicrmw sub i32* %p, i32 1 monotonic
  ret void
}

define void @sub5(i32* nocapture %p) nounwind ssp {
; CHECK-LABEL: sub5:
; CHECK:       # %bb.0: # %entry
; CHECK-NEXT:    lock subl $2, (%rdi)
; CHECK-NEXT:    retq
;
; SLOW_INC-LABEL: sub5:
; SLOW_INC:       # %bb.0: # %entry
; SLOW_INC-NEXT:    lock subl $2, (%rdi)
; SLOW_INC-NEXT:    retq
entry:
  %0 = atomicrmw sub i32* %p, i32 2 monotonic
  ret void
}
