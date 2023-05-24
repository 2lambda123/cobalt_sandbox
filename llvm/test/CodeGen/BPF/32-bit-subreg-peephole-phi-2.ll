; RUN: llc -O2 -march=bpfel -mcpu=v2 -mattr=+alu32 < %s | FileCheck %s
;
; For the below test case, both 'ret' and 'b' at 'ret == b'
; need SLL/SLR. For 'ret', 'ret = a' may receive the value
; from argument with high 32-bit invalid data.
;
; extern int helper(int);
; int test(int a, int b, int c, int d) {
;   int ret;
;   if (a < b)
;     ret = (c < d) ? a : 0;
;   else
;     ret = (c < a) ? 1 : 2;
;   return helper(ret == b);
; }

define dso_local i32 @test(i32 %a, i32 %b, i32 %c, i32 %d) local_unnamed_addr {
entry:
  %cmp = icmp slt i32 %a, %b
  %cmp1 = icmp slt i32 %c, %d
  %cond = select i1 %cmp1, i32 %a, i32 0
  %cmp2 = icmp slt i32 %c, %a
  %cond3 = select i1 %cmp2, i32 1, i32 2
  %ret.0 = select i1 %cmp, i32 %cond, i32 %cond3
  %cmp4 = icmp eq i32 %ret.0, %b
  %conv = zext i1 %cmp4 to i32
  %call = tail call i32 @helper(i32 %conv)
  ret i32 %call
}
; CHECK: r{{[0-9]+}} >>= 32
; CHECK: r{{[0-9]+}} >>= 32
; CHECK: if r{{[0-9]+}} == r{{[0-9]+}} goto

declare dso_local i32 @helper(i32) local_unnamed_addr
