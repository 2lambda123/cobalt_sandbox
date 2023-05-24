# NOTE: Assertions have been autogenerated by utils/update_mca_test_checks.py
# RUN: llvm-mca -mtriple=x86_64-unknown-unknown -mcpu=x86-64 -instruction-tables < %s | FileCheck %s

vpopcntd    %xmm1, %xmm0
vpopcntd    (%rdi), %xmm0
vpopcntd    (%rdi){1to4}, %xmm0

vpopcntd    %xmm1, %xmm0 {%k1}
vpopcntd    (%rdi), %xmm0 {%k1}
vpopcntd    (%rdi){1to4}, %xmm0 {%k1}

vpopcntd    %xmm1, %xmm0 {%k1} {z}
vpopcntd    (%rdi), %xmm0 {%k1} {z}
vpopcntd    (%rdi){1to4}, %xmm0 {%k1} {z}

vpopcntd    %ymm1, %ymm0
vpopcntd    (%rdi), %ymm0
vpopcntd    (%rdi){1to8}, %ymm0

vpopcntd    %ymm1, %ymm0 {%k1}
vpopcntd    (%rdi), %ymm0 {%k1}
vpopcntd    (%rdi){1to8}, %ymm0 {%k1}

vpopcntd    %ymm1, %ymm0 {%k1} {z}
vpopcntd    (%rdi), %ymm0 {%k1} {z}
vpopcntd    (%rdi){1to8}, %ymm0 {%k1} {z}

vpopcntq    %xmm1, %xmm0
vpopcntq    (%rdi), %xmm0
vpopcntq    (%rdi){1to2}, %xmm0

vpopcntq    %xmm1, %xmm0 {%k1}
vpopcntq    (%rdi), %xmm0 {%k1}
vpopcntq    (%rdi){1to2}, %xmm0 {%k1}

vpopcntq    %xmm1, %xmm0 {%k1} {z}
vpopcntq    (%rdi), %xmm0 {%k1} {z}
vpopcntq    (%rdi){1to2}, %xmm0 {%k1} {z}

vpopcntq    %ymm1, %ymm0
vpopcntq    (%rdi), %ymm0
vpopcntq    (%rdi){1to4}, %ymm0

vpopcntq    %ymm1, %ymm0 {%k1}
vpopcntq    (%rdi), %ymm0 {%k1}
vpopcntq    (%rdi){1to4}, %ymm0 {%k1}

vpopcntq    %ymm1, %ymm0 {%k1} {z}
vpopcntq    (%rdi), %ymm0 {%k1} {z}
vpopcntq    (%rdi){1to4}, %ymm0 {%k1} {z}

# CHECK:      Instruction Info:
# CHECK-NEXT: [1]: #uOps
# CHECK-NEXT: [2]: Latency
# CHECK-NEXT: [3]: RThroughput
# CHECK-NEXT: [4]: MayLoad
# CHECK-NEXT: [5]: MayStore
# CHECK-NEXT: [6]: HasSideEffects (U)

# CHECK:      [1]    [2]    [3]    [4]    [5]    [6]    Instructions:
# CHECK-NEXT:  1      1     0.50                        vpopcntd	%xmm1, %xmm0
# CHECK-NEXT:  2      7     0.50    *                   vpopcntd	(%rdi), %xmm0
# CHECK-NEXT:  2      7     0.50    *                   vpopcntd	(%rdi){1to4}, %xmm0
# CHECK-NEXT:  1      1     0.50                        vpopcntd	%xmm1, %xmm0 {%k1}
# CHECK-NEXT:  2      7     0.50    *                   vpopcntd	(%rdi), %xmm0 {%k1}
# CHECK-NEXT:  2      7     0.50    *                   vpopcntd	(%rdi){1to4}, %xmm0 {%k1}
# CHECK-NEXT:  1      1     0.50                        vpopcntd	%xmm1, %xmm0 {%k1} {z}
# CHECK-NEXT:  2      7     0.50    *                   vpopcntd	(%rdi), %xmm0 {%k1} {z}
# CHECK-NEXT:  2      7     0.50    *                   vpopcntd	(%rdi){1to4}, %xmm0 {%k1} {z}
# CHECK-NEXT:  1      1     0.50                        vpopcntd	%ymm1, %ymm0
# CHECK-NEXT:  2      8     0.50    *                   vpopcntd	(%rdi), %ymm0
# CHECK-NEXT:  2      8     0.50    *                   vpopcntd	(%rdi){1to8}, %ymm0
# CHECK-NEXT:  1      1     0.50                        vpopcntd	%ymm1, %ymm0 {%k1}
# CHECK-NEXT:  2      8     0.50    *                   vpopcntd	(%rdi), %ymm0 {%k1}
# CHECK-NEXT:  2      8     0.50    *                   vpopcntd	(%rdi){1to8}, %ymm0 {%k1}
# CHECK-NEXT:  1      1     0.50                        vpopcntd	%ymm1, %ymm0 {%k1} {z}
# CHECK-NEXT:  2      8     0.50    *                   vpopcntd	(%rdi), %ymm0 {%k1} {z}
# CHECK-NEXT:  2      8     0.50    *                   vpopcntd	(%rdi){1to8}, %ymm0 {%k1} {z}
# CHECK-NEXT:  1      1     0.50                        vpopcntq	%xmm1, %xmm0
# CHECK-NEXT:  2      7     0.50    *                   vpopcntq	(%rdi), %xmm0
# CHECK-NEXT:  2      7     0.50    *                   vpopcntq	(%rdi){1to2}, %xmm0
# CHECK-NEXT:  1      1     0.50                        vpopcntq	%xmm1, %xmm0 {%k1}
# CHECK-NEXT:  2      7     0.50    *                   vpopcntq	(%rdi), %xmm0 {%k1}
# CHECK-NEXT:  2      7     0.50    *                   vpopcntq	(%rdi){1to2}, %xmm0 {%k1}
# CHECK-NEXT:  1      1     0.50                        vpopcntq	%xmm1, %xmm0 {%k1} {z}
# CHECK-NEXT:  2      7     0.50    *                   vpopcntq	(%rdi), %xmm0 {%k1} {z}
# CHECK-NEXT:  2      7     0.50    *                   vpopcntq	(%rdi){1to2}, %xmm0 {%k1} {z}
# CHECK-NEXT:  1      1     0.50                        vpopcntq	%ymm1, %ymm0
# CHECK-NEXT:  2      8     0.50    *                   vpopcntq	(%rdi), %ymm0
# CHECK-NEXT:  2      8     0.50    *                   vpopcntq	(%rdi){1to4}, %ymm0
# CHECK-NEXT:  1      1     0.50                        vpopcntq	%ymm1, %ymm0 {%k1}
# CHECK-NEXT:  2      8     0.50    *                   vpopcntq	(%rdi), %ymm0 {%k1}
# CHECK-NEXT:  2      8     0.50    *                   vpopcntq	(%rdi){1to4}, %ymm0 {%k1}
# CHECK-NEXT:  1      1     0.50                        vpopcntq	%ymm1, %ymm0 {%k1} {z}
# CHECK-NEXT:  2      8     0.50    *                   vpopcntq	(%rdi), %ymm0 {%k1} {z}
# CHECK-NEXT:  2      8     0.50    *                   vpopcntq	(%rdi){1to4}, %ymm0 {%k1} {z}

# CHECK:      Resources:
# CHECK-NEXT: [0]   - SBDivider
# CHECK-NEXT: [1]   - SBFPDivider
# CHECK-NEXT: [2]   - SBPort0
# CHECK-NEXT: [3]   - SBPort1
# CHECK-NEXT: [4]   - SBPort4
# CHECK-NEXT: [5]   - SBPort5
# CHECK-NEXT: [6.0] - SBPort23
# CHECK-NEXT: [6.1] - SBPort23

# CHECK:      Resource pressure per iteration:
# CHECK-NEXT: [0]    [1]    [2]    [3]    [4]    [5]    [6.0]  [6.1]
# CHECK-NEXT:  -      -      -     18.00   -     18.00  12.00  12.00

# CHECK:      Resource pressure by instruction:
# CHECK-NEXT: [0]    [1]    [2]    [3]    [4]    [5]    [6.0]  [6.1]  Instructions:
# CHECK-NEXT:  -      -      -     0.50    -     0.50    -      -     vpopcntd	%xmm1, %xmm0
# CHECK-NEXT:  -      -      -     0.50    -     0.50   0.50   0.50   vpopcntd	(%rdi), %xmm0
# CHECK-NEXT:  -      -      -     0.50    -     0.50   0.50   0.50   vpopcntd	(%rdi){1to4}, %xmm0
# CHECK-NEXT:  -      -      -     0.50    -     0.50    -      -     vpopcntd	%xmm1, %xmm0 {%k1}
# CHECK-NEXT:  -      -      -     0.50    -     0.50   0.50   0.50   vpopcntd	(%rdi), %xmm0 {%k1}
# CHECK-NEXT:  -      -      -     0.50    -     0.50   0.50   0.50   vpopcntd	(%rdi){1to4}, %xmm0 {%k1}
# CHECK-NEXT:  -      -      -     0.50    -     0.50    -      -     vpopcntd	%xmm1, %xmm0 {%k1} {z}
# CHECK-NEXT:  -      -      -     0.50    -     0.50   0.50   0.50   vpopcntd	(%rdi), %xmm0 {%k1} {z}
# CHECK-NEXT:  -      -      -     0.50    -     0.50   0.50   0.50   vpopcntd	(%rdi){1to4}, %xmm0 {%k1} {z}
# CHECK-NEXT:  -      -      -     0.50    -     0.50    -      -     vpopcntd	%ymm1, %ymm0
# CHECK-NEXT:  -      -      -     0.50    -     0.50   0.50   0.50   vpopcntd	(%rdi), %ymm0
# CHECK-NEXT:  -      -      -     0.50    -     0.50   0.50   0.50   vpopcntd	(%rdi){1to8}, %ymm0
# CHECK-NEXT:  -      -      -     0.50    -     0.50    -      -     vpopcntd	%ymm1, %ymm0 {%k1}
# CHECK-NEXT:  -      -      -     0.50    -     0.50   0.50   0.50   vpopcntd	(%rdi), %ymm0 {%k1}
# CHECK-NEXT:  -      -      -     0.50    -     0.50   0.50   0.50   vpopcntd	(%rdi){1to8}, %ymm0 {%k1}
# CHECK-NEXT:  -      -      -     0.50    -     0.50    -      -     vpopcntd	%ymm1, %ymm0 {%k1} {z}
# CHECK-NEXT:  -      -      -     0.50    -     0.50   0.50   0.50   vpopcntd	(%rdi), %ymm0 {%k1} {z}
# CHECK-NEXT:  -      -      -     0.50    -     0.50   0.50   0.50   vpopcntd	(%rdi){1to8}, %ymm0 {%k1} {z}
# CHECK-NEXT:  -      -      -     0.50    -     0.50    -      -     vpopcntq	%xmm1, %xmm0
# CHECK-NEXT:  -      -      -     0.50    -     0.50   0.50   0.50   vpopcntq	(%rdi), %xmm0
# CHECK-NEXT:  -      -      -     0.50    -     0.50   0.50   0.50   vpopcntq	(%rdi){1to2}, %xmm0
# CHECK-NEXT:  -      -      -     0.50    -     0.50    -      -     vpopcntq	%xmm1, %xmm0 {%k1}
# CHECK-NEXT:  -      -      -     0.50    -     0.50   0.50   0.50   vpopcntq	(%rdi), %xmm0 {%k1}
# CHECK-NEXT:  -      -      -     0.50    -     0.50   0.50   0.50   vpopcntq	(%rdi){1to2}, %xmm0 {%k1}
# CHECK-NEXT:  -      -      -     0.50    -     0.50    -      -     vpopcntq	%xmm1, %xmm0 {%k1} {z}
# CHECK-NEXT:  -      -      -     0.50    -     0.50   0.50   0.50   vpopcntq	(%rdi), %xmm0 {%k1} {z}
# CHECK-NEXT:  -      -      -     0.50    -     0.50   0.50   0.50   vpopcntq	(%rdi){1to2}, %xmm0 {%k1} {z}
# CHECK-NEXT:  -      -      -     0.50    -     0.50    -      -     vpopcntq	%ymm1, %ymm0
# CHECK-NEXT:  -      -      -     0.50    -     0.50   0.50   0.50   vpopcntq	(%rdi), %ymm0
# CHECK-NEXT:  -      -      -     0.50    -     0.50   0.50   0.50   vpopcntq	(%rdi){1to4}, %ymm0
# CHECK-NEXT:  -      -      -     0.50    -     0.50    -      -     vpopcntq	%ymm1, %ymm0 {%k1}
# CHECK-NEXT:  -      -      -     0.50    -     0.50   0.50   0.50   vpopcntq	(%rdi), %ymm0 {%k1}
# CHECK-NEXT:  -      -      -     0.50    -     0.50   0.50   0.50   vpopcntq	(%rdi){1to4}, %ymm0 {%k1}
# CHECK-NEXT:  -      -      -     0.50    -     0.50    -      -     vpopcntq	%ymm1, %ymm0 {%k1} {z}
# CHECK-NEXT:  -      -      -     0.50    -     0.50   0.50   0.50   vpopcntq	(%rdi), %ymm0 {%k1} {z}
# CHECK-NEXT:  -      -      -     0.50    -     0.50   0.50   0.50   vpopcntq	(%rdi){1to4}, %ymm0 {%k1} {z}
