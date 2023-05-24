; NOTE: Assertions have been autogenerated by utils/update_llc_test_checks.py
; RUN: llc < %s -mtriple=i686-- | FileCheck %s

define i32 @test1(i64 %a) nounwind {
; CHECK-LABEL: test1:
; CHECK:       # %bb.0:
; CHECK-NEXT:    movl {{[0-9]+}}(%esp), %eax
; CHECK-NEXT:    shrl %eax
; CHECK-NEXT:    cwtl
; CHECK-NEXT:    retl
        %tmp29 = lshr i64 %a, 24                ; <i64> [#uses=1]
        %tmp23 = trunc i64 %tmp29 to i32                ; <i32> [#uses=1]
        %tmp410 = lshr i32 %tmp23, 9            ; <i32> [#uses=1]
        %tmp45 = trunc i32 %tmp410 to i16               ; <i16> [#uses=1]
        %tmp456 = sext i16 %tmp45 to i32                ; <i32> [#uses=1]
        ret i32 %tmp456
}

