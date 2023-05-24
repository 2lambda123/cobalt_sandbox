; NOTE: Assertions have been autogenerated by utils/update_test_checks.py
; RUN: opt < %s -instsimplify -S | FileCheck %s

define i64 @test0(i64 %x) {
; CHECK-LABEL: @test0(
; CHECK-NEXT:  start:
; CHECK-NEXT:    [[A:%.*]] = icmp eq i64 [[X:%.*]], 0
; CHECK-NEXT:    br i1 [[A]], label [[EXIT:%.*]], label [[NON_ZERO:%.*]]
; CHECK:       non_zero:
; CHECK-NEXT:    br i1 false, label [[UNREACHABLE:%.*]], label [[EXIT]]
; CHECK:       unreachable:
; CHECK-NEXT:    br label [[EXIT]]
; CHECK:       exit:
; CHECK-NEXT:    [[C:%.*]] = phi i64 [ 0, [[START:%.*]] ], [ 1, [[NON_ZERO]] ], [ 2, [[UNREACHABLE]] ]
; CHECK-NEXT:    ret i64 [[C]]
;
start:
  %a = icmp eq i64 %x, 0
  br i1 %a, label %exit, label %non_zero

non_zero:
  %b = icmp eq i64 %x, 0
  br i1 %b, label %unreachable, label %exit

unreachable:
  br label %exit

exit:
  %c = phi i64 [ 0, %start ], [ 1, %non_zero ], [ 2, %unreachable ]
  ret i64 %c
}

define i64 @test1(i64 %x) {
; CHECK-LABEL: @test1(
; CHECK-NEXT:  start:
; CHECK-NEXT:    [[A:%.*]] = icmp eq i64 [[X:%.*]], 0
; CHECK-NEXT:    br i1 [[A]], label [[EXIT:%.*]], label [[NON_ZERO:%.*]]
; CHECK:       non_zero:
; CHECK-NEXT:    br i1 true, label [[EXIT]], label [[UNREACHABLE:%.*]]
; CHECK:       unreachable:
; CHECK-NEXT:    br label [[EXIT]]
; CHECK:       exit:
; CHECK-NEXT:    [[C:%.*]] = phi i64 [ 0, [[START:%.*]] ], [ [[X]], [[NON_ZERO]] ], [ 0, [[UNREACHABLE]] ]
; CHECK-NEXT:    ret i64 [[C]]
;
start:
  %a = icmp eq i64 %x, 0
  br i1 %a, label %exit, label %non_zero

non_zero:
  %b = icmp ugt i64 %x, 0
  br i1 %b, label %exit, label %unreachable

unreachable:
  br label %exit

exit:
  %c = phi i64 [ 0, %start ], [ %x, %non_zero ], [ 0, %unreachable ]
  ret i64 %c
}

define i1 @test2(i64 %x, i1 %y) {
; CHECK-LABEL: @test2(
; CHECK-NEXT:  start:
; CHECK-NEXT:    [[A:%.*]] = icmp eq i64 [[X:%.*]], 0
; CHECK-NEXT:    br i1 [[A]], label [[EXIT:%.*]], label [[NON_ZERO:%.*]]
; CHECK:       non_zero:
; CHECK-NEXT:    br i1 [[Y:%.*]], label [[ONE:%.*]], label [[TWO:%.*]]
; CHECK:       one:
; CHECK-NEXT:    br label [[MAINBLOCK:%.*]]
; CHECK:       two:
; CHECK-NEXT:    br label [[MAINBLOCK]]
; CHECK:       mainblock:
; CHECK-NEXT:    br label [[EXIT]]
; CHECK:       exit:
; CHECK-NEXT:    [[RES:%.*]] = phi i1 [ false, [[MAINBLOCK]] ], [ true, [[START:%.*]] ]
; CHECK-NEXT:    ret i1 [[RES]]
;
start:
  %a = icmp eq i64 %x, 0
  br i1 %a, label %exit, label %non_zero

non_zero:
  br i1 %y, label %one, label %two

one:
  br label %mainblock

two:
  br label %mainblock

mainblock:
  %p = phi i64 [ %x, %one ], [ 42, %two ]
  %cmp = icmp eq i64 %p, 0
  br label %exit

exit:
  %res = phi i1 [ %cmp, %mainblock ], [ 1, %start ]
  ret i1 %res
}


; The code below exposed a bug similar to the one exposed by D60846, see the commit 6ea477590085.
; In a nutshell, we should not replace %result.0 with 0 here.

define zeroext i8 @update_phi_query_loc_in_recursive_call(i8* nocapture readonly %p){
; CHECK-LABEL: @update_phi_query_loc_in_recursive_call(
; CHECK-NEXT:  entry:
; CHECK-NEXT:    br label [[FOR_COND:%.*]]
; CHECK:       for.cond:
; CHECK-NEXT:    [[RESULT_0:%.*]] = phi i8 [ 0, [[ENTRY:%.*]] ], [ [[CONV2:%.*]], [[FOR_BODY:%.*]] ]
; CHECK-NEXT:    [[SHIFT_0:%.*]] = phi i32 [ 0, [[ENTRY]] ], [ 1, [[FOR_BODY]] ]
; CHECK-NEXT:    [[CMP:%.*]] = icmp eq i32 [[SHIFT_0]], 0
; CHECK-NEXT:    br i1 [[CMP]], label [[FOR_BODY]], label [[FOR_COND_CLEANUP:%.*]]
; CHECK:       for.cond.cleanup:
; CHECK-NEXT:    ret i8 [[RESULT_0]]
; CHECK:       for.body:
; CHECK-NEXT:    [[TMP0:%.*]] = load i8, i8* [[P:%.*]], align 1
; CHECK-NEXT:    [[CONV:%.*]] = zext i8 [[TMP0]] to i32
; CHECK-NEXT:    [[MUL:%.*]] = shl nuw nsw i32 [[SHIFT_0]], 3
; CHECK-NEXT:    [[SHL:%.*]] = shl nuw nsw i32 [[CONV]], [[MUL]]
; CHECK-NEXT:    [[TMP1:%.*]] = trunc i32 [[SHL]] to i8
; CHECK-NEXT:    [[CONV2]] = or i8 [[RESULT_0]], [[TMP1]]
; CHECK-NEXT:    br label [[FOR_COND]]
;
entry:
  br label %for.cond

for.cond:                                         ; preds = %for.body, %entry
  %result.0 = phi i8 [ 0, %entry ], [ %conv2, %for.body ]
  %shift.0 = phi i32 [ 0, %entry ], [ 1, %for.body ]
  %cmp = icmp eq i32 %shift.0, 0
  br i1 %cmp, label %for.body, label %for.cond.cleanup

for.cond.cleanup:                                 ; preds = %for.cond
  ret i8 %result.0

for.body:                                         ; preds = %for.cond
  %0 = load i8, i8* %p, align 1
  %conv = zext i8 %0 to i32
  %mul = shl nuw nsw i32 %shift.0, 3
  %shl = shl nuw nsw i32 %conv, %mul
  %1 = trunc i32 %shl to i8
  %conv2 = or i8 %result.0, %1
  %inc = add nuw nsw i32 %shift.0, 1
  br label %for.cond
}
