
; RUN: llc < %s -mtriple=aarch64-pc-windows-msvc | FileCheck %s
; Control Flow Guard is currently only available on Windows

; Test that Control Flow Guard checks are not added in modules with the
; cfguard=1 flag (emit tables but no checks).


declare void @target_func()

define void @func_in_module_without_cfguard() #0 {
entry:
  %func_ptr = alloca void ()*, align 8
  store void ()* @target_func, void ()** %func_ptr, align 8
  %0 = load void ()*, void ()** %func_ptr, align 8

  call void %0()
  ret void

  ; CHECK-NOT: __guard_check_icall_fptr
  ; CHECK-NOT: __guard_dispatch_icall_fptr
}

!llvm.module.flags = !{!0}
!0 = !{i32 2, !"cfguard", i32 1}
