# NOTE: Assertions have been autogenerated by utils/update_mca_test_checks.py
# RUN: llvm-mca -mtriple=x86_64-unknown-unknown -mcpu=btver2 -bottleneck-analysis < %s | FileCheck %s

vmulps   %xmm0, %xmm1, %xmm2
vhaddps  %xmm2, %xmm2, %xmm3
vhaddps  %xmm3, %xmm3, %xmm4

# CHECK:      Iterations:        100
# CHECK-NEXT: Instructions:      300
# CHECK-NEXT: Total Cycles:      211
# CHECK-NEXT: Total uOps:        300

# CHECK:      Dispatch Width:    2
# CHECK-NEXT: uOps Per Cycle:    1.42
# CHECK-NEXT: IPC:               1.42
# CHECK-NEXT: Block RThroughput: 2.0

# CHECK:      Cycles with backend pressure increase [ 40.76% ]
# CHECK-NEXT: Throughput Bottlenecks:
# CHECK-NEXT:   Resource Pressure       [ 39.34% ]
# CHECK-NEXT:   - JFPA  [ 39.34% ]
# CHECK-NEXT:   - JFPU0  [ 39.34% ]
# CHECK-NEXT:   Data Dependencies:      [ 1.42% ]
# CHECK-NEXT:   - Register Dependencies [ 1.42% ]
# CHECK-NEXT:   - Memory Dependencies   [ 0.00% ]

# CHECK:      Critical sequence based on the simulation:

# CHECK:                    Instruction                                 Dependency Information
# CHECK-NEXT:  +----< 2.    vhaddps	%xmm3, %xmm3, %xmm4
# CHECK-NEXT:  |
# CHECK-NEXT:  |    < loop carried >
# CHECK-NEXT:  |
# CHECK-NEXT:  |      0.    vmulps	%xmm0, %xmm1, %xmm2
# CHECK-NEXT:  +----> 1.    vhaddps	%xmm2, %xmm2, %xmm3               ## RESOURCE interference:  JFPA [ probability: 73% ]
# CHECK-NEXT:  +----> 2.    vhaddps	%xmm3, %xmm3, %xmm4               ## REGISTER dependency:  %xmm3
# CHECK-NEXT:  |
# CHECK-NEXT:  |    < loop carried >
# CHECK-NEXT:  |
# CHECK-NEXT:  +----> 1.    vhaddps	%xmm2, %xmm2, %xmm3               ## RESOURCE interference:  JFPA [ probability: 73% ]

# CHECK:      Instruction Info:
# CHECK-NEXT: [1]: #uOps
# CHECK-NEXT: [2]: Latency
# CHECK-NEXT: [3]: RThroughput
# CHECK-NEXT: [4]: MayLoad
# CHECK-NEXT: [5]: MayStore
# CHECK-NEXT: [6]: HasSideEffects (U)

# CHECK:      [1]    [2]    [3]    [4]    [5]    [6]    Instructions:
# CHECK-NEXT:  1      2     1.00                        vmulps	%xmm0, %xmm1, %xmm2
# CHECK-NEXT:  1      4     1.00                        vhaddps	%xmm2, %xmm2, %xmm3
# CHECK-NEXT:  1      4     1.00                        vhaddps	%xmm3, %xmm3, %xmm4

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
# CHECK-NEXT:  -      -      -     2.00   1.00   2.00   1.00    -      -      -      -      -      -      -

# CHECK:      Resource pressure by instruction:
# CHECK-NEXT: [0]    [1]    [2]    [3]    [4]    [5]    [6]    [7]    [8]    [9]    [10]   [11]   [12]   [13]   Instructions:
# CHECK-NEXT:  -      -      -      -     1.00    -     1.00    -      -      -      -      -      -      -     vmulps	%xmm0, %xmm1, %xmm2
# CHECK-NEXT:  -      -      -     1.00    -     1.00    -      -      -      -      -      -      -      -     vhaddps	%xmm2, %xmm2, %xmm3
# CHECK-NEXT:  -      -      -     1.00    -     1.00    -      -      -      -      -      -      -      -     vhaddps	%xmm3, %xmm3, %xmm4
