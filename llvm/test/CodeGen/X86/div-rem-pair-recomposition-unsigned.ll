; NOTE: Assertions have been autogenerated by utils/update_llc_test_checks.py
; RUN: llc < %s -mtriple=i686-unknown-unknown   -mattr=+sse,+sse2 | FileCheck %s --check-prefixes=ALL,X86
; RUN: llc < %s -mtriple=x86_64-unknown-unknown -mattr=+sse,+sse2 | FileCheck %s --check-prefixes=ALL,X64

; If the target does not have a single div/rem operation,
; -div-rem-pairs pass will decompose the remainder calculation as:
;   X % Y --> X - ((X / Y) * Y)
; But if the target does have a single div/rem operation,
; the opposite transform is likely beneficial.

define i8 @scalar_i8(i8 %x, i8 %y, i8* %divdst) nounwind {
; X86-LABEL: scalar_i8:
; X86:       # %bb.0:
; X86-NEXT:    movl {{[0-9]+}}(%esp), %edx
; X86-NEXT:    movb {{[0-9]+}}(%esp), %ch
; X86-NEXT:    movb {{[0-9]+}}(%esp), %cl
; X86-NEXT:    movzbl %cl, %eax
; X86-NEXT:    divb %ch
; X86-NEXT:    movb %al, (%edx)
; X86-NEXT:    mulb %ch
; X86-NEXT:    subb %al, %cl
; X86-NEXT:    movl %ecx, %eax
; X86-NEXT:    retl
;
; X64-LABEL: scalar_i8:
; X64:       # %bb.0:
; X64-NEXT:    movzbl %dil, %ecx
; X64-NEXT:    movl %ecx, %eax
; X64-NEXT:    divb %sil
; X64-NEXT:    movb %al, (%rdx)
; X64-NEXT:    mulb %sil
; X64-NEXT:    subb %al, %cl
; X64-NEXT:    movl %ecx, %eax
; X64-NEXT:    retq
  %div = udiv i8 %x, %y
  store i8 %div, i8* %divdst, align 4
  %t1 = mul i8 %div, %y
  %t2 = sub i8 %x, %t1
  ret i8 %t2
}

define i16 @scalar_i16(i16 %x, i16 %y, i16* %divdst) nounwind {
; X86-LABEL: scalar_i16:
; X86:       # %bb.0:
; X86-NEXT:    pushl %edi
; X86-NEXT:    pushl %esi
; X86-NEXT:    movl {{[0-9]+}}(%esp), %ecx
; X86-NEXT:    movl {{[0-9]+}}(%esp), %esi
; X86-NEXT:    movl {{[0-9]+}}(%esp), %edi
; X86-NEXT:    movl %ecx, %eax
; X86-NEXT:    xorl %edx, %edx
; X86-NEXT:    divw %si
; X86-NEXT:    # kill: def $ax killed $ax def $eax
; X86-NEXT:    movw %ax, (%edi)
; X86-NEXT:    imull %eax, %esi
; X86-NEXT:    subl %esi, %ecx
; X86-NEXT:    movl %ecx, %eax
; X86-NEXT:    popl %esi
; X86-NEXT:    popl %edi
; X86-NEXT:    retl
;
; X64-LABEL: scalar_i16:
; X64:       # %bb.0:
; X64-NEXT:    movq %rdx, %rcx
; X64-NEXT:    movl %edi, %eax
; X64-NEXT:    xorl %edx, %edx
; X64-NEXT:    divw %si
; X64-NEXT:    # kill: def $ax killed $ax def $eax
; X64-NEXT:    movw %ax, (%rcx)
; X64-NEXT:    imull %eax, %esi
; X64-NEXT:    subl %esi, %edi
; X64-NEXT:    movl %edi, %eax
; X64-NEXT:    retq
  %div = udiv i16 %x, %y
  store i16 %div, i16* %divdst, align 4
  %t1 = mul i16 %div, %y
  %t2 = sub i16 %x, %t1
  ret i16 %t2
}

define i32 @scalar_i32(i32 %x, i32 %y, i32* %divdst) nounwind {
; X86-LABEL: scalar_i32:
; X86:       # %bb.0:
; X86-NEXT:    pushl %edi
; X86-NEXT:    pushl %esi
; X86-NEXT:    movl {{[0-9]+}}(%esp), %esi
; X86-NEXT:    movl {{[0-9]+}}(%esp), %ecx
; X86-NEXT:    movl {{[0-9]+}}(%esp), %edi
; X86-NEXT:    movl %ecx, %eax
; X86-NEXT:    xorl %edx, %edx
; X86-NEXT:    divl %edi
; X86-NEXT:    movl %eax, (%esi)
; X86-NEXT:    imull %edi, %eax
; X86-NEXT:    subl %eax, %ecx
; X86-NEXT:    movl %ecx, %eax
; X86-NEXT:    popl %esi
; X86-NEXT:    popl %edi
; X86-NEXT:    retl
;
; X64-LABEL: scalar_i32:
; X64:       # %bb.0:
; X64-NEXT:    movq %rdx, %rcx
; X64-NEXT:    movl %edi, %eax
; X64-NEXT:    xorl %edx, %edx
; X64-NEXT:    divl %esi
; X64-NEXT:    movl %eax, (%rcx)
; X64-NEXT:    imull %esi, %eax
; X64-NEXT:    subl %eax, %edi
; X64-NEXT:    movl %edi, %eax
; X64-NEXT:    retq
  %div = udiv i32 %x, %y
  store i32 %div, i32* %divdst, align 4
  %t1 = mul i32 %div, %y
  %t2 = sub i32 %x, %t1
  ret i32 %t2
}

define i64 @scalar_i64(i64 %x, i64 %y, i64* %divdst) nounwind {
; X86-LABEL: scalar_i64:
; X86:       # %bb.0:
; X86-NEXT:    pushl %ebp
; X86-NEXT:    pushl %ebx
; X86-NEXT:    pushl %edi
; X86-NEXT:    pushl %esi
; X86-NEXT:    movl {{[0-9]+}}(%esp), %esi
; X86-NEXT:    movl {{[0-9]+}}(%esp), %edi
; X86-NEXT:    movl {{[0-9]+}}(%esp), %ebp
; X86-NEXT:    movl {{[0-9]+}}(%esp), %ebx
; X86-NEXT:    pushl %ebx
; X86-NEXT:    pushl %ebp
; X86-NEXT:    pushl %edi
; X86-NEXT:    pushl %esi
; X86-NEXT:    calll __udivdi3
; X86-NEXT:    addl $16, %esp
; X86-NEXT:    movl %edx, %ecx
; X86-NEXT:    movl {{[0-9]+}}(%esp), %edx
; X86-NEXT:    movl %ecx, 4(%edx)
; X86-NEXT:    movl %eax, (%edx)
; X86-NEXT:    imull %eax, %ebx
; X86-NEXT:    mull %ebp
; X86-NEXT:    addl %ebx, %edx
; X86-NEXT:    imull %ebp, %ecx
; X86-NEXT:    addl %edx, %ecx
; X86-NEXT:    subl %eax, %esi
; X86-NEXT:    sbbl %ecx, %edi
; X86-NEXT:    movl %esi, %eax
; X86-NEXT:    movl %edi, %edx
; X86-NEXT:    popl %esi
; X86-NEXT:    popl %edi
; X86-NEXT:    popl %ebx
; X86-NEXT:    popl %ebp
; X86-NEXT:    retl
;
; X64-LABEL: scalar_i64:
; X64:       # %bb.0:
; X64-NEXT:    movq %rdx, %rcx
; X64-NEXT:    movq %rdi, %rax
; X64-NEXT:    xorl %edx, %edx
; X64-NEXT:    divq %rsi
; X64-NEXT:    movq %rax, (%rcx)
; X64-NEXT:    imulq %rsi, %rax
; X64-NEXT:    subq %rax, %rdi
; X64-NEXT:    movq %rdi, %rax
; X64-NEXT:    retq
  %div = udiv i64 %x, %y
  store i64 %div, i64* %divdst, align 4
  %t1 = mul i64 %div, %y
  %t2 = sub i64 %x, %t1
  ret i64 %t2
}

define <16 x i8> @vector_i128_i8(<16 x i8> %x, <16 x i8> %y, <16 x i8>* %divdst) nounwind {
; X86-LABEL: vector_i128_i8:
; X86:       # %bb.0:
; X86-NEXT:    pushl %ebp
; X86-NEXT:    movl %esp, %ebp
; X86-NEXT:    pushl %ebx
; X86-NEXT:    pushl %edi
; X86-NEXT:    pushl %esi
; X86-NEXT:    andl $-16, %esp
; X86-NEXT:    subl $48, %esp
; X86-NEXT:    movdqa %xmm0, (%esp)
; X86-NEXT:    movdqa %xmm1, {{[0-9]+}}(%esp)
; X86-NEXT:    movzbl {{[0-9]+}}(%esp), %eax
; X86-NEXT:    divb {{[0-9]+}}(%esp)
; X86-NEXT:    movzbl %al, %eax
; X86-NEXT:    movd %eax, %xmm2
; X86-NEXT:    movzbl {{[0-9]+}}(%esp), %eax
; X86-NEXT:    divb {{[0-9]+}}(%esp)
; X86-NEXT:    movzbl %al, %eax
; X86-NEXT:    movd %eax, %xmm3
; X86-NEXT:    punpcklbw {{.*#+}} xmm3 = xmm3[0],xmm2[0],xmm3[1],xmm2[1],xmm3[2],xmm2[2],xmm3[3],xmm2[3],xmm3[4],xmm2[4],xmm3[5],xmm2[5],xmm3[6],xmm2[6],xmm3[7],xmm2[7]
; X86-NEXT:    movzbl {{[0-9]+}}(%esp), %eax
; X86-NEXT:    divb {{[0-9]+}}(%esp)
; X86-NEXT:    movzbl %al, %eax
; X86-NEXT:    movd %eax, %xmm4
; X86-NEXT:    movzbl {{[0-9]+}}(%esp), %eax
; X86-NEXT:    divb {{[0-9]+}}(%esp)
; X86-NEXT:    movzbl %al, %eax
; X86-NEXT:    movd %eax, %xmm2
; X86-NEXT:    punpcklbw {{.*#+}} xmm2 = xmm2[0],xmm4[0],xmm2[1],xmm4[1],xmm2[2],xmm4[2],xmm2[3],xmm4[3],xmm2[4],xmm4[4],xmm2[5],xmm4[5],xmm2[6],xmm4[6],xmm2[7],xmm4[7]
; X86-NEXT:    punpcklwd {{.*#+}} xmm2 = xmm2[0],xmm3[0],xmm2[1],xmm3[1],xmm2[2],xmm3[2],xmm2[3],xmm3[3]
; X86-NEXT:    movzbl {{[0-9]+}}(%esp), %eax
; X86-NEXT:    divb {{[0-9]+}}(%esp)
; X86-NEXT:    movzbl %al, %eax
; X86-NEXT:    movd %eax, %xmm3
; X86-NEXT:    movzbl {{[0-9]+}}(%esp), %eax
; X86-NEXT:    divb {{[0-9]+}}(%esp)
; X86-NEXT:    movzbl %al, %eax
; X86-NEXT:    movd %eax, %xmm4
; X86-NEXT:    punpcklbw {{.*#+}} xmm4 = xmm4[0],xmm3[0],xmm4[1],xmm3[1],xmm4[2],xmm3[2],xmm4[3],xmm3[3],xmm4[4],xmm3[4],xmm4[5],xmm3[5],xmm4[6],xmm3[6],xmm4[7],xmm3[7]
; X86-NEXT:    movzbl {{[0-9]+}}(%esp), %eax
; X86-NEXT:    divb {{[0-9]+}}(%esp)
; X86-NEXT:    movzbl %al, %eax
; X86-NEXT:    movd %eax, %xmm5
; X86-NEXT:    movzbl {{[0-9]+}}(%esp), %eax
; X86-NEXT:    divb {{[0-9]+}}(%esp)
; X86-NEXT:    movzbl %al, %eax
; X86-NEXT:    movd %eax, %xmm3
; X86-NEXT:    punpcklbw {{.*#+}} xmm3 = xmm3[0],xmm5[0],xmm3[1],xmm5[1],xmm3[2],xmm5[2],xmm3[3],xmm5[3],xmm3[4],xmm5[4],xmm3[5],xmm5[5],xmm3[6],xmm5[6],xmm3[7],xmm5[7]
; X86-NEXT:    movzbl {{[0-9]+}}(%esp), %eax
; X86-NEXT:    divb {{[0-9]+}}(%esp)
; X86-NEXT:    movzbl %al, %eax
; X86-NEXT:    movd %eax, %xmm5
; X86-NEXT:    movzbl {{[0-9]+}}(%esp), %eax
; X86-NEXT:    divb {{[0-9]+}}(%esp)
; X86-NEXT:    movzbl %al, %eax
; X86-NEXT:    movd %eax, %xmm6
; X86-NEXT:    movzbl {{[0-9]+}}(%esp), %eax
; X86-NEXT:    divb {{[0-9]+}}(%esp)
; X86-NEXT:    movzbl %al, %edx
; X86-NEXT:    movzbl {{[0-9]+}}(%esp), %eax
; X86-NEXT:    divb {{[0-9]+}}(%esp)
; X86-NEXT:    movzbl %al, %esi
; X86-NEXT:    movzbl {{[0-9]+}}(%esp), %eax
; X86-NEXT:    divb {{[0-9]+}}(%esp)
; X86-NEXT:    movzbl %al, %edi
; X86-NEXT:    movzbl {{[0-9]+}}(%esp), %eax
; X86-NEXT:    divb {{[0-9]+}}(%esp)
; X86-NEXT:    movzbl %al, %ebx
; X86-NEXT:    movzbl {{[0-9]+}}(%esp), %eax
; X86-NEXT:    divb {{[0-9]+}}(%esp)
; X86-NEXT:    movl %eax, %ecx
; X86-NEXT:    movzbl (%esp), %eax
; X86-NEXT:    divb {{[0-9]+}}(%esp)
; X86-NEXT:    punpcklwd {{.*#+}} xmm3 = xmm3[0],xmm4[0],xmm3[1],xmm4[1],xmm3[2],xmm4[2],xmm3[3],xmm4[3]
; X86-NEXT:    movd %edx, %xmm4
; X86-NEXT:    punpckldq {{.*#+}} xmm3 = xmm3[0],xmm2[0],xmm3[1],xmm2[1]
; X86-NEXT:    movd %esi, %xmm2
; X86-NEXT:    punpcklbw {{.*#+}} xmm6 = xmm6[0],xmm5[0],xmm6[1],xmm5[1],xmm6[2],xmm5[2],xmm6[3],xmm5[3],xmm6[4],xmm5[4],xmm6[5],xmm5[5],xmm6[6],xmm5[6],xmm6[7],xmm5[7]
; X86-NEXT:    movd %edi, %xmm5
; X86-NEXT:    punpcklbw {{.*#+}} xmm2 = xmm2[0],xmm4[0],xmm2[1],xmm4[1],xmm2[2],xmm4[2],xmm2[3],xmm4[3],xmm2[4],xmm4[4],xmm2[5],xmm4[5],xmm2[6],xmm4[6],xmm2[7],xmm4[7]
; X86-NEXT:    punpcklwd {{.*#+}} xmm2 = xmm2[0],xmm6[0],xmm2[1],xmm6[1],xmm2[2],xmm6[2],xmm2[3],xmm6[3]
; X86-NEXT:    movd %ebx, %xmm4
; X86-NEXT:    movzbl %cl, %ecx
; X86-NEXT:    movd %ecx, %xmm6
; X86-NEXT:    movl 8(%ebp), %ecx
; X86-NEXT:    punpcklbw {{.*#+}} xmm4 = xmm4[0],xmm5[0],xmm4[1],xmm5[1],xmm4[2],xmm5[2],xmm4[3],xmm5[3],xmm4[4],xmm5[4],xmm4[5],xmm5[5],xmm4[6],xmm5[6],xmm4[7],xmm5[7]
; X86-NEXT:    movzbl %al, %eax
; X86-NEXT:    movd %eax, %xmm5
; X86-NEXT:    punpcklbw {{.*#+}} xmm5 = xmm5[0],xmm6[0],xmm5[1],xmm6[1],xmm5[2],xmm6[2],xmm5[3],xmm6[3],xmm5[4],xmm6[4],xmm5[5],xmm6[5],xmm5[6],xmm6[6],xmm5[7],xmm6[7]
; X86-NEXT:    punpcklwd {{.*#+}} xmm5 = xmm5[0],xmm4[0],xmm5[1],xmm4[1],xmm5[2],xmm4[2],xmm5[3],xmm4[3]
; X86-NEXT:    punpckldq {{.*#+}} xmm5 = xmm5[0],xmm2[0],xmm5[1],xmm2[1]
; X86-NEXT:    movdqa %xmm5, %xmm2
; X86-NEXT:    punpcklqdq {{.*#+}} xmm2 = xmm2[0],xmm3[0]
; X86-NEXT:    movdqa %xmm2, (%ecx)
; X86-NEXT:    punpcklbw {{.*#+}} xmm3 = xmm3[0],xmm0[0],xmm3[1],xmm0[1],xmm3[2],xmm0[2],xmm3[3],xmm0[3],xmm3[4],xmm0[4],xmm3[5],xmm0[5],xmm3[6],xmm0[6],xmm3[7],xmm0[7]
; X86-NEXT:    movdqa %xmm1, %xmm2
; X86-NEXT:    punpckhbw {{.*#+}} xmm2 = xmm2[8],xmm0[8],xmm2[9],xmm0[9],xmm2[10],xmm0[10],xmm2[11],xmm0[11],xmm2[12],xmm0[12],xmm2[13],xmm0[13],xmm2[14],xmm0[14],xmm2[15],xmm0[15]
; X86-NEXT:    pmullw %xmm3, %xmm2
; X86-NEXT:    movdqa {{.*#+}} xmm3 = [255,255,255,255,255,255,255,255]
; X86-NEXT:    pand %xmm3, %xmm2
; X86-NEXT:    punpcklbw {{.*#+}} xmm5 = xmm5[0],xmm0[0],xmm5[1],xmm0[1],xmm5[2],xmm0[2],xmm5[3],xmm0[3],xmm5[4],xmm0[4],xmm5[5],xmm0[5],xmm5[6],xmm0[6],xmm5[7],xmm0[7]
; X86-NEXT:    punpcklbw {{.*#+}} xmm1 = xmm1[0],xmm0[0],xmm1[1],xmm0[1],xmm1[2],xmm0[2],xmm1[3],xmm0[3],xmm1[4],xmm0[4],xmm1[5],xmm0[5],xmm1[6],xmm0[6],xmm1[7],xmm0[7]
; X86-NEXT:    pmullw %xmm5, %xmm1
; X86-NEXT:    pand %xmm3, %xmm1
; X86-NEXT:    packuswb %xmm2, %xmm1
; X86-NEXT:    psubb %xmm1, %xmm0
; X86-NEXT:    leal -12(%ebp), %esp
; X86-NEXT:    popl %esi
; X86-NEXT:    popl %edi
; X86-NEXT:    popl %ebx
; X86-NEXT:    popl %ebp
; X86-NEXT:    retl
;
; X64-LABEL: vector_i128_i8:
; X64:       # %bb.0:
; X64-NEXT:    pushq %rbp
; X64-NEXT:    pushq %r15
; X64-NEXT:    pushq %r14
; X64-NEXT:    pushq %r13
; X64-NEXT:    pushq %r12
; X64-NEXT:    pushq %rbx
; X64-NEXT:    movq %rdi, {{[-0-9]+}}(%r{{[sb]}}p) # 8-byte Spill
; X64-NEXT:    movdqa %xmm0, -{{[0-9]+}}(%rsp)
; X64-NEXT:    movdqa %xmm1, -{{[0-9]+}}(%rsp)
; X64-NEXT:    movzbl -{{[0-9]+}}(%rsp), %eax
; X64-NEXT:    divb -{{[0-9]+}}(%rsp)
; X64-NEXT:    movzbl %al, %eax
; X64-NEXT:    movd %eax, %xmm2
; X64-NEXT:    movzbl -{{[0-9]+}}(%rsp), %eax
; X64-NEXT:    divb -{{[0-9]+}}(%rsp)
; X64-NEXT:    movzbl %al, %r8d
; X64-NEXT:    movzbl -{{[0-9]+}}(%rsp), %eax
; X64-NEXT:    divb -{{[0-9]+}}(%rsp)
; X64-NEXT:    movzbl %al, %r9d
; X64-NEXT:    movzbl -{{[0-9]+}}(%rsp), %eax
; X64-NEXT:    divb -{{[0-9]+}}(%rsp)
; X64-NEXT:    movzbl %al, %r10d
; X64-NEXT:    movzbl -{{[0-9]+}}(%rsp), %eax
; X64-NEXT:    divb -{{[0-9]+}}(%rsp)
; X64-NEXT:    movzbl %al, %r11d
; X64-NEXT:    movzbl -{{[0-9]+}}(%rsp), %eax
; X64-NEXT:    divb -{{[0-9]+}}(%rsp)
; X64-NEXT:    movzbl %al, %r14d
; X64-NEXT:    movzbl -{{[0-9]+}}(%rsp), %eax
; X64-NEXT:    divb -{{[0-9]+}}(%rsp)
; X64-NEXT:    movzbl %al, %r15d
; X64-NEXT:    movzbl -{{[0-9]+}}(%rsp), %eax
; X64-NEXT:    divb -{{[0-9]+}}(%rsp)
; X64-NEXT:    movzbl %al, %r12d
; X64-NEXT:    movzbl -{{[0-9]+}}(%rsp), %eax
; X64-NEXT:    divb -{{[0-9]+}}(%rsp)
; X64-NEXT:    movzbl %al, %r13d
; X64-NEXT:    movzbl -{{[0-9]+}}(%rsp), %eax
; X64-NEXT:    divb -{{[0-9]+}}(%rsp)
; X64-NEXT:    movzbl %al, %edi
; X64-NEXT:    movzbl -{{[0-9]+}}(%rsp), %eax
; X64-NEXT:    divb -{{[0-9]+}}(%rsp)
; X64-NEXT:    movzbl %al, %esi
; X64-NEXT:    movzbl -{{[0-9]+}}(%rsp), %eax
; X64-NEXT:    divb -{{[0-9]+}}(%rsp)
; X64-NEXT:    movzbl %al, %ebx
; X64-NEXT:    movzbl -{{[0-9]+}}(%rsp), %eax
; X64-NEXT:    divb -{{[0-9]+}}(%rsp)
; X64-NEXT:    movzbl %al, %ebp
; X64-NEXT:    movzbl -{{[0-9]+}}(%rsp), %eax
; X64-NEXT:    divb -{{[0-9]+}}(%rsp)
; X64-NEXT:    movzbl %al, %edx
; X64-NEXT:    movzbl -{{[0-9]+}}(%rsp), %eax
; X64-NEXT:    divb -{{[0-9]+}}(%rsp)
; X64-NEXT:    movl %eax, %ecx
; X64-NEXT:    movzbl -{{[0-9]+}}(%rsp), %eax
; X64-NEXT:    divb -{{[0-9]+}}(%rsp)
; X64-NEXT:    movd %r8d, %xmm3
; X64-NEXT:    movd %r9d, %xmm4
; X64-NEXT:    movd %r10d, %xmm5
; X64-NEXT:    movd %r11d, %xmm6
; X64-NEXT:    punpcklbw {{.*#+}} xmm3 = xmm3[0],xmm2[0],xmm3[1],xmm2[1],xmm3[2],xmm2[2],xmm3[3],xmm2[3],xmm3[4],xmm2[4],xmm3[5],xmm2[5],xmm3[6],xmm2[6],xmm3[7],xmm2[7]
; X64-NEXT:    movd %r14d, %xmm2
; X64-NEXT:    punpcklbw {{.*#+}} xmm5 = xmm5[0],xmm4[0],xmm5[1],xmm4[1],xmm5[2],xmm4[2],xmm5[3],xmm4[3],xmm5[4],xmm4[4],xmm5[5],xmm4[5],xmm5[6],xmm4[6],xmm5[7],xmm4[7]
; X64-NEXT:    movd %r15d, %xmm4
; X64-NEXT:    punpcklwd {{.*#+}} xmm5 = xmm5[0],xmm3[0],xmm5[1],xmm3[1],xmm5[2],xmm3[2],xmm5[3],xmm3[3]
; X64-NEXT:    movd %r12d, %xmm3
; X64-NEXT:    punpcklbw {{.*#+}} xmm2 = xmm2[0],xmm6[0],xmm2[1],xmm6[1],xmm2[2],xmm6[2],xmm2[3],xmm6[3],xmm2[4],xmm6[4],xmm2[5],xmm6[5],xmm2[6],xmm6[6],xmm2[7],xmm6[7]
; X64-NEXT:    movd %r13d, %xmm6
; X64-NEXT:    punpcklbw {{.*#+}} xmm3 = xmm3[0],xmm4[0],xmm3[1],xmm4[1],xmm3[2],xmm4[2],xmm3[3],xmm4[3],xmm3[4],xmm4[4],xmm3[5],xmm4[5],xmm3[6],xmm4[6],xmm3[7],xmm4[7]
; X64-NEXT:    movd %edi, %xmm4
; X64-NEXT:    punpcklwd {{.*#+}} xmm3 = xmm3[0],xmm2[0],xmm3[1],xmm2[1],xmm3[2],xmm2[2],xmm3[3],xmm2[3]
; X64-NEXT:    movd %esi, %xmm2
; X64-NEXT:    punpckldq {{.*#+}} xmm3 = xmm3[0],xmm5[0],xmm3[1],xmm5[1]
; X64-NEXT:    movd %ebx, %xmm5
; X64-NEXT:    punpcklbw {{.*#+}} xmm4 = xmm4[0],xmm6[0],xmm4[1],xmm6[1],xmm4[2],xmm6[2],xmm4[3],xmm6[3],xmm4[4],xmm6[4],xmm4[5],xmm6[5],xmm4[6],xmm6[6],xmm4[7],xmm6[7]
; X64-NEXT:    movd %ebp, %xmm6
; X64-NEXT:    punpcklbw {{.*#+}} xmm5 = xmm5[0],xmm2[0],xmm5[1],xmm2[1],xmm5[2],xmm2[2],xmm5[3],xmm2[3],xmm5[4],xmm2[4],xmm5[5],xmm2[5],xmm5[6],xmm2[6],xmm5[7],xmm2[7]
; X64-NEXT:    movd %edx, %xmm2
; X64-NEXT:    punpcklwd {{.*#+}} xmm5 = xmm5[0],xmm4[0],xmm5[1],xmm4[1],xmm5[2],xmm4[2],xmm5[3],xmm4[3]
; X64-NEXT:    movzbl %cl, %ecx
; X64-NEXT:    movd %ecx, %xmm4
; X64-NEXT:    punpcklbw {{.*#+}} xmm2 = xmm2[0],xmm6[0],xmm2[1],xmm6[1],xmm2[2],xmm6[2],xmm2[3],xmm6[3],xmm2[4],xmm6[4],xmm2[5],xmm6[5],xmm2[6],xmm6[6],xmm2[7],xmm6[7]
; X64-NEXT:    movzbl %al, %eax
; X64-NEXT:    movd %eax, %xmm6
; X64-NEXT:    punpcklbw {{.*#+}} xmm6 = xmm6[0],xmm4[0],xmm6[1],xmm4[1],xmm6[2],xmm4[2],xmm6[3],xmm4[3],xmm6[4],xmm4[4],xmm6[5],xmm4[5],xmm6[6],xmm4[6],xmm6[7],xmm4[7]
; X64-NEXT:    punpcklwd {{.*#+}} xmm6 = xmm6[0],xmm2[0],xmm6[1],xmm2[1],xmm6[2],xmm2[2],xmm6[3],xmm2[3]
; X64-NEXT:    punpckldq {{.*#+}} xmm6 = xmm6[0],xmm5[0],xmm6[1],xmm5[1]
; X64-NEXT:    movdqa %xmm6, %xmm2
; X64-NEXT:    punpcklqdq {{.*#+}} xmm2 = xmm2[0],xmm3[0]
; X64-NEXT:    movq {{[-0-9]+}}(%r{{[sb]}}p), %rax # 8-byte Reload
; X64-NEXT:    movdqa %xmm2, (%rax)
; X64-NEXT:    punpcklbw {{.*#+}} xmm3 = xmm3[0],xmm0[0],xmm3[1],xmm0[1],xmm3[2],xmm0[2],xmm3[3],xmm0[3],xmm3[4],xmm0[4],xmm3[5],xmm0[5],xmm3[6],xmm0[6],xmm3[7],xmm0[7]
; X64-NEXT:    movdqa %xmm1, %xmm2
; X64-NEXT:    punpckhbw {{.*#+}} xmm2 = xmm2[8],xmm0[8],xmm2[9],xmm0[9],xmm2[10],xmm0[10],xmm2[11],xmm0[11],xmm2[12],xmm0[12],xmm2[13],xmm0[13],xmm2[14],xmm0[14],xmm2[15],xmm0[15]
; X64-NEXT:    pmullw %xmm3, %xmm2
; X64-NEXT:    movdqa {{.*#+}} xmm3 = [255,255,255,255,255,255,255,255]
; X64-NEXT:    pand %xmm3, %xmm2
; X64-NEXT:    punpcklbw {{.*#+}} xmm6 = xmm6[0],xmm0[0],xmm6[1],xmm0[1],xmm6[2],xmm0[2],xmm6[3],xmm0[3],xmm6[4],xmm0[4],xmm6[5],xmm0[5],xmm6[6],xmm0[6],xmm6[7],xmm0[7]
; X64-NEXT:    punpcklbw {{.*#+}} xmm1 = xmm1[0],xmm0[0],xmm1[1],xmm0[1],xmm1[2],xmm0[2],xmm1[3],xmm0[3],xmm1[4],xmm0[4],xmm1[5],xmm0[5],xmm1[6],xmm0[6],xmm1[7],xmm0[7]
; X64-NEXT:    pmullw %xmm6, %xmm1
; X64-NEXT:    pand %xmm3, %xmm1
; X64-NEXT:    packuswb %xmm2, %xmm1
; X64-NEXT:    psubb %xmm1, %xmm0
; X64-NEXT:    popq %rbx
; X64-NEXT:    popq %r12
; X64-NEXT:    popq %r13
; X64-NEXT:    popq %r14
; X64-NEXT:    popq %r15
; X64-NEXT:    popq %rbp
; X64-NEXT:    retq
  %div = udiv <16 x i8> %x, %y
  store <16 x i8> %div, <16 x i8>* %divdst, align 16
  %t1 = mul <16 x i8> %div, %y
  %t2 = sub <16 x i8> %x, %t1
  ret <16 x i8> %t2
}

define <8 x i16> @vector_i128_i16(<8 x i16> %x, <8 x i16> %y, <8 x i16>* %divdst) nounwind {
; X86-LABEL: vector_i128_i16:
; X86:       # %bb.0:
; X86-NEXT:    pushl %esi
; X86-NEXT:    movl {{[0-9]+}}(%esp), %ecx
; X86-NEXT:    pextrw $7, %xmm0, %eax
; X86-NEXT:    pextrw $7, %xmm1, %esi
; X86-NEXT:    # kill: def $ax killed $ax killed $eax
; X86-NEXT:    xorl %edx, %edx
; X86-NEXT:    divw %si
; X86-NEXT:    # kill: def $ax killed $ax def $eax
; X86-NEXT:    movd %eax, %xmm2
; X86-NEXT:    pextrw $6, %xmm0, %eax
; X86-NEXT:    pextrw $6, %xmm1, %esi
; X86-NEXT:    # kill: def $ax killed $ax killed $eax
; X86-NEXT:    xorl %edx, %edx
; X86-NEXT:    divw %si
; X86-NEXT:    # kill: def $ax killed $ax def $eax
; X86-NEXT:    movd %eax, %xmm3
; X86-NEXT:    punpcklwd {{.*#+}} xmm3 = xmm3[0],xmm2[0],xmm3[1],xmm2[1],xmm3[2],xmm2[2],xmm3[3],xmm2[3]
; X86-NEXT:    pextrw $5, %xmm0, %eax
; X86-NEXT:    pextrw $5, %xmm1, %esi
; X86-NEXT:    # kill: def $ax killed $ax killed $eax
; X86-NEXT:    xorl %edx, %edx
; X86-NEXT:    divw %si
; X86-NEXT:    # kill: def $ax killed $ax def $eax
; X86-NEXT:    movd %eax, %xmm4
; X86-NEXT:    pextrw $4, %xmm0, %eax
; X86-NEXT:    pextrw $4, %xmm1, %esi
; X86-NEXT:    # kill: def $ax killed $ax killed $eax
; X86-NEXT:    xorl %edx, %edx
; X86-NEXT:    divw %si
; X86-NEXT:    # kill: def $ax killed $ax def $eax
; X86-NEXT:    movd %eax, %xmm2
; X86-NEXT:    punpcklwd {{.*#+}} xmm2 = xmm2[0],xmm4[0],xmm2[1],xmm4[1],xmm2[2],xmm4[2],xmm2[3],xmm4[3]
; X86-NEXT:    punpckldq {{.*#+}} xmm2 = xmm2[0],xmm3[0],xmm2[1],xmm3[1]
; X86-NEXT:    pextrw $3, %xmm0, %eax
; X86-NEXT:    pextrw $3, %xmm1, %esi
; X86-NEXT:    # kill: def $ax killed $ax killed $eax
; X86-NEXT:    xorl %edx, %edx
; X86-NEXT:    divw %si
; X86-NEXT:    # kill: def $ax killed $ax def $eax
; X86-NEXT:    movd %eax, %xmm3
; X86-NEXT:    pextrw $2, %xmm0, %eax
; X86-NEXT:    pextrw $2, %xmm1, %esi
; X86-NEXT:    # kill: def $ax killed $ax killed $eax
; X86-NEXT:    xorl %edx, %edx
; X86-NEXT:    divw %si
; X86-NEXT:    # kill: def $ax killed $ax def $eax
; X86-NEXT:    movd %eax, %xmm4
; X86-NEXT:    punpcklwd {{.*#+}} xmm4 = xmm4[0],xmm3[0],xmm4[1],xmm3[1],xmm4[2],xmm3[2],xmm4[3],xmm3[3]
; X86-NEXT:    pextrw $1, %xmm0, %eax
; X86-NEXT:    pextrw $1, %xmm1, %esi
; X86-NEXT:    # kill: def $ax killed $ax killed $eax
; X86-NEXT:    xorl %edx, %edx
; X86-NEXT:    divw %si
; X86-NEXT:    # kill: def $ax killed $ax def $eax
; X86-NEXT:    movd %eax, %xmm3
; X86-NEXT:    movd %xmm0, %eax
; X86-NEXT:    movd %xmm1, %esi
; X86-NEXT:    # kill: def $ax killed $ax killed $eax
; X86-NEXT:    xorl %edx, %edx
; X86-NEXT:    divw %si
; X86-NEXT:    # kill: def $ax killed $ax def $eax
; X86-NEXT:    movd %eax, %xmm5
; X86-NEXT:    punpcklwd {{.*#+}} xmm5 = xmm5[0],xmm3[0],xmm5[1],xmm3[1],xmm5[2],xmm3[2],xmm5[3],xmm3[3]
; X86-NEXT:    punpckldq {{.*#+}} xmm5 = xmm5[0],xmm4[0],xmm5[1],xmm4[1]
; X86-NEXT:    punpcklqdq {{.*#+}} xmm5 = xmm5[0],xmm2[0]
; X86-NEXT:    movdqa %xmm5, (%ecx)
; X86-NEXT:    pmullw %xmm1, %xmm5
; X86-NEXT:    psubw %xmm5, %xmm0
; X86-NEXT:    popl %esi
; X86-NEXT:    retl
;
; X64-LABEL: vector_i128_i16:
; X64:       # %bb.0:
; X64-NEXT:    pextrw $7, %xmm0, %eax
; X64-NEXT:    pextrw $7, %xmm1, %ecx
; X64-NEXT:    # kill: def $ax killed $ax killed $eax
; X64-NEXT:    xorl %edx, %edx
; X64-NEXT:    divw %cx
; X64-NEXT:    # kill: def $ax killed $ax def $eax
; X64-NEXT:    movd %eax, %xmm2
; X64-NEXT:    pextrw $6, %xmm0, %eax
; X64-NEXT:    pextrw $6, %xmm1, %ecx
; X64-NEXT:    # kill: def $ax killed $ax killed $eax
; X64-NEXT:    xorl %edx, %edx
; X64-NEXT:    divw %cx
; X64-NEXT:    # kill: def $ax killed $ax def $eax
; X64-NEXT:    movd %eax, %xmm3
; X64-NEXT:    punpcklwd {{.*#+}} xmm3 = xmm3[0],xmm2[0],xmm3[1],xmm2[1],xmm3[2],xmm2[2],xmm3[3],xmm2[3]
; X64-NEXT:    pextrw $5, %xmm0, %eax
; X64-NEXT:    pextrw $5, %xmm1, %ecx
; X64-NEXT:    # kill: def $ax killed $ax killed $eax
; X64-NEXT:    xorl %edx, %edx
; X64-NEXT:    divw %cx
; X64-NEXT:    # kill: def $ax killed $ax def $eax
; X64-NEXT:    movd %eax, %xmm4
; X64-NEXT:    pextrw $4, %xmm0, %eax
; X64-NEXT:    pextrw $4, %xmm1, %ecx
; X64-NEXT:    # kill: def $ax killed $ax killed $eax
; X64-NEXT:    xorl %edx, %edx
; X64-NEXT:    divw %cx
; X64-NEXT:    # kill: def $ax killed $ax def $eax
; X64-NEXT:    movd %eax, %xmm2
; X64-NEXT:    punpcklwd {{.*#+}} xmm2 = xmm2[0],xmm4[0],xmm2[1],xmm4[1],xmm2[2],xmm4[2],xmm2[3],xmm4[3]
; X64-NEXT:    punpckldq {{.*#+}} xmm2 = xmm2[0],xmm3[0],xmm2[1],xmm3[1]
; X64-NEXT:    pextrw $3, %xmm0, %eax
; X64-NEXT:    pextrw $3, %xmm1, %ecx
; X64-NEXT:    # kill: def $ax killed $ax killed $eax
; X64-NEXT:    xorl %edx, %edx
; X64-NEXT:    divw %cx
; X64-NEXT:    # kill: def $ax killed $ax def $eax
; X64-NEXT:    movd %eax, %xmm3
; X64-NEXT:    pextrw $2, %xmm0, %eax
; X64-NEXT:    pextrw $2, %xmm1, %ecx
; X64-NEXT:    # kill: def $ax killed $ax killed $eax
; X64-NEXT:    xorl %edx, %edx
; X64-NEXT:    divw %cx
; X64-NEXT:    # kill: def $ax killed $ax def $eax
; X64-NEXT:    movd %eax, %xmm4
; X64-NEXT:    punpcklwd {{.*#+}} xmm4 = xmm4[0],xmm3[0],xmm4[1],xmm3[1],xmm4[2],xmm3[2],xmm4[3],xmm3[3]
; X64-NEXT:    pextrw $1, %xmm0, %eax
; X64-NEXT:    pextrw $1, %xmm1, %ecx
; X64-NEXT:    # kill: def $ax killed $ax killed $eax
; X64-NEXT:    xorl %edx, %edx
; X64-NEXT:    divw %cx
; X64-NEXT:    # kill: def $ax killed $ax def $eax
; X64-NEXT:    movd %eax, %xmm3
; X64-NEXT:    movd %xmm0, %eax
; X64-NEXT:    movd %xmm1, %ecx
; X64-NEXT:    # kill: def $ax killed $ax killed $eax
; X64-NEXT:    xorl %edx, %edx
; X64-NEXT:    divw %cx
; X64-NEXT:    # kill: def $ax killed $ax def $eax
; X64-NEXT:    movd %eax, %xmm5
; X64-NEXT:    punpcklwd {{.*#+}} xmm5 = xmm5[0],xmm3[0],xmm5[1],xmm3[1],xmm5[2],xmm3[2],xmm5[3],xmm3[3]
; X64-NEXT:    punpckldq {{.*#+}} xmm5 = xmm5[0],xmm4[0],xmm5[1],xmm4[1]
; X64-NEXT:    punpcklqdq {{.*#+}} xmm5 = xmm5[0],xmm2[0]
; X64-NEXT:    movdqa %xmm5, (%rdi)
; X64-NEXT:    pmullw %xmm1, %xmm5
; X64-NEXT:    psubw %xmm5, %xmm0
; X64-NEXT:    retq
  %div = udiv <8 x i16> %x, %y
  store <8 x i16> %div, <8 x i16>* %divdst, align 16
  %t1 = mul <8 x i16> %div, %y
  %t2 = sub <8 x i16> %x, %t1
  ret <8 x i16> %t2
}

define <4 x i32> @vector_i128_i32(<4 x i32> %x, <4 x i32> %y, <4 x i32>* %divdst) nounwind {
; X86-LABEL: vector_i128_i32:
; X86:       # %bb.0:
; X86-NEXT:    pushl %esi
; X86-NEXT:    movl {{[0-9]+}}(%esp), %ecx
; X86-NEXT:    pshufd {{.*#+}} xmm2 = xmm0[3,1,2,3]
; X86-NEXT:    movd %xmm2, %eax
; X86-NEXT:    pshufd {{.*#+}} xmm2 = xmm1[3,1,2,3]
; X86-NEXT:    movd %xmm2, %esi
; X86-NEXT:    xorl %edx, %edx
; X86-NEXT:    divl %esi
; X86-NEXT:    movd %eax, %xmm2
; X86-NEXT:    pshufd {{.*#+}} xmm3 = xmm0[2,3,0,1]
; X86-NEXT:    movd %xmm3, %eax
; X86-NEXT:    pshufd {{.*#+}} xmm3 = xmm1[2,3,0,1]
; X86-NEXT:    movd %xmm3, %esi
; X86-NEXT:    xorl %edx, %edx
; X86-NEXT:    divl %esi
; X86-NEXT:    movd %eax, %xmm3
; X86-NEXT:    punpckldq {{.*#+}} xmm3 = xmm3[0],xmm2[0],xmm3[1],xmm2[1]
; X86-NEXT:    movd %xmm0, %eax
; X86-NEXT:    movd %xmm1, %esi
; X86-NEXT:    xorl %edx, %edx
; X86-NEXT:    divl %esi
; X86-NEXT:    movd %eax, %xmm4
; X86-NEXT:    pshufd {{.*#+}} xmm5 = xmm0[1,1,2,3]
; X86-NEXT:    movd %xmm5, %eax
; X86-NEXT:    pshufd {{.*#+}} xmm5 = xmm1[1,1,2,3]
; X86-NEXT:    movd %xmm5, %esi
; X86-NEXT:    xorl %edx, %edx
; X86-NEXT:    divl %esi
; X86-NEXT:    movd %eax, %xmm5
; X86-NEXT:    punpckldq {{.*#+}} xmm4 = xmm4[0],xmm5[0],xmm4[1],xmm5[1]
; X86-NEXT:    punpcklqdq {{.*#+}} xmm4 = xmm4[0],xmm3[0]
; X86-NEXT:    movdqa %xmm4, (%ecx)
; X86-NEXT:    pmuludq %xmm1, %xmm4
; X86-NEXT:    pshufd {{.*#+}} xmm3 = xmm4[0,2,2,3]
; X86-NEXT:    shufps {{.*#+}} xmm5 = xmm5[0,0],xmm2[0,0]
; X86-NEXT:    pshufd {{.*#+}} xmm1 = xmm1[1,1,3,3]
; X86-NEXT:    pmuludq %xmm5, %xmm1
; X86-NEXT:    pshufd {{.*#+}} xmm1 = xmm1[0,2,2,3]
; X86-NEXT:    punpckldq {{.*#+}} xmm3 = xmm3[0],xmm1[0],xmm3[1],xmm1[1]
; X86-NEXT:    psubd %xmm3, %xmm0
; X86-NEXT:    popl %esi
; X86-NEXT:    retl
;
; X64-LABEL: vector_i128_i32:
; X64:       # %bb.0:
; X64-NEXT:    pshufd {{.*#+}} xmm2 = xmm0[3,1,2,3]
; X64-NEXT:    movd %xmm2, %eax
; X64-NEXT:    pshufd {{.*#+}} xmm2 = xmm1[3,1,2,3]
; X64-NEXT:    movd %xmm2, %ecx
; X64-NEXT:    xorl %edx, %edx
; X64-NEXT:    divl %ecx
; X64-NEXT:    movd %eax, %xmm2
; X64-NEXT:    pshufd {{.*#+}} xmm3 = xmm0[2,3,0,1]
; X64-NEXT:    movd %xmm3, %eax
; X64-NEXT:    pshufd {{.*#+}} xmm3 = xmm1[2,3,0,1]
; X64-NEXT:    movd %xmm3, %ecx
; X64-NEXT:    xorl %edx, %edx
; X64-NEXT:    divl %ecx
; X64-NEXT:    movd %eax, %xmm3
; X64-NEXT:    punpckldq {{.*#+}} xmm3 = xmm3[0],xmm2[0],xmm3[1],xmm2[1]
; X64-NEXT:    movd %xmm0, %eax
; X64-NEXT:    movd %xmm1, %ecx
; X64-NEXT:    xorl %edx, %edx
; X64-NEXT:    divl %ecx
; X64-NEXT:    movd %eax, %xmm4
; X64-NEXT:    pshufd {{.*#+}} xmm5 = xmm0[1,1,2,3]
; X64-NEXT:    movd %xmm5, %eax
; X64-NEXT:    pshufd {{.*#+}} xmm5 = xmm1[1,1,2,3]
; X64-NEXT:    movd %xmm5, %ecx
; X64-NEXT:    xorl %edx, %edx
; X64-NEXT:    divl %ecx
; X64-NEXT:    movd %eax, %xmm5
; X64-NEXT:    punpckldq {{.*#+}} xmm4 = xmm4[0],xmm5[0],xmm4[1],xmm5[1]
; X64-NEXT:    punpcklqdq {{.*#+}} xmm4 = xmm4[0],xmm3[0]
; X64-NEXT:    movdqa %xmm4, (%rdi)
; X64-NEXT:    pmuludq %xmm1, %xmm4
; X64-NEXT:    pshufd {{.*#+}} xmm3 = xmm4[0,2,2,3]
; X64-NEXT:    shufps {{.*#+}} xmm5 = xmm5[0,0],xmm2[0,0]
; X64-NEXT:    pshufd {{.*#+}} xmm1 = xmm1[1,1,3,3]
; X64-NEXT:    pmuludq %xmm5, %xmm1
; X64-NEXT:    pshufd {{.*#+}} xmm1 = xmm1[0,2,2,3]
; X64-NEXT:    punpckldq {{.*#+}} xmm3 = xmm3[0],xmm1[0],xmm3[1],xmm1[1]
; X64-NEXT:    psubd %xmm3, %xmm0
; X64-NEXT:    retq
  %div = udiv <4 x i32> %x, %y
  store <4 x i32> %div, <4 x i32>* %divdst, align 16
  %t1 = mul <4 x i32> %div, %y
  %t2 = sub <4 x i32> %x, %t1
  ret <4 x i32> %t2
}

define <2 x i64> @vector_i128_i64(<2 x i64> %x, <2 x i64> %y, <2 x i64>* %divdst) nounwind {
; X86-LABEL: vector_i128_i64:
; X86:       # %bb.0:
; X86-NEXT:    pushl %esi
; X86-NEXT:    subl $72, %esp
; X86-NEXT:    movdqu %xmm1, {{[-0-9]+}}(%e{{[sb]}}p) # 16-byte Spill
; X86-NEXT:    movdqu %xmm0, {{[-0-9]+}}(%e{{[sb]}}p) # 16-byte Spill
; X86-NEXT:    movl {{[0-9]+}}(%esp), %esi
; X86-NEXT:    pshufd {{.*#+}} xmm2 = xmm1[3,1,2,3]
; X86-NEXT:    movd %xmm2, {{[0-9]+}}(%esp)
; X86-NEXT:    pshufd {{.*#+}} xmm2 = xmm1[2,3,0,1]
; X86-NEXT:    movd %xmm2, {{[0-9]+}}(%esp)
; X86-NEXT:    pshufd {{.*#+}} xmm1 = xmm0[3,1,2,3]
; X86-NEXT:    movd %xmm1, {{[0-9]+}}(%esp)
; X86-NEXT:    pshufd {{.*#+}} xmm1 = xmm0[2,3,0,1]
; X86-NEXT:    movd %xmm1, (%esp)
; X86-NEXT:    calll __udivdi3
; X86-NEXT:    movdqu {{[-0-9]+}}(%e{{[sb]}}p), %xmm1 # 16-byte Reload
; X86-NEXT:    pshufd {{.*#+}} xmm0 = xmm1[1,1,2,3]
; X86-NEXT:    movd %xmm0, {{[0-9]+}}(%esp)
; X86-NEXT:    movd %xmm1, {{[0-9]+}}(%esp)
; X86-NEXT:    movdqu {{[-0-9]+}}(%e{{[sb]}}p), %xmm1 # 16-byte Reload
; X86-NEXT:    pshufd {{.*#+}} xmm0 = xmm1[1,1,2,3]
; X86-NEXT:    movd %xmm0, {{[0-9]+}}(%esp)
; X86-NEXT:    movd %xmm1, (%esp)
; X86-NEXT:    movd %edx, %xmm0
; X86-NEXT:    movd %eax, %xmm1
; X86-NEXT:    punpckldq {{.*#+}} xmm1 = xmm1[0],xmm0[0],xmm1[1],xmm0[1]
; X86-NEXT:    movdqu %xmm1, {{[-0-9]+}}(%e{{[sb]}}p) # 16-byte Spill
; X86-NEXT:    calll __udivdi3
; X86-NEXT:    movd %edx, %xmm0
; X86-NEXT:    movd %eax, %xmm1
; X86-NEXT:    punpckldq {{.*#+}} xmm1 = xmm1[0],xmm0[0],xmm1[1],xmm0[1]
; X86-NEXT:    movdqu {{[-0-9]+}}(%e{{[sb]}}p), %xmm0 # 16-byte Reload
; X86-NEXT:    punpcklqdq {{.*#+}} xmm1 = xmm1[0],xmm0[0]
; X86-NEXT:    movdqa %xmm1, (%esi)
; X86-NEXT:    movdqu {{[-0-9]+}}(%e{{[sb]}}p), %xmm3 # 16-byte Reload
; X86-NEXT:    movdqa %xmm3, %xmm0
; X86-NEXT:    psrlq $32, %xmm0
; X86-NEXT:    pmuludq %xmm1, %xmm0
; X86-NEXT:    movdqa %xmm1, %xmm2
; X86-NEXT:    psrlq $32, %xmm2
; X86-NEXT:    pmuludq %xmm3, %xmm2
; X86-NEXT:    paddq %xmm0, %xmm2
; X86-NEXT:    psllq $32, %xmm2
; X86-NEXT:    pmuludq %xmm3, %xmm1
; X86-NEXT:    paddq %xmm2, %xmm1
; X86-NEXT:    movdqu {{[-0-9]+}}(%e{{[sb]}}p), %xmm0 # 16-byte Reload
; X86-NEXT:    psubq %xmm1, %xmm0
; X86-NEXT:    addl $72, %esp
; X86-NEXT:    popl %esi
; X86-NEXT:    retl
;
; X64-LABEL: vector_i128_i64:
; X64:       # %bb.0:
; X64-NEXT:    movq %xmm0, %rax
; X64-NEXT:    movq %xmm1, %rcx
; X64-NEXT:    xorl %edx, %edx
; X64-NEXT:    divq %rcx
; X64-NEXT:    movq %rax, %xmm2
; X64-NEXT:    pshufd {{.*#+}} xmm3 = xmm0[2,3,0,1]
; X64-NEXT:    movq %xmm3, %rax
; X64-NEXT:    pshufd {{.*#+}} xmm3 = xmm1[2,3,0,1]
; X64-NEXT:    movq %xmm3, %rcx
; X64-NEXT:    xorl %edx, %edx
; X64-NEXT:    divq %rcx
; X64-NEXT:    movq %rax, %xmm3
; X64-NEXT:    punpcklqdq {{.*#+}} xmm2 = xmm2[0],xmm3[0]
; X64-NEXT:    movdqa %xmm2, (%rdi)
; X64-NEXT:    movdqa %xmm1, %xmm3
; X64-NEXT:    psrlq $32, %xmm3
; X64-NEXT:    pmuludq %xmm2, %xmm3
; X64-NEXT:    movdqa %xmm2, %xmm4
; X64-NEXT:    psrlq $32, %xmm4
; X64-NEXT:    pmuludq %xmm1, %xmm4
; X64-NEXT:    paddq %xmm3, %xmm4
; X64-NEXT:    psllq $32, %xmm4
; X64-NEXT:    pmuludq %xmm1, %xmm2
; X64-NEXT:    paddq %xmm4, %xmm2
; X64-NEXT:    psubq %xmm2, %xmm0
; X64-NEXT:    retq
  %div = udiv <2 x i64> %x, %y
  store <2 x i64> %div, <2 x i64>* %divdst, align 16
  %t1 = mul <2 x i64> %div, %y
  %t2 = sub <2 x i64> %x, %t1
  ret <2 x i64> %t2
}

; Special tests.

define i32 @scalar_i32_commutative(i32 %x, i32* %ysrc, i32* %divdst) nounwind {
; X86-LABEL: scalar_i32_commutative:
; X86:       # %bb.0:
; X86-NEXT:    pushl %edi
; X86-NEXT:    pushl %esi
; X86-NEXT:    movl {{[0-9]+}}(%esp), %esi
; X86-NEXT:    movl {{[0-9]+}}(%esp), %ecx
; X86-NEXT:    movl {{[0-9]+}}(%esp), %eax
; X86-NEXT:    movl (%eax), %edi
; X86-NEXT:    movl %ecx, %eax
; X86-NEXT:    xorl %edx, %edx
; X86-NEXT:    divl %edi
; X86-NEXT:    movl %eax, (%esi)
; X86-NEXT:    imull %eax, %edi
; X86-NEXT:    subl %edi, %ecx
; X86-NEXT:    movl %ecx, %eax
; X86-NEXT:    popl %esi
; X86-NEXT:    popl %edi
; X86-NEXT:    retl
;
; X64-LABEL: scalar_i32_commutative:
; X64:       # %bb.0:
; X64-NEXT:    movq %rdx, %rcx
; X64-NEXT:    movl (%rsi), %esi
; X64-NEXT:    movl %edi, %eax
; X64-NEXT:    xorl %edx, %edx
; X64-NEXT:    divl %esi
; X64-NEXT:    movl %eax, (%rcx)
; X64-NEXT:    imull %eax, %esi
; X64-NEXT:    subl %esi, %edi
; X64-NEXT:    movl %edi, %eax
; X64-NEXT:    retq
  %y = load i32, i32* %ysrc, align 4
  %div = udiv i32 %x, %y
  store i32 %div, i32* %divdst, align 4
  %t1 = mul i32 %y, %div ; commutative
  %t2 = sub i32 %x, %t1
  ret i32 %t2
}

; We do not care about extra uses.
define i32 @extrause(i32 %x, i32 %y, i32* %divdst, i32* %t1dst) nounwind {
; X86-LABEL: extrause:
; X86:       # %bb.0:
; X86-NEXT:    pushl %ebx
; X86-NEXT:    pushl %edi
; X86-NEXT:    pushl %esi
; X86-NEXT:    movl {{[0-9]+}}(%esp), %esi
; X86-NEXT:    movl {{[0-9]+}}(%esp), %edi
; X86-NEXT:    movl {{[0-9]+}}(%esp), %ecx
; X86-NEXT:    movl {{[0-9]+}}(%esp), %ebx
; X86-NEXT:    movl %ecx, %eax
; X86-NEXT:    xorl %edx, %edx
; X86-NEXT:    divl %ebx
; X86-NEXT:    movl %eax, (%edi)
; X86-NEXT:    imull %ebx, %eax
; X86-NEXT:    movl %eax, (%esi)
; X86-NEXT:    subl %eax, %ecx
; X86-NEXT:    movl %ecx, %eax
; X86-NEXT:    popl %esi
; X86-NEXT:    popl %edi
; X86-NEXT:    popl %ebx
; X86-NEXT:    retl
;
; X64-LABEL: extrause:
; X64:       # %bb.0:
; X64-NEXT:    movq %rdx, %r8
; X64-NEXT:    movl %edi, %eax
; X64-NEXT:    xorl %edx, %edx
; X64-NEXT:    divl %esi
; X64-NEXT:    movl %eax, (%r8)
; X64-NEXT:    imull %esi, %eax
; X64-NEXT:    movl %eax, (%rcx)
; X64-NEXT:    subl %eax, %edi
; X64-NEXT:    movl %edi, %eax
; X64-NEXT:    retq
  %div = udiv i32 %x, %y
  store i32 %div, i32* %divdst, align 4
  %t1 = mul i32 %div, %y
  store i32 %t1, i32* %t1dst, align 4
  %t2 = sub i32 %x, %t1
  ret i32 %t2
}

; 'rem' should appear next to 'div'.
define i32 @multiple_bb(i32 %x, i32 %y, i32* %divdst, i1 zeroext %store_urem, i32* %uremdst) nounwind {
; X86-LABEL: multiple_bb:
; X86:       # %bb.0:
; X86-NEXT:    pushl %ebx
; X86-NEXT:    pushl %edi
; X86-NEXT:    pushl %esi
; X86-NEXT:    movl {{[0-9]+}}(%esp), %esi
; X86-NEXT:    movl {{[0-9]+}}(%esp), %ecx
; X86-NEXT:    movb {{[0-9]+}}(%esp), %bl
; X86-NEXT:    movl {{[0-9]+}}(%esp), %edi
; X86-NEXT:    movl %ecx, %eax
; X86-NEXT:    xorl %edx, %edx
; X86-NEXT:    divl %esi
; X86-NEXT:    movl %eax, (%edi)
; X86-NEXT:    testb %bl, %bl
; X86-NEXT:    je .LBB10_2
; X86-NEXT:  # %bb.1: # %do_urem
; X86-NEXT:    movl {{[0-9]+}}(%esp), %edx
; X86-NEXT:    movl %eax, %edi
; X86-NEXT:    imull %esi, %edi
; X86-NEXT:    subl %edi, %ecx
; X86-NEXT:    movl %ecx, (%edx)
; X86-NEXT:  .LBB10_2: # %end
; X86-NEXT:    popl %esi
; X86-NEXT:    popl %edi
; X86-NEXT:    popl %ebx
; X86-NEXT:    retl
;
; X64-LABEL: multiple_bb:
; X64:       # %bb.0:
; X64-NEXT:    movq %rdx, %r9
; X64-NEXT:    movl %edi, %eax
; X64-NEXT:    xorl %edx, %edx
; X64-NEXT:    divl %esi
; X64-NEXT:    movl %eax, (%r9)
; X64-NEXT:    testb %cl, %cl
; X64-NEXT:    je .LBB10_2
; X64-NEXT:  # %bb.1: # %do_urem
; X64-NEXT:    movl %eax, %ecx
; X64-NEXT:    imull %esi, %ecx
; X64-NEXT:    subl %ecx, %edi
; X64-NEXT:    movl %edi, (%r8)
; X64-NEXT:  .LBB10_2: # %end
; X64-NEXT:    retq
  %div = udiv i32 %x, %y
  store i32 %div, i32* %divdst, align 4
  br i1 %store_urem, label %do_urem, label %end
do_urem:
  %t1 = mul i32 %div, %y
  %t2 = sub i32 %x, %t1
  store i32 %t2, i32* %uremdst, align 4
  br label %end
end:
  ret i32 %div
}

define i32 @negative_different_x(i32 %x0, i32 %x1, i32 %y, i32* %divdst) nounwind {
; X86-LABEL: negative_different_x:
; X86:       # %bb.0:
; X86-NEXT:    pushl %edi
; X86-NEXT:    pushl %esi
; X86-NEXT:    movl {{[0-9]+}}(%esp), %ecx
; X86-NEXT:    movl {{[0-9]+}}(%esp), %esi
; X86-NEXT:    movl {{[0-9]+}}(%esp), %eax
; X86-NEXT:    movl {{[0-9]+}}(%esp), %edi
; X86-NEXT:    xorl %edx, %edx
; X86-NEXT:    divl %edi
; X86-NEXT:    movl %eax, (%esi)
; X86-NEXT:    imull %edi, %eax
; X86-NEXT:    subl %eax, %ecx
; X86-NEXT:    movl %ecx, %eax
; X86-NEXT:    popl %esi
; X86-NEXT:    popl %edi
; X86-NEXT:    retl
;
; X64-LABEL: negative_different_x:
; X64:       # %bb.0:
; X64-NEXT:    movl %edx, %r8d
; X64-NEXT:    movl %edi, %eax
; X64-NEXT:    xorl %edx, %edx
; X64-NEXT:    divl %r8d
; X64-NEXT:    movl %eax, (%rcx)
; X64-NEXT:    imull %r8d, %eax
; X64-NEXT:    subl %eax, %esi
; X64-NEXT:    movl %esi, %eax
; X64-NEXT:    retq
  %div = udiv i32 %x0, %y ; not %x1
  store i32 %div, i32* %divdst, align 4
  %t1 = mul i32 %div, %y
  %t2 = sub i32 %x1, %t1 ; not %x0
  ret i32 %t2
}
