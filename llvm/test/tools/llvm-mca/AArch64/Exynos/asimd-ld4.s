# NOTE: Assertions have been autogenerated by utils/update_mca_test_checks.py
# RUN: llvm-mca -mtriple=aarch64-linux-gnu -mcpu=exynos-m3 -resource-pressure=false < %s | FileCheck %s -check-prefixes=ALL,M3
# RUN: llvm-mca -mtriple=aarch64-linux-gnu -mcpu=exynos-m4 -resource-pressure=false < %s | FileCheck %s -check-prefixes=ALL,M4
# RUN: llvm-mca -mtriple=aarch64-linux-gnu -mcpu=exynos-m5 -resource-pressure=false < %s | FileCheck %s -check-prefixes=ALL,M5

ld4	{v0.s, v1.s, v2.s, v3.s}[0], [sp]
ld4r	{v0.2s, v1.2s, v2.2s, v3.2s}, [sp]
ld4	{v0.2s, v1.2s, v2.2s, v3.2s}, [sp]

ld4	{v0.d, v1.d, v2.d, v3.d}[0], [sp]
ld4r	{v0.2d, v1.2d, v2.2d, v3.2d}, [sp]
ld4	{v0.2d, v1.2d, v2.2d, v3.2d}, [sp]

ld4	{v0.s, v1.s, v2.s, v3.s}[0], [sp], #16
ld4r	{v0.2s, v1.2s, v2.2s, v3.2s}, [sp], #16
ld4	{v0.2s, v1.2s, v2.2s, v3.2s}, [sp], #32

ld4	{v0.d, v1.d, v2.d, v3.d}[0], [sp], #32
ld4r	{v0.2d, v1.2d, v2.2d, v3.2d}, [sp], #32
ld4	{v0.2d, v1.2d, v2.2d, v3.2d}, [sp], #64

ld4	{v0.s, v1.s, v2.s, v3.s}[0], [sp], x0
ld4r	{v0.2s, v1.2s, v2.2s, v3.2s}, [sp], x0
ld4	{v0.2s, v1.2s, v2.2s, v3.2s}, [sp], x0

ld4	{v0.d, v1.d, v2.d, v3.d}[0], [sp], x0
ld4r	{v0.2d, v1.2d, v2.2d, v3.2d}, [sp], x0
ld4	{v0.2d, v1.2d, v2.2d, v3.2d}, [sp], x0

# ALL:      Iterations:        100
# ALL-NEXT: Instructions:      1800

# M3-NEXT:  Total Cycles:      15598
# M4-NEXT:  Total Cycles:      13004
# M5-NEXT:  Total Cycles:      14304

# ALL-NEXT: Total uOps:        9300

# ALL:      Dispatch Width:    6

# M3-NEXT:  uOps Per Cycle:    0.60
# M3-NEXT:  IPC:               0.12
# M3-NEXT:  Block RThroughput: 108.0

# M4-NEXT:  uOps Per Cycle:    0.72
# M4-NEXT:  IPC:               0.14
# M4-NEXT:  Block RThroughput: 61.5

# M5-NEXT:  uOps Per Cycle:    0.65
# M5-NEXT:  IPC:               0.13
# M5-NEXT:  Block RThroughput: 40.5

# ALL:      Instruction Info:
# ALL-NEXT: [1]: #uOps
# ALL-NEXT: [2]: Latency
# ALL-NEXT: [3]: RThroughput
# ALL-NEXT: [4]: MayLoad
# ALL-NEXT: [5]: MayStore
# ALL-NEXT: [6]: HasSideEffects (U)

# ALL:      [1]    [2]    [3]    [4]    [5]    [6]    Instructions:

# M3-NEXT:   5      9     2.00    *                   ld4	{ v0.s, v1.s, v2.s, v3.s }[0], [sp]
# M3-NEXT:   4      6     2.00    *                   ld4r	{ v0.2s, v1.2s, v2.2s, v3.2s }, [sp]
# M3-NEXT:   4      14    12.00   *                   ld4	{ v0.2s, v1.2s, v2.2s, v3.2s }, [sp]
# M3-NEXT:   6      7     6.00    *                   ld4	{ v0.d, v1.d, v2.d, v3.d }[0], [sp]
# M3-NEXT:   4      6     2.00    *                   ld4r	{ v0.2d, v1.2d, v2.2d, v3.2d }, [sp]
# M3-NEXT:   4      14    12.00   *                   ld4	{ v0.2d, v1.2d, v2.2d, v3.2d }, [sp]
# M3-NEXT:   6      9     2.00    *                   ld4	{ v0.s, v1.s, v2.s, v3.s }[0], [sp], #16
# M3-NEXT:   5      6     2.00    *                   ld4r	{ v0.2s, v1.2s, v2.2s, v3.2s }, [sp], #16
# M3-NEXT:   5      14    12.00   *                   ld4	{ v0.2s, v1.2s, v2.2s, v3.2s }, [sp], #32
# M3-NEXT:   7      7     6.00    *                   ld4	{ v0.d, v1.d, v2.d, v3.d }[0], [sp], #32
# M3-NEXT:   5      6     2.00    *                   ld4r	{ v0.2d, v1.2d, v2.2d, v3.2d }, [sp], #32
# M3-NEXT:   5      14    12.00   *                   ld4	{ v0.2d, v1.2d, v2.2d, v3.2d }, [sp], #64
# M3-NEXT:   6      9     2.00    *                   ld4	{ v0.s, v1.s, v2.s, v3.s }[0], [sp], x0
# M3-NEXT:   5      6     2.00    *                   ld4r	{ v0.2s, v1.2s, v2.2s, v3.2s }, [sp], x0
# M3-NEXT:   5      14    12.00   *                   ld4	{ v0.2s, v1.2s, v2.2s, v3.2s }, [sp], x0
# M3-NEXT:   7      7     6.00    *                   ld4	{ v0.d, v1.d, v2.d, v3.d }[0], [sp], x0
# M3-NEXT:   5      6     2.00    *                   ld4r	{ v0.2d, v1.2d, v2.2d, v3.2d }, [sp], x0
# M3-NEXT:   5      14    12.00   *                   ld4	{ v0.2d, v1.2d, v2.2d, v3.2d }, [sp], x0

# M4-NEXT:   5      7     2.00    *                   ld4	{ v0.s, v1.s, v2.s, v3.s }[0], [sp]
# M4-NEXT:   4      6     2.00    *                   ld4r	{ v0.2s, v1.2s, v2.2s, v3.2s }, [sp]
# M4-NEXT:   4      14    6.00    *                   ld4	{ v0.2s, v1.2s, v2.2s, v3.2s }, [sp]
# M4-NEXT:   6      7     3.00    *                   ld4	{ v0.d, v1.d, v2.d, v3.d }[0], [sp]
# M4-NEXT:   4      6     2.00    *                   ld4r	{ v0.2d, v1.2d, v2.2d, v3.2d }, [sp]
# M4-NEXT:   4      14    6.00    *                   ld4	{ v0.2d, v1.2d, v2.2d, v3.2d }, [sp]
# M4-NEXT:   6      7     2.00    *                   ld4	{ v0.s, v1.s, v2.s, v3.s }[0], [sp], #16
# M4-NEXT:   5      6     2.00    *                   ld4r	{ v0.2s, v1.2s, v2.2s, v3.2s }, [sp], #16
# M4-NEXT:   5      14    6.00    *                   ld4	{ v0.2s, v1.2s, v2.2s, v3.2s }, [sp], #32
# M4-NEXT:   7      7     3.00    *                   ld4	{ v0.d, v1.d, v2.d, v3.d }[0], [sp], #32
# M4-NEXT:   5      6     2.00    *                   ld4r	{ v0.2d, v1.2d, v2.2d, v3.2d }, [sp], #32
# M4-NEXT:   5      14    6.00    *                   ld4	{ v0.2d, v1.2d, v2.2d, v3.2d }, [sp], #64
# M4-NEXT:   6      7     2.00    *                   ld4	{ v0.s, v1.s, v2.s, v3.s }[0], [sp], x0
# M4-NEXT:   5      6     2.00    *                   ld4r	{ v0.2s, v1.2s, v2.2s, v3.2s }, [sp], x0
# M4-NEXT:   5      14    6.00    *                   ld4	{ v0.2s, v1.2s, v2.2s, v3.2s }, [sp], x0
# M4-NEXT:   7      7     3.00    *                   ld4	{ v0.d, v1.d, v2.d, v3.d }[0], [sp], x0
# M4-NEXT:   5      6     2.00    *                   ld4r	{ v0.2d, v1.2d, v2.2d, v3.2d }, [sp], x0
# M4-NEXT:   5      14    6.00    *                   ld4	{ v0.2d, v1.2d, v2.2d, v3.2d }, [sp], x0

# M5-NEXT:   5      8     2.00    *                   ld4	{ v0.s, v1.s, v2.s, v3.s }[0], [sp]
# M5-NEXT:   4      7     2.00    *                   ld4r	{ v0.2s, v1.2s, v2.2s, v3.2s }, [sp]
# M5-NEXT:   4      15    4.00    *                   ld4	{ v0.2s, v1.2s, v2.2s, v3.2s }, [sp]
# M5-NEXT:   6      8     2.00    *                   ld4	{ v0.d, v1.d, v2.d, v3.d }[0], [sp]
# M5-NEXT:   4      7     2.00    *                   ld4r	{ v0.2d, v1.2d, v2.2d, v3.2d }, [sp]
# M5-NEXT:   4      15    4.00    *                   ld4	{ v0.2d, v1.2d, v2.2d, v3.2d }, [sp]
# M5-NEXT:   6      8     2.00    *                   ld4	{ v0.s, v1.s, v2.s, v3.s }[0], [sp], #16
# M5-NEXT:   5      7     2.00    *                   ld4r	{ v0.2s, v1.2s, v2.2s, v3.2s }, [sp], #16
# M5-NEXT:   5      15    4.00    *                   ld4	{ v0.2s, v1.2s, v2.2s, v3.2s }, [sp], #32
# M5-NEXT:   7      8     2.00    *                   ld4	{ v0.d, v1.d, v2.d, v3.d }[0], [sp], #32
# M5-NEXT:   5      7     2.00    *                   ld4r	{ v0.2d, v1.2d, v2.2d, v3.2d }, [sp], #32
# M5-NEXT:   5      15    4.00    *                   ld4	{ v0.2d, v1.2d, v2.2d, v3.2d }, [sp], #64
# M5-NEXT:   6      8     2.00    *                   ld4	{ v0.s, v1.s, v2.s, v3.s }[0], [sp], x0
# M5-NEXT:   5      7     2.00    *                   ld4r	{ v0.2s, v1.2s, v2.2s, v3.2s }, [sp], x0
# M5-NEXT:   5      15    4.00    *                   ld4	{ v0.2s, v1.2s, v2.2s, v3.2s }, [sp], x0
# M5-NEXT:   7      8     2.00    *                   ld4	{ v0.d, v1.d, v2.d, v3.d }[0], [sp], x0
# M5-NEXT:   5      7     2.00    *                   ld4r	{ v0.2d, v1.2d, v2.2d, v3.2d }, [sp], x0
# M5-NEXT:   5      15    4.00    *                   ld4	{ v0.2d, v1.2d, v2.2d, v3.2d }, [sp], x0
