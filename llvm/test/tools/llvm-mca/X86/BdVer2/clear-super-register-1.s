# NOTE: Assertions have been autogenerated by utils/update_mca_test_checks.py
# RUN: llvm-mca -mtriple=x86_64-unknown-unknown -mcpu=bdver2 -iterations=100 -resource-pressure=false -timeline -timeline-max-iterations=2 < %s | FileCheck %s

## Sets register RAX.
imulq $5, %rcx, %rax

## Kills the previous definition of RAX.
## The upper portion of RAX is cleared.
lzcnt %ecx, %eax

## The AND can start immediately after the LZCNT.
## It doesn't need to wait for the IMUL.
and   %rcx, %rax
bsf   %rax, %rcx

# CHECK:      Iterations:        100
# CHECK-NEXT: Instructions:      400
# CHECK-NEXT: Total Cycles:      900
# CHECK-NEXT: Total uOps:        1000

# CHECK:      Dispatch Width:    4
# CHECK-NEXT: uOps Per Cycle:    1.11
# CHECK-NEXT: IPC:               0.44
# CHECK-NEXT: Block RThroughput: 4.0

# CHECK:      Instruction Info:
# CHECK-NEXT: [1]: #uOps
# CHECK-NEXT: [2]: Latency
# CHECK-NEXT: [3]: RThroughput
# CHECK-NEXT: [4]: MayLoad
# CHECK-NEXT: [5]: MayStore
# CHECK-NEXT: [6]: HasSideEffects (U)

# CHECK:      [1]    [2]    [3]    [4]    [5]    [6]    Instructions:
# CHECK-NEXT:  1      6     4.00                        imulq	$5, %rcx, %rax
# CHECK-NEXT:  2      2     2.00                        lzcntl	%ecx, %eax
# CHECK-NEXT:  1      1     1.00                        andq	%rcx, %rax
# CHECK-NEXT:  6      3     3.00                        bsfq	%rax, %rcx

# CHECK:      Timeline view:
# CHECK-NEXT:                     01234567
# CHECK-NEXT: Index     0123456789

# CHECK:      [0,0]     DeeeeeeER .    . .   imulq	$5, %rcx, %rax
# CHECK-NEXT: [0,1]     DeeE----R .    . .   lzcntl	%ecx, %eax
# CHECK-NEXT: [0,2]     D==eE---R .    . .   andq	%rcx, %rax
# CHECK-NEXT: [0,3]     .D==eeeER .    . .   bsfq	%rax, %rcx
# CHECK-NEXT: [1,0]     . D====eeeeeeER. .   imulq	$5, %rcx, %rax
# CHECK-NEXT: [1,1]     .  D======eeE-R. .   lzcntl	%ecx, %eax
# CHECK-NEXT: [1,2]     .  D========eER. .   andq	%rcx, %rax
# CHECK-NEXT: [1,3]     .   D========eeeER   bsfq	%rax, %rcx

# CHECK:      Average Wait times (based on the timeline view):
# CHECK-NEXT: [0]: Executions
# CHECK-NEXT: [1]: Average time spent waiting in a scheduler's queue
# CHECK-NEXT: [2]: Average time spent waiting in a scheduler's queue while ready
# CHECK-NEXT: [3]: Average time elapsed from WB until retire stage

# CHECK:            [0]    [1]    [2]    [3]
# CHECK-NEXT: 0.     2     3.0    0.5    0.0       imulq	$5, %rcx, %rax
# CHECK-NEXT: 1.     2     4.0    2.0    2.5       lzcntl	%ecx, %eax
# CHECK-NEXT: 2.     2     6.0    0.0    1.5       andq	%rcx, %rax
# CHECK-NEXT: 3.     2     6.0    0.0    0.0       bsfq	%rax, %rcx
# CHECK-NEXT:        2     4.8    0.6    1.0       <total>
