; NOTE: Assertions have been autogenerated by utils/update_llc_test_checks.py
; RUN: llc -mtriple=x86_64-unknown-linux-gnu -mattr=+sse2 < %s | FileCheck %s --check-prefixes=CHECK,CHECK-SSE,CHECK-SSE2
; RUN: llc -mtriple=x86_64-unknown-linux-gnu -mattr=+sse4.1 < %s | FileCheck %s --check-prefixes=CHECK,CHECK-SSE,CHECK-SSE41
; RUN: llc -mtriple=x86_64-unknown-linux-gnu -mattr=+avx < %s | FileCheck %s --check-prefixes=CHECK,CHECK-AVX,CHECK-AVX1
; RUN: llc -mtriple=x86_64-unknown-linux-gnu -mattr=+avx2 < %s | FileCheck %s --check-prefixes=CHECK,CHECK-AVX,CHECK-AVX2
; RUN: llc -mtriple=x86_64-unknown-linux-gnu -mattr=+avx512f,+avx512vl < %s | FileCheck %s --check-prefixes=CHECK,CHECK-AVX,CHECK-AVX512VL

define <4 x i1> @t32_3(<4 x i32> %X) nounwind {
; CHECK-SSE2-LABEL: t32_3:
; CHECK-SSE2:       # %bb.0:
; CHECK-SSE2-NEXT:    psubd {{.*}}(%rip), %xmm0
; CHECK-SSE2-NEXT:    movdqa {{.*#+}} xmm1 = [2863311531,2863311531,2863311531,2863311531]
; CHECK-SSE2-NEXT:    pshufd {{.*#+}} xmm2 = xmm0[1,1,3,3]
; CHECK-SSE2-NEXT:    pmuludq %xmm1, %xmm0
; CHECK-SSE2-NEXT:    pshufd {{.*#+}} xmm3 = xmm0[0,2,2,3]
; CHECK-SSE2-NEXT:    pmuludq %xmm1, %xmm2
; CHECK-SSE2-NEXT:    pshufd {{.*#+}} xmm0 = xmm2[0,2,2,3]
; CHECK-SSE2-NEXT:    punpckldq {{.*#+}} xmm3 = xmm3[0],xmm0[0],xmm3[1],xmm0[1]
; CHECK-SSE2-NEXT:    pxor {{.*}}(%rip), %xmm3
; CHECK-SSE2-NEXT:    pcmpgtd {{.*}}(%rip), %xmm3
; CHECK-SSE2-NEXT:    pcmpeqd %xmm0, %xmm0
; CHECK-SSE2-NEXT:    pxor %xmm3, %xmm0
; CHECK-SSE2-NEXT:    retq
;
; CHECK-SSE41-LABEL: t32_3:
; CHECK-SSE41:       # %bb.0:
; CHECK-SSE41-NEXT:    psubd {{.*}}(%rip), %xmm0
; CHECK-SSE41-NEXT:    pmulld {{.*}}(%rip), %xmm0
; CHECK-SSE41-NEXT:    movdqa {{.*#+}} xmm1 = [1431655765,1431655764,1431655764,1431655764]
; CHECK-SSE41-NEXT:    pminud %xmm0, %xmm1
; CHECK-SSE41-NEXT:    pcmpeqd %xmm1, %xmm0
; CHECK-SSE41-NEXT:    retq
;
; CHECK-AVX1-LABEL: t32_3:
; CHECK-AVX1:       # %bb.0:
; CHECK-AVX1-NEXT:    vpsubd {{.*}}(%rip), %xmm0, %xmm0
; CHECK-AVX1-NEXT:    vpmulld {{.*}}(%rip), %xmm0, %xmm0
; CHECK-AVX1-NEXT:    vpminud {{.*}}(%rip), %xmm0, %xmm1
; CHECK-AVX1-NEXT:    vpcmpeqd %xmm1, %xmm0, %xmm0
; CHECK-AVX1-NEXT:    retq
;
; CHECK-AVX2-LABEL: t32_3:
; CHECK-AVX2:       # %bb.0:
; CHECK-AVX2-NEXT:    vpsubd {{.*}}(%rip), %xmm0, %xmm0
; CHECK-AVX2-NEXT:    vpbroadcastd {{.*#+}} xmm1 = [2863311531,2863311531,2863311531,2863311531]
; CHECK-AVX2-NEXT:    vpmulld %xmm1, %xmm0, %xmm0
; CHECK-AVX2-NEXT:    vpminud {{.*}}(%rip), %xmm0, %xmm1
; CHECK-AVX2-NEXT:    vpcmpeqd %xmm1, %xmm0, %xmm0
; CHECK-AVX2-NEXT:    retq
;
; CHECK-AVX512VL-LABEL: t32_3:
; CHECK-AVX512VL:       # %bb.0:
; CHECK-AVX512VL-NEXT:    vpsubd {{.*}}(%rip), %xmm0, %xmm0
; CHECK-AVX512VL-NEXT:    vpmulld {{.*}}(%rip){1to4}, %xmm0, %xmm0
; CHECK-AVX512VL-NEXT:    vpminud {{.*}}(%rip), %xmm0, %xmm1
; CHECK-AVX512VL-NEXT:    vpcmpeqd %xmm1, %xmm0, %xmm0
; CHECK-AVX512VL-NEXT:    retq
  %urem = urem <4 x i32> %X, <i32 3, i32 3, i32 3, i32 3>
  %cmp = icmp eq <4 x i32> %urem, <i32 0, i32 1, i32 2, i32 2>
  ret <4 x i1> %cmp
}

define <4 x i1> @t32_5(<4 x i32> %X) nounwind {
; CHECK-SSE2-LABEL: t32_5:
; CHECK-SSE2:       # %bb.0:
; CHECK-SSE2-NEXT:    psubd {{.*}}(%rip), %xmm0
; CHECK-SSE2-NEXT:    movdqa {{.*#+}} xmm1 = [3435973837,3435973837,3435973837,3435973837]
; CHECK-SSE2-NEXT:    pshufd {{.*#+}} xmm2 = xmm0[1,1,3,3]
; CHECK-SSE2-NEXT:    pmuludq %xmm1, %xmm0
; CHECK-SSE2-NEXT:    pshufd {{.*#+}} xmm3 = xmm0[0,2,2,3]
; CHECK-SSE2-NEXT:    pmuludq %xmm1, %xmm2
; CHECK-SSE2-NEXT:    pshufd {{.*#+}} xmm0 = xmm2[0,2,2,3]
; CHECK-SSE2-NEXT:    punpckldq {{.*#+}} xmm3 = xmm3[0],xmm0[0],xmm3[1],xmm0[1]
; CHECK-SSE2-NEXT:    pxor {{.*}}(%rip), %xmm3
; CHECK-SSE2-NEXT:    pcmpgtd {{.*}}(%rip), %xmm3
; CHECK-SSE2-NEXT:    pcmpeqd %xmm0, %xmm0
; CHECK-SSE2-NEXT:    pxor %xmm3, %xmm0
; CHECK-SSE2-NEXT:    retq
;
; CHECK-SSE41-LABEL: t32_5:
; CHECK-SSE41:       # %bb.0:
; CHECK-SSE41-NEXT:    psubd {{.*}}(%rip), %xmm0
; CHECK-SSE41-NEXT:    pmulld {{.*}}(%rip), %xmm0
; CHECK-SSE41-NEXT:    movdqa {{.*#+}} xmm1 = [858993458,858993458,858993458,858993458]
; CHECK-SSE41-NEXT:    pminud %xmm0, %xmm1
; CHECK-SSE41-NEXT:    pcmpeqd %xmm1, %xmm0
; CHECK-SSE41-NEXT:    retq
;
; CHECK-AVX1-LABEL: t32_5:
; CHECK-AVX1:       # %bb.0:
; CHECK-AVX1-NEXT:    vpsubd {{.*}}(%rip), %xmm0, %xmm0
; CHECK-AVX1-NEXT:    vpmulld {{.*}}(%rip), %xmm0, %xmm0
; CHECK-AVX1-NEXT:    vpminud {{.*}}(%rip), %xmm0, %xmm1
; CHECK-AVX1-NEXT:    vpcmpeqd %xmm1, %xmm0, %xmm0
; CHECK-AVX1-NEXT:    retq
;
; CHECK-AVX2-LABEL: t32_5:
; CHECK-AVX2:       # %bb.0:
; CHECK-AVX2-NEXT:    vpsubd {{.*}}(%rip), %xmm0, %xmm0
; CHECK-AVX2-NEXT:    vpbroadcastd {{.*#+}} xmm1 = [3435973837,3435973837,3435973837,3435973837]
; CHECK-AVX2-NEXT:    vpmulld %xmm1, %xmm0, %xmm0
; CHECK-AVX2-NEXT:    vpbroadcastd {{.*#+}} xmm1 = [858993458,858993458,858993458,858993458]
; CHECK-AVX2-NEXT:    vpminud %xmm1, %xmm0, %xmm1
; CHECK-AVX2-NEXT:    vpcmpeqd %xmm1, %xmm0, %xmm0
; CHECK-AVX2-NEXT:    retq
;
; CHECK-AVX512VL-LABEL: t32_5:
; CHECK-AVX512VL:       # %bb.0:
; CHECK-AVX512VL-NEXT:    vpsubd {{.*}}(%rip), %xmm0, %xmm0
; CHECK-AVX512VL-NEXT:    vpmulld {{.*}}(%rip){1to4}, %xmm0, %xmm0
; CHECK-AVX512VL-NEXT:    vpminud {{.*}}(%rip){1to4}, %xmm0, %xmm1
; CHECK-AVX512VL-NEXT:    vpcmpeqd %xmm1, %xmm0, %xmm0
; CHECK-AVX512VL-NEXT:    retq
  %urem = urem <4 x i32> %X, <i32 5, i32 5, i32 5, i32 5>
  %cmp = icmp eq <4 x i32> %urem, <i32 1, i32 2, i32 3, i32 4>
  ret <4 x i1> %cmp
}

define <4 x i1> @t32_6_part0(<4 x i32> %X) nounwind {
; CHECK-SSE2-LABEL: t32_6_part0:
; CHECK-SSE2:       # %bb.0:
; CHECK-SSE2-NEXT:    movdqa {{.*#+}} xmm1 = [2863311531,2863311531,2863311531,2863311531]
; CHECK-SSE2-NEXT:    movdqa %xmm0, %xmm2
; CHECK-SSE2-NEXT:    pmuludq %xmm1, %xmm2
; CHECK-SSE2-NEXT:    pshufd {{.*#+}} xmm2 = xmm2[1,3,2,3]
; CHECK-SSE2-NEXT:    pshufd {{.*#+}} xmm3 = xmm0[1,1,3,3]
; CHECK-SSE2-NEXT:    pmuludq %xmm1, %xmm3
; CHECK-SSE2-NEXT:    pshufd {{.*#+}} xmm1 = xmm3[1,3,2,3]
; CHECK-SSE2-NEXT:    punpckldq {{.*#+}} xmm2 = xmm2[0],xmm1[0],xmm2[1],xmm1[1]
; CHECK-SSE2-NEXT:    psrld $2, %xmm2
; CHECK-SSE2-NEXT:    movdqa {{.*#+}} xmm1 = [6,6,6,6]
; CHECK-SSE2-NEXT:    pshufd {{.*#+}} xmm3 = xmm2[1,1,3,3]
; CHECK-SSE2-NEXT:    pmuludq %xmm1, %xmm2
; CHECK-SSE2-NEXT:    pshufd {{.*#+}} xmm2 = xmm2[0,2,2,3]
; CHECK-SSE2-NEXT:    pmuludq %xmm1, %xmm3
; CHECK-SSE2-NEXT:    pshufd {{.*#+}} xmm1 = xmm3[0,2,2,3]
; CHECK-SSE2-NEXT:    punpckldq {{.*#+}} xmm2 = xmm2[0],xmm1[0],xmm2[1],xmm1[1]
; CHECK-SSE2-NEXT:    psubd %xmm2, %xmm0
; CHECK-SSE2-NEXT:    pcmpeqd {{.*}}(%rip), %xmm0
; CHECK-SSE2-NEXT:    retq
;
; CHECK-SSE41-LABEL: t32_6_part0:
; CHECK-SSE41:       # %bb.0:
; CHECK-SSE41-NEXT:    pshufd {{.*#+}} xmm1 = xmm0[1,1,3,3]
; CHECK-SSE41-NEXT:    movdqa {{.*#+}} xmm2 = [2863311531,2863311531,2863311531,2863311531]
; CHECK-SSE41-NEXT:    pmuludq %xmm2, %xmm1
; CHECK-SSE41-NEXT:    pmuludq %xmm0, %xmm2
; CHECK-SSE41-NEXT:    pshufd {{.*#+}} xmm2 = xmm2[1,1,3,3]
; CHECK-SSE41-NEXT:    pblendw {{.*#+}} xmm2 = xmm2[0,1],xmm1[2,3],xmm2[4,5],xmm1[6,7]
; CHECK-SSE41-NEXT:    psrld $2, %xmm2
; CHECK-SSE41-NEXT:    pmulld {{.*}}(%rip), %xmm2
; CHECK-SSE41-NEXT:    psubd %xmm2, %xmm0
; CHECK-SSE41-NEXT:    pcmpeqd {{.*}}(%rip), %xmm0
; CHECK-SSE41-NEXT:    retq
;
; CHECK-AVX1-LABEL: t32_6_part0:
; CHECK-AVX1:       # %bb.0:
; CHECK-AVX1-NEXT:    vpshufd {{.*#+}} xmm1 = xmm0[1,1,3,3]
; CHECK-AVX1-NEXT:    vmovdqa {{.*#+}} xmm2 = [2863311531,2863311531,2863311531,2863311531]
; CHECK-AVX1-NEXT:    vpmuludq %xmm2, %xmm1, %xmm1
; CHECK-AVX1-NEXT:    vpmuludq %xmm2, %xmm0, %xmm2
; CHECK-AVX1-NEXT:    vpshufd {{.*#+}} xmm2 = xmm2[1,1,3,3]
; CHECK-AVX1-NEXT:    vpblendw {{.*#+}} xmm1 = xmm2[0,1],xmm1[2,3],xmm2[4,5],xmm1[6,7]
; CHECK-AVX1-NEXT:    vpsrld $2, %xmm1, %xmm1
; CHECK-AVX1-NEXT:    vpmulld {{.*}}(%rip), %xmm1, %xmm1
; CHECK-AVX1-NEXT:    vpsubd %xmm1, %xmm0, %xmm0
; CHECK-AVX1-NEXT:    vpcmpeqd {{.*}}(%rip), %xmm0, %xmm0
; CHECK-AVX1-NEXT:    retq
;
; CHECK-AVX2-LABEL: t32_6_part0:
; CHECK-AVX2:       # %bb.0:
; CHECK-AVX2-NEXT:    vpshufd {{.*#+}} xmm1 = xmm0[1,1,3,3]
; CHECK-AVX2-NEXT:    vpbroadcastd {{.*#+}} xmm2 = [2863311531,2863311531,2863311531,2863311531]
; CHECK-AVX2-NEXT:    vpmuludq %xmm2, %xmm1, %xmm1
; CHECK-AVX2-NEXT:    vpmuludq %xmm2, %xmm0, %xmm2
; CHECK-AVX2-NEXT:    vpshufd {{.*#+}} xmm2 = xmm2[1,1,3,3]
; CHECK-AVX2-NEXT:    vpblendd {{.*#+}} xmm1 = xmm2[0],xmm1[1],xmm2[2],xmm1[3]
; CHECK-AVX2-NEXT:    vpsrld $2, %xmm1, %xmm1
; CHECK-AVX2-NEXT:    vpbroadcastd {{.*#+}} xmm2 = [6,6,6,6]
; CHECK-AVX2-NEXT:    vpmulld %xmm2, %xmm1, %xmm1
; CHECK-AVX2-NEXT:    vpsubd %xmm1, %xmm0, %xmm0
; CHECK-AVX2-NEXT:    vpcmpeqd {{.*}}(%rip), %xmm0, %xmm0
; CHECK-AVX2-NEXT:    retq
;
; CHECK-AVX512VL-LABEL: t32_6_part0:
; CHECK-AVX512VL:       # %bb.0:
; CHECK-AVX512VL-NEXT:    vpsubd {{.*}}(%rip), %xmm0, %xmm0
; CHECK-AVX512VL-NEXT:    vpmulld {{.*}}(%rip){1to4}, %xmm0, %xmm0
; CHECK-AVX512VL-NEXT:    vprord $1, %xmm0, %xmm0
; CHECK-AVX512VL-NEXT:    vpminud {{.*}}(%rip){1to4}, %xmm0, %xmm1
; CHECK-AVX512VL-NEXT:    vpcmpeqd %xmm1, %xmm0, %xmm0
; CHECK-AVX512VL-NEXT:    retq
  %urem = urem <4 x i32> %X, <i32 6, i32 6, i32 6, i32 6>
  %cmp = icmp eq <4 x i32> %urem, <i32 0, i32 1, i32 2, i32 3>
  ret <4 x i1> %cmp
}

define <4 x i1> @t32_6_part1(<4 x i32> %X) nounwind {
; CHECK-SSE2-LABEL: t32_6_part1:
; CHECK-SSE2:       # %bb.0:
; CHECK-SSE2-NEXT:    movdqa {{.*#+}} xmm1 = [2863311531,2863311531,2863311531,2863311531]
; CHECK-SSE2-NEXT:    movdqa %xmm0, %xmm2
; CHECK-SSE2-NEXT:    pmuludq %xmm1, %xmm2
; CHECK-SSE2-NEXT:    pshufd {{.*#+}} xmm2 = xmm2[1,3,2,3]
; CHECK-SSE2-NEXT:    pshufd {{.*#+}} xmm3 = xmm0[1,1,3,3]
; CHECK-SSE2-NEXT:    pmuludq %xmm1, %xmm3
; CHECK-SSE2-NEXT:    pshufd {{.*#+}} xmm1 = xmm3[1,3,2,3]
; CHECK-SSE2-NEXT:    punpckldq {{.*#+}} xmm2 = xmm2[0],xmm1[0],xmm2[1],xmm1[1]
; CHECK-SSE2-NEXT:    psrld $2, %xmm2
; CHECK-SSE2-NEXT:    movdqa {{.*#+}} xmm1 = [6,6,6,6]
; CHECK-SSE2-NEXT:    pshufd {{.*#+}} xmm3 = xmm2[1,1,3,3]
; CHECK-SSE2-NEXT:    pmuludq %xmm1, %xmm2
; CHECK-SSE2-NEXT:    pshufd {{.*#+}} xmm2 = xmm2[0,2,2,3]
; CHECK-SSE2-NEXT:    pmuludq %xmm1, %xmm3
; CHECK-SSE2-NEXT:    pshufd {{.*#+}} xmm1 = xmm3[0,2,2,3]
; CHECK-SSE2-NEXT:    punpckldq {{.*#+}} xmm2 = xmm2[0],xmm1[0],xmm2[1],xmm1[1]
; CHECK-SSE2-NEXT:    psubd %xmm2, %xmm0
; CHECK-SSE2-NEXT:    pcmpeqd {{.*}}(%rip), %xmm0
; CHECK-SSE2-NEXT:    retq
;
; CHECK-SSE41-LABEL: t32_6_part1:
; CHECK-SSE41:       # %bb.0:
; CHECK-SSE41-NEXT:    pshufd {{.*#+}} xmm1 = xmm0[1,1,3,3]
; CHECK-SSE41-NEXT:    movdqa {{.*#+}} xmm2 = [2863311531,2863311531,2863311531,2863311531]
; CHECK-SSE41-NEXT:    pmuludq %xmm2, %xmm1
; CHECK-SSE41-NEXT:    pmuludq %xmm0, %xmm2
; CHECK-SSE41-NEXT:    pshufd {{.*#+}} xmm2 = xmm2[1,1,3,3]
; CHECK-SSE41-NEXT:    pblendw {{.*#+}} xmm2 = xmm2[0,1],xmm1[2,3],xmm2[4,5],xmm1[6,7]
; CHECK-SSE41-NEXT:    psrld $2, %xmm2
; CHECK-SSE41-NEXT:    pmulld {{.*}}(%rip), %xmm2
; CHECK-SSE41-NEXT:    psubd %xmm2, %xmm0
; CHECK-SSE41-NEXT:    pcmpeqd {{.*}}(%rip), %xmm0
; CHECK-SSE41-NEXT:    retq
;
; CHECK-AVX1-LABEL: t32_6_part1:
; CHECK-AVX1:       # %bb.0:
; CHECK-AVX1-NEXT:    vpshufd {{.*#+}} xmm1 = xmm0[1,1,3,3]
; CHECK-AVX1-NEXT:    vmovdqa {{.*#+}} xmm2 = [2863311531,2863311531,2863311531,2863311531]
; CHECK-AVX1-NEXT:    vpmuludq %xmm2, %xmm1, %xmm1
; CHECK-AVX1-NEXT:    vpmuludq %xmm2, %xmm0, %xmm2
; CHECK-AVX1-NEXT:    vpshufd {{.*#+}} xmm2 = xmm2[1,1,3,3]
; CHECK-AVX1-NEXT:    vpblendw {{.*#+}} xmm1 = xmm2[0,1],xmm1[2,3],xmm2[4,5],xmm1[6,7]
; CHECK-AVX1-NEXT:    vpsrld $2, %xmm1, %xmm1
; CHECK-AVX1-NEXT:    vpmulld {{.*}}(%rip), %xmm1, %xmm1
; CHECK-AVX1-NEXT:    vpsubd %xmm1, %xmm0, %xmm0
; CHECK-AVX1-NEXT:    vpcmpeqd {{.*}}(%rip), %xmm0, %xmm0
; CHECK-AVX1-NEXT:    retq
;
; CHECK-AVX2-LABEL: t32_6_part1:
; CHECK-AVX2:       # %bb.0:
; CHECK-AVX2-NEXT:    vpshufd {{.*#+}} xmm1 = xmm0[1,1,3,3]
; CHECK-AVX2-NEXT:    vpbroadcastd {{.*#+}} xmm2 = [2863311531,2863311531,2863311531,2863311531]
; CHECK-AVX2-NEXT:    vpmuludq %xmm2, %xmm1, %xmm1
; CHECK-AVX2-NEXT:    vpmuludq %xmm2, %xmm0, %xmm2
; CHECK-AVX2-NEXT:    vpshufd {{.*#+}} xmm2 = xmm2[1,1,3,3]
; CHECK-AVX2-NEXT:    vpblendd {{.*#+}} xmm1 = xmm2[0],xmm1[1],xmm2[2],xmm1[3]
; CHECK-AVX2-NEXT:    vpsrld $2, %xmm1, %xmm1
; CHECK-AVX2-NEXT:    vpbroadcastd {{.*#+}} xmm2 = [6,6,6,6]
; CHECK-AVX2-NEXT:    vpmulld %xmm2, %xmm1, %xmm1
; CHECK-AVX2-NEXT:    vpsubd %xmm1, %xmm0, %xmm0
; CHECK-AVX2-NEXT:    vpcmpeqd {{.*}}(%rip), %xmm0, %xmm0
; CHECK-AVX2-NEXT:    retq
;
; CHECK-AVX512VL-LABEL: t32_6_part1:
; CHECK-AVX512VL:       # %bb.0:
; CHECK-AVX512VL-NEXT:    vpsubd {{.*}}(%rip), %xmm0, %xmm0
; CHECK-AVX512VL-NEXT:    vpmulld {{.*}}(%rip){1to4}, %xmm0, %xmm0
; CHECK-AVX512VL-NEXT:    vprord $1, %xmm0, %xmm0
; CHECK-AVX512VL-NEXT:    vpminud {{.*}}(%rip), %xmm0, %xmm1
; CHECK-AVX512VL-NEXT:    vpcmpeqd %xmm1, %xmm0, %xmm0
; CHECK-AVX512VL-NEXT:    retq
  %urem = urem <4 x i32> %X, <i32 6, i32 6, i32 6, i32 6>
  %cmp = icmp eq <4 x i32> %urem, <i32 4, i32 5, i32 0, i32 0>
  ret <4 x i1> %cmp
}

define <4 x i1> @t32_tautological(<4 x i32> %X) nounwind {
; CHECK-SSE2-LABEL: t32_tautological:
; CHECK-SSE2:       # %bb.0:
; CHECK-SSE2-NEXT:    movdqa {{.*#+}} xmm1 = [0,0,2147483648,2863311531]
; CHECK-SSE2-NEXT:    movdqa %xmm0, %xmm2
; CHECK-SSE2-NEXT:    pmuludq %xmm1, %xmm2
; CHECK-SSE2-NEXT:    pshufd {{.*#+}} xmm2 = xmm2[1,3,2,3]
; CHECK-SSE2-NEXT:    pshufd {{.*#+}} xmm1 = xmm1[1,1,3,3]
; CHECK-SSE2-NEXT:    pshufd {{.*#+}} xmm3 = xmm0[1,1,3,3]
; CHECK-SSE2-NEXT:    pmuludq %xmm1, %xmm3
; CHECK-SSE2-NEXT:    pshufd {{.*#+}} xmm1 = xmm3[1,3,2,3]
; CHECK-SSE2-NEXT:    punpckldq {{.*#+}} xmm2 = xmm2[0],xmm1[0],xmm2[1],xmm1[1]
; CHECK-SSE2-NEXT:    pshufd {{.*#+}} xmm1 = xmm2[0,1,2,2]
; CHECK-SSE2-NEXT:    psrld $1, %xmm2
; CHECK-SSE2-NEXT:    punpckhdq {{.*#+}} xmm1 = xmm1[2],xmm2[2],xmm1[3],xmm2[3]
; CHECK-SSE2-NEXT:    movsd {{.*#+}} xmm1 = xmm0[0],xmm1[1]
; CHECK-SSE2-NEXT:    movdqa {{.*#+}} xmm2 = [1,1,2,3]
; CHECK-SSE2-NEXT:    movapd %xmm1, %xmm3
; CHECK-SSE2-NEXT:    pmuludq %xmm2, %xmm3
; CHECK-SSE2-NEXT:    pshufd {{.*#+}} xmm3 = xmm3[0,2,2,3]
; CHECK-SSE2-NEXT:    shufps {{.*#+}} xmm1 = xmm1[1,1,3,3]
; CHECK-SSE2-NEXT:    pshufd {{.*#+}} xmm2 = xmm2[1,1,3,3]
; CHECK-SSE2-NEXT:    pmuludq %xmm1, %xmm2
; CHECK-SSE2-NEXT:    pshufd {{.*#+}} xmm1 = xmm2[0,2,2,3]
; CHECK-SSE2-NEXT:    punpckldq {{.*#+}} xmm3 = xmm3[0],xmm1[0],xmm3[1],xmm1[1]
; CHECK-SSE2-NEXT:    psubd %xmm3, %xmm0
; CHECK-SSE2-NEXT:    pcmpeqd {{.*}}(%rip), %xmm0
; CHECK-SSE2-NEXT:    retq
;
; CHECK-SSE41-LABEL: t32_tautological:
; CHECK-SSE41:       # %bb.0:
; CHECK-SSE41-NEXT:    movdqa {{.*#+}} xmm1 = [0,0,2147483648,2863311531]
; CHECK-SSE41-NEXT:    pshufd {{.*#+}} xmm2 = xmm1[1,1,3,3]
; CHECK-SSE41-NEXT:    pshufd {{.*#+}} xmm3 = xmm0[1,1,3,3]
; CHECK-SSE41-NEXT:    pmuludq %xmm2, %xmm3
; CHECK-SSE41-NEXT:    pmuludq %xmm0, %xmm1
; CHECK-SSE41-NEXT:    pshufd {{.*#+}} xmm1 = xmm1[1,1,3,3]
; CHECK-SSE41-NEXT:    pblendw {{.*#+}} xmm1 = xmm1[0,1],xmm3[2,3],xmm1[4,5],xmm3[6,7]
; CHECK-SSE41-NEXT:    movdqa %xmm1, %xmm2
; CHECK-SSE41-NEXT:    psrld $1, %xmm2
; CHECK-SSE41-NEXT:    pblendw {{.*#+}} xmm2 = xmm1[0,1,2,3,4,5],xmm2[6,7]
; CHECK-SSE41-NEXT:    pblendw {{.*#+}} xmm2 = xmm0[0,1,2,3],xmm2[4,5,6,7]
; CHECK-SSE41-NEXT:    pmulld {{.*}}(%rip), %xmm2
; CHECK-SSE41-NEXT:    psubd %xmm2, %xmm0
; CHECK-SSE41-NEXT:    pcmpeqd {{.*}}(%rip), %xmm0
; CHECK-SSE41-NEXT:    retq
;
; CHECK-AVX1-LABEL: t32_tautological:
; CHECK-AVX1:       # %bb.0:
; CHECK-AVX1-NEXT:    vmovdqa {{.*#+}} xmm1 = [0,0,2147483648,2863311531]
; CHECK-AVX1-NEXT:    vpshufd {{.*#+}} xmm2 = xmm1[1,1,3,3]
; CHECK-AVX1-NEXT:    vpshufd {{.*#+}} xmm3 = xmm0[1,1,3,3]
; CHECK-AVX1-NEXT:    vpmuludq %xmm2, %xmm3, %xmm2
; CHECK-AVX1-NEXT:    vpmuludq %xmm1, %xmm0, %xmm1
; CHECK-AVX1-NEXT:    vpshufd {{.*#+}} xmm1 = xmm1[1,1,3,3]
; CHECK-AVX1-NEXT:    vpblendw {{.*#+}} xmm1 = xmm1[0,1],xmm2[2,3],xmm1[4,5],xmm2[6,7]
; CHECK-AVX1-NEXT:    vpsrld $1, %xmm1, %xmm2
; CHECK-AVX1-NEXT:    vpblendw {{.*#+}} xmm1 = xmm1[0,1,2,3,4,5],xmm2[6,7]
; CHECK-AVX1-NEXT:    vpblendw {{.*#+}} xmm1 = xmm0[0,1,2,3],xmm1[4,5,6,7]
; CHECK-AVX1-NEXT:    vpmulld {{.*}}(%rip), %xmm1, %xmm1
; CHECK-AVX1-NEXT:    vpsubd %xmm1, %xmm0, %xmm0
; CHECK-AVX1-NEXT:    vpcmpeqd {{.*}}(%rip), %xmm0, %xmm0
; CHECK-AVX1-NEXT:    retq
;
; CHECK-AVX2-LABEL: t32_tautological:
; CHECK-AVX2:       # %bb.0:
; CHECK-AVX2-NEXT:    vmovdqa {{.*#+}} xmm1 = [0,0,2147483648,2863311531]
; CHECK-AVX2-NEXT:    vpshufd {{.*#+}} xmm2 = xmm1[1,1,3,3]
; CHECK-AVX2-NEXT:    vpshufd {{.*#+}} xmm3 = xmm0[1,1,3,3]
; CHECK-AVX2-NEXT:    vpmuludq %xmm2, %xmm3, %xmm2
; CHECK-AVX2-NEXT:    vpmuludq %xmm1, %xmm0, %xmm1
; CHECK-AVX2-NEXT:    vpshufd {{.*#+}} xmm1 = xmm1[1,1,3,3]
; CHECK-AVX2-NEXT:    vpblendd {{.*#+}} xmm1 = xmm1[0],xmm2[1],xmm1[2],xmm2[3]
; CHECK-AVX2-NEXT:    vpsrlvd {{.*}}(%rip), %xmm1, %xmm1
; CHECK-AVX2-NEXT:    vpblendd {{.*#+}} xmm1 = xmm0[0,1],xmm1[2,3]
; CHECK-AVX2-NEXT:    vpmulld {{.*}}(%rip), %xmm1, %xmm1
; CHECK-AVX2-NEXT:    vpsubd %xmm1, %xmm0, %xmm0
; CHECK-AVX2-NEXT:    vpcmpeqd {{.*}}(%rip), %xmm0, %xmm0
; CHECK-AVX2-NEXT:    retq
;
; CHECK-AVX512VL-LABEL: t32_tautological:
; CHECK-AVX512VL:       # %bb.0:
; CHECK-AVX512VL-NEXT:    vpsubd {{.*}}(%rip), %xmm0, %xmm0
; CHECK-AVX512VL-NEXT:    vpmulld {{.*}}(%rip){1to4}, %xmm0, %xmm0
; CHECK-AVX512VL-NEXT:    vpminud {{.*}}(%rip), %xmm0, %xmm1
; CHECK-AVX512VL-NEXT:    vpcmpeqd %xmm1, %xmm0, %xmm0
; CHECK-AVX512VL-NEXT:    vpxor %xmm1, %xmm1, %xmm1
; CHECK-AVX512VL-NEXT:    vpblendd {{.*#+}} xmm0 = xmm0[0],xmm1[1,2],xmm0[3]
; CHECK-AVX512VL-NEXT:    retq
  %urem = urem <4 x i32> %X, <i32 1, i32 1, i32 2, i32 3>
  %cmp = icmp eq <4 x i32> %urem, <i32 0, i32 1, i32 2, i32 2>
  ret <4 x i1> %cmp
}
