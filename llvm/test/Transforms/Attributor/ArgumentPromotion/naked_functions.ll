; NOTE: Assertions have been autogenerated by utils/update_test_checks.py UTC_ARGS: --function-signature --scrub-attributes
; RUN: opt -S -passes=attributor -aa-pipeline='basic-aa' -attributor-disable=false -attributor-max-iterations-verify -attributor-max-iterations=1 < %s | FileCheck %s

; Don't promote paramaters of/arguments to naked functions

@g = common global i32 0, align 4

define i32 @bar() {
; CHECK-LABEL: define {{[^@]+}}@bar()
; CHECK-NEXT:  entry:
; CHECK-NEXT:    [[CALL:%.*]] = call i32 @foo(i32* nonnull align 4 dereferenceable(4) @g)
; CHECK-NEXT:    ret i32 [[CALL]]
;
entry:
  %call = call i32 @foo(i32* @g)
  ret i32 %call
}

define internal i32 @foo(i32*) #0 {
; CHECK-LABEL: define {{[^@]+}}@foo
; CHECK-SAME: (i32* [[TMP0:%.*]])
; CHECK-NEXT:  entry:
; CHECK-NEXT:    [[RETVAL:%.*]] = alloca i32, align 4
; CHECK-NEXT:    call void asm sideeffect "ldr r0, [r0] \0Abx lr \0A", ""()
; CHECK-NEXT:    unreachable
;
entry:
  %retval = alloca i32, align 4
  call void asm sideeffect "ldr r0, [r0] \0Abx lr        \0A", ""()
  unreachable
}


attributes #0 = { naked }
