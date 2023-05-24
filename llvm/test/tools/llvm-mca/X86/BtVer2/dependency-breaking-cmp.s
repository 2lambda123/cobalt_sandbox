# NOTE: Assertions have been autogenerated by utils/update_mca_test_checks.py
# RUN: llvm-mca -mtriple=x86_64-unknown-unknown -mcpu=btver2 -timeline -timeline-max-iterations=3 -iterations=1500 < %s | FileCheck %s

# Perf stat reports an IPC of 1.97 for this block of code.

# The CMP instruction doesn't depend on the value of EAX.  It can set the flags
# without having to read the inputs.

cmp %eax, %eax
cmovae %ebx, %eax

# CHECK:      Iterations:        1500
# CHECK-NEXT: Instructions:      3000
# CHECK-NEXT: Total Cycles:      1504
# CHECK-NEXT: Total uOps:        3000

# CHECK:      Dispatch Width:    2
# CHECK-NEXT: uOps Per Cycle:    1.99
# CHECK-NEXT: IPC:               1.99
# CHECK-NEXT: Block RThroughput: 1.0

# CHECK:      Instruction Info:
# CHECK-NEXT: [1]: #uOps
# CHECK-NEXT: [2]: Latency
# CHECK-NEXT: [3]: RThroughput
# CHECK-NEXT: [4]: MayLoad
# CHECK-NEXT: [5]: MayStore
# CHECK-NEXT: [6]: HasSideEffects (U)

# CHECK:      [1]    [2]    [3]    [4]    [5]    [6]    Instructions:
# CHECK-NEXT:  1      1     0.50                        cmpl	%eax, %eax
# CHECK-NEXT:  1      1     0.50                        cmovael	%ebx, %eax

# CHECK:      Resources:
# CHECK-NEXT: [0]   - JALU0
# CHECK-NEXT: [1]   - JALU1
# CHECK-NEXT: [2]   - JDiv
# CHECK-NEXT: [3]   - JFPA
# CHECK-NEXT: [4]   - JFPM
# CHECK-NEXT: [5]   - JFPU0
# CHECK-NEXT: [6]   - JFPU1
# CHECK-NEXT: [7]   - JLAGU
# CHECK-NEXT: [8]   - JMul
# CHECK-NEXT: [9]   - JSAGU
# CHECK-NEXT: [10]  - JSTC
# CHECK-NEXT: [11]  - JVALU0
# CHECK-NEXT: [12]  - JVALU1
# CHECK-NEXT: [13]  - JVIMUL

# CHECK:      Resource pressure per iteration:
# CHECK-NEXT: [0]    [1]    [2]    [3]    [4]    [5]    [6]    [7]    [8]    [9]    [10]   [11]   [12]   [13]
# CHECK-NEXT: 1.00   1.00    -      -      -      -      -      -      -      -      -      -      -      -

# CHECK:      Resource pressure by instruction:
# CHECK-NEXT: [0]    [1]    [2]    [3]    [4]    [5]    [6]    [7]    [8]    [9]    [10]   [11]   [12]   [13]   Instructions:
# CHECK-NEXT:  -     1.00    -      -      -      -      -      -      -      -      -      -      -      -     cmpl	%eax, %eax
# CHECK-NEXT: 1.00    -      -      -      -      -      -      -      -      -      -      -      -      -     cmovael	%ebx, %eax

# CHECK:      Timeline view:
# CHECK-NEXT: Index     0123456

# CHECK:      [0,0]     DeER ..   cmpl	%eax, %eax
# CHECK-NEXT: [0,1]     D=eER..   cmovael	%ebx, %eax
# CHECK-NEXT: [1,0]     .DeER..   cmpl	%eax, %eax
# CHECK-NEXT: [1,1]     .D=eER.   cmovael	%ebx, %eax
# CHECK-NEXT: [2,0]     . DeER.   cmpl	%eax, %eax
# CHECK-NEXT: [2,1]     . D=eER   cmovael	%ebx, %eax

# CHECK:      Average Wait times (based on the timeline view):
# CHECK-NEXT: [0]: Executions
# CHECK-NEXT: [1]: Average time spent waiting in a scheduler's queue
# CHECK-NEXT: [2]: Average time spent waiting in a scheduler's queue while ready
# CHECK-NEXT: [3]: Average time elapsed from WB until retire stage

# CHECK:            [0]    [1]    [2]    [3]
# CHECK-NEXT: 0.     3     1.0    1.0    0.0       cmpl	%eax, %eax
# CHECK-NEXT: 1.     3     2.0    0.0    0.0       cmovael	%ebx, %eax
# CHECK-NEXT:        3     1.5    0.5    0.0       <total>
