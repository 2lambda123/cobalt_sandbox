; NOTE: Assertions have been autogenerated by utils/update_llc_test_checks.py
; RUN: llc < %s -mtriple=x86_64-- | FileCheck %s

; Utilize implicit zero-extension on x86-64 to eliminate explicit
; zero-extensions. Shrink 64-bit adds to 32-bit when the high
; 32-bits will be zeroed.

define void @bar(i64 %x, i64 %y, i64* %z) nounwind readnone {
; CHECK-LABEL: bar:
; CHECK:       # %bb.0: # %entry
; CHECK-NEXT:    addl %esi, %edi
; CHECK-NEXT:    movq %rdi, (%rdx)
; CHECK-NEXT:    retq
entry:
	%t0 = add i64 %x, %y
	%t1 = and i64 %t0, 4294967295
        store i64 %t1, i64* %z
	ret void
}
define void @easy(i32 %x, i32 %y, i64* %z) nounwind readnone {
; CHECK-LABEL: easy:
; CHECK:       # %bb.0: # %entry
; CHECK-NEXT:    # kill: def $edi killed $edi def $rdi
; CHECK-NEXT:    addl %esi, %edi
; CHECK-NEXT:    movq %rdi, (%rdx)
; CHECK-NEXT:    retq
entry:
	%t0 = add i32 %x, %y
        %tn = zext i32 %t0 to i64
	%t1 = and i64 %tn, 4294967295
        store i64 %t1, i64* %z
	ret void
}
define void @cola(i64 *%x, i64 %y, i64* %z, i64 %u) nounwind readnone {
; CHECK-LABEL: cola:
; CHECK:       # %bb.0: # %entry
; CHECK-NEXT:    addl (%rdi), %esi
; CHECK-NEXT:    xorq %rcx, %rsi
; CHECK-NEXT:    movq %rsi, (%rdx)
; CHECK-NEXT:    retq
entry:
        %p = load i64, i64* %x
	%t0 = add i64 %p, %y
	%t1 = and i64 %t0, 4294967295
        %t2 = xor i64 %t1, %u
        store i64 %t2, i64* %z
	ret void
}
define void @yaks(i64 *%x, i64 %y, i64* %z, i64 %u) nounwind readnone {
; CHECK-LABEL: yaks:
; CHECK:       # %bb.0: # %entry
; CHECK-NEXT:    addl (%rdi), %esi
; CHECK-NEXT:    xorl %esi, %ecx
; CHECK-NEXT:    movq %rcx, (%rdx)
; CHECK-NEXT:    retq
entry:
        %p = load i64, i64* %x
	%t0 = add i64 %p, %y
        %t1 = xor i64 %t0, %u
	%t2 = and i64 %t1, 4294967295
        store i64 %t2, i64* %z
	ret void
}
define void @foo(i64 *%x, i64 *%y, i64* %z) nounwind readnone {
; CHECK-LABEL: foo:
; CHECK:       # %bb.0: # %entry
; CHECK-NEXT:    movl (%rdi), %eax
; CHECK-NEXT:    addl (%rsi), %eax
; CHECK-NEXT:    movq %rax, (%rdx)
; CHECK-NEXT:    retq
entry:
        %a = load i64, i64* %x
        %b = load i64, i64* %y
	%t0 = add i64 %a, %b
	%t1 = and i64 %t0, 4294967295
        store i64 %t1, i64* %z
	ret void
}
define void @avo(i64 %x, i64* %z, i64 %u) nounwind readnone {
; CHECK-LABEL: avo:
; CHECK:       # %bb.0: # %entry
; CHECK-NEXT:    addl $734847, %edi # imm = 0xB367F
; CHECK-NEXT:    xorq %rdx, %rdi
; CHECK-NEXT:    movq %rdi, (%rsi)
; CHECK-NEXT:    retq
entry:
	%t0 = add i64 %x, 734847
	%t1 = and i64 %t0, 4294967295
        %t2 = xor i64 %t1, %u
        store i64 %t2, i64* %z
	ret void
}
define void @phe(i64 %x, i64* %z, i64 %u) nounwind readnone {
; CHECK-LABEL: phe:
; CHECK:       # %bb.0: # %entry
; CHECK-NEXT:    addl $734847, %edi # imm = 0xB367F
; CHECK-NEXT:    xorl %edi, %edx
; CHECK-NEXT:    movq %rdx, (%rsi)
; CHECK-NEXT:    retq
entry:
	%t0 = add i64 %x, 734847
        %t1 = xor i64 %t0, %u
	%t2 = and i64 %t1, 4294967295
        store i64 %t2, i64* %z
	ret void
}
define void @oze(i64 %y, i64* %z) nounwind readnone {
; CHECK-LABEL: oze:
; CHECK:       # %bb.0: # %entry
; CHECK-NEXT:    incl %edi
; CHECK-NEXT:    movq %rdi, (%rsi)
; CHECK-NEXT:    retq
entry:
	%t0 = add i64 %y, 1
	%t1 = and i64 %t0, 4294967295
        store i64 %t1, i64* %z
	ret void
}

define void @sbar(i64 %x, i64 %y, i64* %z) nounwind readnone {
; CHECK-LABEL: sbar:
; CHECK:       # %bb.0: # %entry
; CHECK-NEXT:    subl %esi, %edi
; CHECK-NEXT:    movq %rdi, (%rdx)
; CHECK-NEXT:    retq
entry:
	%t0 = sub i64 %x, %y
	%t1 = and i64 %t0, 4294967295
        store i64 %t1, i64* %z
	ret void
}
define void @seasy(i32 %x, i32 %y, i64* %z) nounwind readnone {
; CHECK-LABEL: seasy:
; CHECK:       # %bb.0: # %entry
; CHECK-NEXT:    # kill: def $edi killed $edi def $rdi
; CHECK-NEXT:    subl %esi, %edi
; CHECK-NEXT:    movq %rdi, (%rdx)
; CHECK-NEXT:    retq
entry:
	%t0 = sub i32 %x, %y
        %tn = zext i32 %t0 to i64
	%t1 = and i64 %tn, 4294967295
        store i64 %t1, i64* %z
	ret void
}
define void @scola(i64 *%x, i64 %y, i64* %z, i64 %u) nounwind readnone {
; CHECK-LABEL: scola:
; CHECK:       # %bb.0: # %entry
; CHECK-NEXT:    movl (%rdi), %eax
; CHECK-NEXT:    subl %esi, %eax
; CHECK-NEXT:    xorq %rcx, %rax
; CHECK-NEXT:    movq %rax, (%rdx)
; CHECK-NEXT:    retq
entry:
        %p = load i64, i64* %x
	%t0 = sub i64 %p, %y
	%t1 = and i64 %t0, 4294967295
        %t2 = xor i64 %t1, %u
        store i64 %t2, i64* %z
	ret void
}
define void @syaks(i64 *%x, i64 %y, i64* %z, i64 %u) nounwind readnone {
; CHECK-LABEL: syaks:
; CHECK:       # %bb.0: # %entry
; CHECK-NEXT:    movl (%rdi), %eax
; CHECK-NEXT:    subl %esi, %eax
; CHECK-NEXT:    xorl %eax, %ecx
; CHECK-NEXT:    movq %rcx, (%rdx)
; CHECK-NEXT:    retq
entry:
        %p = load i64, i64* %x
	%t0 = sub i64 %p, %y
        %t1 = xor i64 %t0, %u
	%t2 = and i64 %t1, 4294967295
        store i64 %t2, i64* %z
	ret void
}
define void @sfoo(i64 *%x, i64 *%y, i64* %z) nounwind readnone {
; CHECK-LABEL: sfoo:
; CHECK:       # %bb.0: # %entry
; CHECK-NEXT:    movl (%rdi), %eax
; CHECK-NEXT:    subl (%rsi), %eax
; CHECK-NEXT:    movq %rax, (%rdx)
; CHECK-NEXT:    retq
entry:
        %a = load i64, i64* %x
        %b = load i64, i64* %y
	%t0 = sub i64 %a, %b
	%t1 = and i64 %t0, 4294967295
        store i64 %t1, i64* %z
	ret void
}
define void @swya(i64 %y, i64* %z) nounwind readnone {
; CHECK-LABEL: swya:
; CHECK:       # %bb.0: # %entry
; CHECK-NEXT:    negl %edi
; CHECK-NEXT:    movq %rdi, (%rsi)
; CHECK-NEXT:    retq
entry:
	%t0 = sub i64 0, %y
	%t1 = and i64 %t0, 4294967295
        store i64 %t1, i64* %z
	ret void
}
define void @soze(i64 %y, i64* %z) nounwind readnone {
; CHECK-LABEL: soze:
; CHECK:       # %bb.0: # %entry
; CHECK-NEXT:    decl %edi
; CHECK-NEXT:    movq %rdi, (%rsi)
; CHECK-NEXT:    retq
entry:
	%t0 = sub i64 %y, 1
	%t1 = and i64 %t0, 4294967295
        store i64 %t1, i64* %z
	ret void
}
