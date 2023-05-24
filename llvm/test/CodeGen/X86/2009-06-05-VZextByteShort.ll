; NOTE: Assertions have been autogenerated by utils/update_llc_test_checks.py
; RUN: llc < %s -mtriple=i686-- -mcpu=core2 | FileCheck %s

define <4 x i16> @a(i32* %x1) nounwind {
; CHECK-LABEL: a:
; CHECK:       # %bb.0:
; CHECK-NEXT:    movl {{[0-9]+}}(%esp), %eax
; CHECK-NEXT:    movl (%eax), %eax
; CHECK-NEXT:    shrl %eax
; CHECK-NEXT:    movzwl %ax, %eax
; CHECK-NEXT:    movd %eax, %xmm0
; CHECK-NEXT:    retl

  %x2 = load i32, i32* %x1
  %x3 = lshr i32 %x2, 1
  %x = trunc i32 %x3 to i16
  %r = insertelement <4 x i16> zeroinitializer, i16 %x, i32 0
  ret <4 x i16> %r
}

define <8 x i16> @b(i32* %x1) nounwind {
; CHECK-LABEL: b:
; CHECK:       # %bb.0:
; CHECK-NEXT:    movl {{[0-9]+}}(%esp), %eax
; CHECK-NEXT:    movl (%eax), %eax
; CHECK-NEXT:    shrl %eax
; CHECK-NEXT:    movzwl %ax, %eax
; CHECK-NEXT:    movd %eax, %xmm0
; CHECK-NEXT:    retl

  %x2 = load i32, i32* %x1
  %x3 = lshr i32 %x2, 1
  %x = trunc i32 %x3 to i16
  %r = insertelement <8 x i16> zeroinitializer, i16 %x, i32 0
  ret <8 x i16> %r
}

define <8 x i8> @c(i32* %x1) nounwind {
; CHECK-LABEL: c:
; CHECK:       # %bb.0:
; CHECK-NEXT:    movl {{[0-9]+}}(%esp), %eax
; CHECK-NEXT:    movl (%eax), %eax
; CHECK-NEXT:    shrl %eax
; CHECK-NEXT:    movzbl %al, %eax
; CHECK-NEXT:    movd %eax, %xmm0
; CHECK-NEXT:    retl

  %x2 = load i32, i32* %x1
  %x3 = lshr i32 %x2, 1
  %x = trunc i32 %x3 to i8
  %r = insertelement <8 x i8> zeroinitializer, i8 %x, i32 0
  ret <8 x i8> %r
}

define <16 x i8> @d(i32* %x1) nounwind {
; CHECK-LABEL: d:
; CHECK:       # %bb.0:
; CHECK-NEXT:    movl {{[0-9]+}}(%esp), %eax
; CHECK-NEXT:    movl (%eax), %eax
; CHECK-NEXT:    shrl %eax
; CHECK-NEXT:    movzbl %al, %eax
; CHECK-NEXT:    movd %eax, %xmm0
; CHECK-NEXT:    retl

  %x2 = load i32, i32* %x1
  %x3 = lshr i32 %x2, 1
  %x = trunc i32 %x3 to i8
  %r = insertelement <16 x i8> zeroinitializer, i8 %x, i32 0
  ret <16 x i8> %r
}

