# NOTE: Assertions have been autogenerated by utils/update_mca_test_checks.py
# RUN: llvm-mca -mtriple=x86_64-unknown-unknown -mcpu=btver2 -all-views=false -instruction-info                < %s | FileCheck %s --check-prefix=NORMAL
# RUN: llvm-mca -mtriple=x86_64-unknown-unknown -mcpu=btver2 -all-views=false -instruction-info -show-encoding=false < %s | FileCheck %s --check-prefix=NORMAL
# RUN: llvm-mca -mtriple=x86_64-unknown-unknown -mcpu=btver2 -all-views=false -instruction-info -show-encoding < %s | FileCheck %s --check-prefix=WITHENCODINGS

  movq 0x170(%rbp), %r10
  lea (%r8,%r8,2), %r9d
  movsx %r9d, %r9
  inc %r8d
  movq 0x178(%rbp), %r11
  vmovups (%r10,%r9,4), %xmm3
  vpslldq $0x4, %xmm3, %xmm2
  vpslldq $0x4, %xmm3, %xmm4
  vaddps %xmm2, %xmm3, %xmm6
  vpslldq $0xc, %xmm3, %xmm5
  vaddps %xmm4, %xmm5, %xmm7
  vaddps %xmm6, %xmm7, %xmm8
  vaddps %xmm8, %xmm0, %xmm9
  vshufps $0xff, %xmm9, %xmm9, %xmm0
  vmovups %xmm9, (%r11,%r9,4)
  cmp %r8d, %esi
  jl -90

# NORMAL:             Instruction Info:
# NORMAL-NEXT:        [1]: #uOps
# NORMAL-NEXT:        [2]: Latency
# NORMAL-NEXT:        [3]: RThroughput
# NORMAL-NEXT:        [4]: MayLoad
# NORMAL-NEXT:        [5]: MayStore
# NORMAL-NEXT:        [6]: HasSideEffects (U)

# WITHENCODINGS:      Instruction Info:
# WITHENCODINGS-NEXT: [1]: #uOps
# WITHENCODINGS-NEXT: [2]: Latency
# WITHENCODINGS-NEXT: [3]: RThroughput
# WITHENCODINGS-NEXT: [4]: MayLoad
# WITHENCODINGS-NEXT: [5]: MayStore
# WITHENCODINGS-NEXT: [6]: HasSideEffects (U)
# WITHENCODINGS-NEXT: [7]: Encoding Size

# NORMAL:             [1]    [2]    [3]    [4]    [5]    [6]    Instructions:
# NORMAL-NEXT:         1      3     1.00    *                   movq	368(%rbp), %r10
# NORMAL-NEXT:         1      2     1.00                        leal	(%r8,%r8,2), %r9d
# NORMAL-NEXT:         1      1     0.50                        movslq	%r9d, %r9
# NORMAL-NEXT:         1      1     0.50                        incl	%r8d
# NORMAL-NEXT:         1      3     1.00    *                   movq	376(%rbp), %r11
# NORMAL-NEXT:         1      5     1.00    *                   vmovups	(%r10,%r9,4), %xmm3
# NORMAL-NEXT:         1      1     0.50                        vpslldq	$4, %xmm3, %xmm2
# NORMAL-NEXT:         1      1     0.50                        vpslldq	$4, %xmm3, %xmm4
# NORMAL-NEXT:         1      3     1.00                        vaddps	%xmm2, %xmm3, %xmm6
# NORMAL-NEXT:         1      1     0.50                        vpslldq	$12, %xmm3, %xmm5
# NORMAL-NEXT:         1      3     1.00                        vaddps	%xmm4, %xmm5, %xmm7
# NORMAL-NEXT:         1      3     1.00                        vaddps	%xmm6, %xmm7, %xmm8
# NORMAL-NEXT:         1      3     1.00                        vaddps	%xmm8, %xmm0, %xmm9
# NORMAL-NEXT:         1      1     0.50                        vshufps	$255, %xmm9, %xmm9, %xmm0
# NORMAL-NEXT:         1      1     1.00           *            vmovups	%xmm9, (%r11,%r9,4)
# NORMAL-NEXT:         1      1     0.50                        cmpl	%r8d, %esi
# NORMAL-NEXT:         1      1     0.50                        jl	-90

# WITHENCODINGS:      [1]    [2]    [3]    [4]    [5]    [6]    [7]    Encodings:                    Instructions:
# WITHENCODINGS-NEXT:  1      3     1.00    *                    7     4c 8b 95 70 01 00 00          movq	368(%rbp), %r10
# WITHENCODINGS-NEXT:  1      2     1.00                         4     47 8d 0c 40                   leal	(%r8,%r8,2), %r9d
# WITHENCODINGS-NEXT:  1      1     0.50                         3     4d 63 c9                      movslq	%r9d, %r9
# WITHENCODINGS-NEXT:  1      1     0.50                         3     41 ff c0                      incl	%r8d
# WITHENCODINGS-NEXT:  1      3     1.00    *                    7     4c 8b 9d 78 01 00 00          movq	376(%rbp), %r11
# WITHENCODINGS-NEXT:  1      5     1.00    *                    6     c4 81 78 10 1c 8a             vmovups	(%r10,%r9,4), %xmm3
# WITHENCODINGS-NEXT:  1      1     0.50                         5     c5 e9 73 fb 04                vpslldq	$4, %xmm3, %xmm2
# WITHENCODINGS-NEXT:  1      1     0.50                         5     c5 d9 73 fb 04                vpslldq	$4, %xmm3, %xmm4
# WITHENCODINGS-NEXT:  1      3     1.00                         4     c5 e0 58 f2                   vaddps	%xmm2, %xmm3, %xmm6
# WITHENCODINGS-NEXT:  1      1     0.50                         5     c5 d1 73 fb 0c                vpslldq	$12, %xmm3, %xmm5
# WITHENCODINGS-NEXT:  1      3     1.00                         4     c5 d0 58 fc                   vaddps	%xmm4, %xmm5, %xmm7
# WITHENCODINGS-NEXT:  1      3     1.00                         4     c5 40 58 c6                   vaddps	%xmm6, %xmm7, %xmm8
# WITHENCODINGS-NEXT:  1      3     1.00                         5     c4 41 78 58 c8                vaddps	%xmm8, %xmm0, %xmm9
# WITHENCODINGS-NEXT:  1      1     0.50                         6     c4 c1 30 c6 c1 ff             vshufps	$255, %xmm9, %xmm9, %xmm0
# WITHENCODINGS-NEXT:  1      1     1.00           *             6     c4 01 78 11 0c 8b             vmovups	%xmm9, (%r11,%r9,4)
# WITHENCODINGS-NEXT:  1      1     0.50                         3     44 39 c6                      cmpl	%r8d, %esi
# WITHENCODINGS-NEXT:  1      1     0.50                         6     0f 8c 00 00 00 00             jl	-90
