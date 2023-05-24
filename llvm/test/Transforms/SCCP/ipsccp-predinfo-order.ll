; NOTE: Assertions have been autogenerated by utils/update_test_checks.py
; RUN: opt -ipsccp -S %s | FileCheck %s

declare i32 @hoge()

define dso_local i32 @ham(i8* %arg, i8* %arg1) {
; CHECK-LABEL: @ham(
; CHECK-NEXT:  bb:
; CHECK-NEXT:    [[TMP:%.*]] = alloca i32
; CHECK-NEXT:    [[TMP2:%.*]] = alloca i32, align 4
; CHECK-NEXT:    br label [[BB19:%.*]]
; CHECK:       bb4:
; CHECK-NEXT:    br label [[BB6:%.*]]
; CHECK:       bb6:
; CHECK-NEXT:    [[TMP7:%.*]] = call i32 @hoge()
; CHECK-NEXT:    store i32 [[TMP7]], i32* [[TMP]]
; CHECK-NEXT:    [[TMP8:%.*]] = load i32, i32* [[TMP]]
; CHECK-NEXT:    [[TMP9:%.*]] = icmp eq i32 [[TMP8]], 912730082
; CHECK-NEXT:    [[TMP10:%.*]] = load i32, i32* [[TMP]]
; CHECK-NEXT:    br i1 [[TMP9]], label [[BB11:%.*]], label [[BB16:%.*]]
; CHECK:       bb11:
; CHECK-NEXT:    unreachable
; CHECK:       bb13:
; CHECK-NEXT:    br label [[BB14:%.*]]
; CHECK:       bb14:
; CHECK-NEXT:    [[TMP15:%.*]] = load i32, i32* [[TMP]]
; CHECK-NEXT:    br label [[BB16]]
; CHECK:       bb16:
; CHECK-NEXT:    [[TMP17:%.*]] = phi i32 [ [[TMP10]], [[BB6]] ], [ 0, [[BB14]] ]
; CHECK-NEXT:    br label [[BB19]]
; CHECK:       bb18:
; CHECK-NEXT:    unreachable
; CHECK:       bb19:
; CHECK-NEXT:    br label [[BB20:%.*]]
; CHECK:       bb20:
; CHECK-NEXT:    indirectbr i8* null, [label [[BB4:%.*]], label [[BB13:%.*]], label %bb18]
;
bb:
  %tmp = alloca i32
  %tmp2 = alloca i32, align 4
  br label %bb19

bb4:                                              ; preds = %bb20
  br label %bb6

bb6:                                              ; preds = %bb4
  %tmp7 = call i32 @hoge()
  store i32 %tmp7, i32* %tmp
  %tmp8 = load i32, i32* %tmp
  %tmp9 = icmp eq i32 %tmp8, 912730082
  %tmp10 = load i32, i32* %tmp
  br i1 %tmp9, label %bb11, label %bb16

bb11:                                             ; preds = %bb6
  unreachable

bb13:                                             ; preds = %bb20
  br label %bb14

bb14:                                             ; preds = %bb13
  %tmp15 = load i32, i32* %tmp
  br label %bb16

bb16:                                             ; preds = %bb14, %bb6
  %tmp17 = phi i32 [ %tmp10, %bb6 ], [ 0, %bb14 ]
  br label %bb19

bb18:                                             ; preds = %bb20
  unreachable

bb19:                                             ; preds = %bb16, %bb
  br label %bb20

bb20:                                             ; preds = %bb19
  indirectbr i8* null, [label %bb4, label %bb13, label %bb18]
}
