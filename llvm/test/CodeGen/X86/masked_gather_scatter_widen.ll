; NOTE: Assertions have been autogenerated by utils/update_llc_test_checks.py
; RUN: llc -mtriple=x86_64-unknown-linux-gnu -mattr=+avx512vl -mattr=+avx512dq < %s | FileCheck %s --check-prefix=CHECK --check-prefix=WIDEN --check-prefix=WIDEN_SKX
; RUN: llc -mtriple=x86_64-unknown-linux-gnu -mattr=+avx512f < %s | FileCheck %s --check-prefix=CHECK --check-prefix=WIDEN --check-prefix=WIDEN_KNL
; RUN: llc -mtriple=x86_64-unknown-linux-gnu -mcpu=skylake < %s | FileCheck %s --check-prefix=WIDEN_AVX2

define <2 x double> @test_gather_v2i32_index(double* %base, <2 x i32> %ind, <2 x i1> %mask, <2 x double> %src0) {
; WIDEN_SKX-LABEL: test_gather_v2i32_index:
; WIDEN_SKX:       # %bb.0:
; WIDEN_SKX-NEXT:    vpsllq $63, %xmm1, %xmm1
; WIDEN_SKX-NEXT:    vpmovq2m %xmm1, %k1
; WIDEN_SKX-NEXT:    vgatherdpd (%rdi,%xmm0,8), %xmm2 {%k1}
; WIDEN_SKX-NEXT:    vmovapd %xmm2, %xmm0
; WIDEN_SKX-NEXT:    retq
;
; WIDEN_KNL-LABEL: test_gather_v2i32_index:
; WIDEN_KNL:       # %bb.0:
; WIDEN_KNL-NEXT:    # kill: def $xmm2 killed $xmm2 def $zmm2
; WIDEN_KNL-NEXT:    # kill: def $xmm0 killed $xmm0 def $ymm0
; WIDEN_KNL-NEXT:    vpsllq $63, %xmm1, %xmm1
; WIDEN_KNL-NEXT:    vptestmq %zmm1, %zmm1, %k0
; WIDEN_KNL-NEXT:    kshiftlw $14, %k0, %k0
; WIDEN_KNL-NEXT:    kshiftrw $14, %k0, %k1
; WIDEN_KNL-NEXT:    vgatherdpd (%rdi,%ymm0,8), %zmm2 {%k1}
; WIDEN_KNL-NEXT:    vmovapd %xmm2, %xmm0
; WIDEN_KNL-NEXT:    vzeroupper
; WIDEN_KNL-NEXT:    retq
;
; WIDEN_AVX2-LABEL: test_gather_v2i32_index:
; WIDEN_AVX2:       # %bb.0:
; WIDEN_AVX2-NEXT:    vpsllq $63, %xmm1, %xmm1
; WIDEN_AVX2-NEXT:    vgatherdpd %xmm1, (%rdi,%xmm0,8), %xmm2
; WIDEN_AVX2-NEXT:    vmovapd %xmm2, %xmm0
; WIDEN_AVX2-NEXT:    retq
  %gep.random = getelementptr double, double* %base, <2 x i32> %ind
  %res = call <2 x double> @llvm.masked.gather.v2f64.v2p0f64(<2 x double*> %gep.random, i32 4, <2 x i1> %mask, <2 x double> %src0)
  ret <2 x double> %res
}

define void @test_scatter_v2i32_index(<2 x double> %a1, double* %base, <2 x i32> %ind, <2 x i1> %mask) {
; WIDEN_SKX-LABEL: test_scatter_v2i32_index:
; WIDEN_SKX:       # %bb.0:
; WIDEN_SKX-NEXT:    vpsllq $63, %xmm2, %xmm2
; WIDEN_SKX-NEXT:    vpmovq2m %xmm2, %k1
; WIDEN_SKX-NEXT:    vscatterdpd %xmm0, (%rdi,%xmm1,8) {%k1}
; WIDEN_SKX-NEXT:    retq
;
; WIDEN_KNL-LABEL: test_scatter_v2i32_index:
; WIDEN_KNL:       # %bb.0:
; WIDEN_KNL-NEXT:    # kill: def $xmm1 killed $xmm1 def $ymm1
; WIDEN_KNL-NEXT:    # kill: def $xmm0 killed $xmm0 def $zmm0
; WIDEN_KNL-NEXT:    vpsllq $63, %xmm2, %xmm2
; WIDEN_KNL-NEXT:    vptestmq %zmm2, %zmm2, %k0
; WIDEN_KNL-NEXT:    kshiftlw $14, %k0, %k0
; WIDEN_KNL-NEXT:    kshiftrw $14, %k0, %k1
; WIDEN_KNL-NEXT:    vscatterdpd %zmm0, (%rdi,%ymm1,8) {%k1}
; WIDEN_KNL-NEXT:    vzeroupper
; WIDEN_KNL-NEXT:    retq
;
; WIDEN_AVX2-LABEL: test_scatter_v2i32_index:
; WIDEN_AVX2:       # %bb.0:
; WIDEN_AVX2-NEXT:    vpmovsxdq %xmm1, %xmm1
; WIDEN_AVX2-NEXT:    vpsllq $3, %xmm1, %xmm1
; WIDEN_AVX2-NEXT:    vmovq %rdi, %xmm3
; WIDEN_AVX2-NEXT:    vpbroadcastq %xmm3, %xmm3
; WIDEN_AVX2-NEXT:    vpaddq %xmm1, %xmm3, %xmm1
; WIDEN_AVX2-NEXT:    vpsllq $63, %xmm2, %xmm2
; WIDEN_AVX2-NEXT:    vmovmskpd %xmm2, %eax
; WIDEN_AVX2-NEXT:    testb $1, %al
; WIDEN_AVX2-NEXT:    jne .LBB1_1
; WIDEN_AVX2-NEXT:  # %bb.2: # %else
; WIDEN_AVX2-NEXT:    testb $2, %al
; WIDEN_AVX2-NEXT:    jne .LBB1_3
; WIDEN_AVX2-NEXT:  .LBB1_4: # %else2
; WIDEN_AVX2-NEXT:    retq
; WIDEN_AVX2-NEXT:  .LBB1_1: # %cond.store
; WIDEN_AVX2-NEXT:    vmovq %xmm1, %rcx
; WIDEN_AVX2-NEXT:    vmovlps %xmm0, (%rcx)
; WIDEN_AVX2-NEXT:    testb $2, %al
; WIDEN_AVX2-NEXT:    je .LBB1_4
; WIDEN_AVX2-NEXT:  .LBB1_3: # %cond.store1
; WIDEN_AVX2-NEXT:    vpextrq $1, %xmm1, %rax
; WIDEN_AVX2-NEXT:    vmovhps %xmm0, (%rax)
; WIDEN_AVX2-NEXT:    retq
  %gep = getelementptr double, double *%base, <2 x i32> %ind
  call void @llvm.masked.scatter.v2f64.v2p0f64(<2 x double> %a1, <2 x double*> %gep, i32 4, <2 x i1> %mask)
  ret void
}

define <2 x i32> @test_gather_v2i32_data(<2 x i32*> %ptr, <2 x i1> %mask, <2 x i32> %src0) {
; WIDEN_SKX-LABEL: test_gather_v2i32_data:
; WIDEN_SKX:       # %bb.0:
; WIDEN_SKX-NEXT:    vpsllq $63, %xmm1, %xmm1
; WIDEN_SKX-NEXT:    vpmovq2m %xmm1, %k1
; WIDEN_SKX-NEXT:    vpgatherqd (,%xmm0), %xmm2 {%k1}
; WIDEN_SKX-NEXT:    vmovdqa %xmm2, %xmm0
; WIDEN_SKX-NEXT:    retq
;
; WIDEN_KNL-LABEL: test_gather_v2i32_data:
; WIDEN_KNL:       # %bb.0:
; WIDEN_KNL-NEXT:    # kill: def $xmm2 killed $xmm2 def $ymm2
; WIDEN_KNL-NEXT:    # kill: def $xmm0 killed $xmm0 def $zmm0
; WIDEN_KNL-NEXT:    vpsllq $63, %xmm1, %xmm1
; WIDEN_KNL-NEXT:    vptestmq %zmm1, %zmm1, %k0
; WIDEN_KNL-NEXT:    kshiftlw $14, %k0, %k0
; WIDEN_KNL-NEXT:    kshiftrw $14, %k0, %k1
; WIDEN_KNL-NEXT:    vpgatherqd (,%zmm0), %ymm2 {%k1}
; WIDEN_KNL-NEXT:    vmovdqa %xmm2, %xmm0
; WIDEN_KNL-NEXT:    vzeroupper
; WIDEN_KNL-NEXT:    retq
;
; WIDEN_AVX2-LABEL: test_gather_v2i32_data:
; WIDEN_AVX2:       # %bb.0:
; WIDEN_AVX2-NEXT:    vpshufd {{.*#+}} xmm1 = xmm1[0,2,2,3]
; WIDEN_AVX2-NEXT:    vpslld $31, %xmm1, %xmm1
; WIDEN_AVX2-NEXT:    vpgatherqd %xmm1, (,%xmm0), %xmm2
; WIDEN_AVX2-NEXT:    vmovdqa %xmm2, %xmm0
; WIDEN_AVX2-NEXT:    retq
  %res = call <2 x i32> @llvm.masked.gather.v2i32.v2p0i32(<2 x i32*> %ptr, i32 4, <2 x i1> %mask, <2 x i32> %src0)
  ret <2 x i32>%res
}

define void @test_scatter_v2i32_data(<2 x i32>%a1, <2 x i32*> %ptr, <2 x i1>%mask) {
; WIDEN_SKX-LABEL: test_scatter_v2i32_data:
; WIDEN_SKX:       # %bb.0:
; WIDEN_SKX-NEXT:    vpsllq $63, %xmm2, %xmm2
; WIDEN_SKX-NEXT:    vpmovq2m %xmm2, %k1
; WIDEN_SKX-NEXT:    vpscatterqd %xmm0, (,%xmm1) {%k1}
; WIDEN_SKX-NEXT:    retq
;
; WIDEN_KNL-LABEL: test_scatter_v2i32_data:
; WIDEN_KNL:       # %bb.0:
; WIDEN_KNL-NEXT:    # kill: def $xmm1 killed $xmm1 def $zmm1
; WIDEN_KNL-NEXT:    # kill: def $xmm0 killed $xmm0 def $ymm0
; WIDEN_KNL-NEXT:    vpsllq $63, %xmm2, %xmm2
; WIDEN_KNL-NEXT:    vptestmq %zmm2, %zmm2, %k0
; WIDEN_KNL-NEXT:    kshiftlw $14, %k0, %k0
; WIDEN_KNL-NEXT:    kshiftrw $14, %k0, %k1
; WIDEN_KNL-NEXT:    vpscatterqd %ymm0, (,%zmm1) {%k1}
; WIDEN_KNL-NEXT:    vzeroupper
; WIDEN_KNL-NEXT:    retq
;
; WIDEN_AVX2-LABEL: test_scatter_v2i32_data:
; WIDEN_AVX2:       # %bb.0:
; WIDEN_AVX2-NEXT:    vpsllq $63, %xmm2, %xmm2
; WIDEN_AVX2-NEXT:    vmovmskpd %xmm2, %eax
; WIDEN_AVX2-NEXT:    testb $1, %al
; WIDEN_AVX2-NEXT:    jne .LBB3_1
; WIDEN_AVX2-NEXT:  # %bb.2: # %else
; WIDEN_AVX2-NEXT:    testb $2, %al
; WIDEN_AVX2-NEXT:    jne .LBB3_3
; WIDEN_AVX2-NEXT:  .LBB3_4: # %else2
; WIDEN_AVX2-NEXT:    retq
; WIDEN_AVX2-NEXT:  .LBB3_1: # %cond.store
; WIDEN_AVX2-NEXT:    vmovq %xmm1, %rcx
; WIDEN_AVX2-NEXT:    vmovss %xmm0, (%rcx)
; WIDEN_AVX2-NEXT:    testb $2, %al
; WIDEN_AVX2-NEXT:    je .LBB3_4
; WIDEN_AVX2-NEXT:  .LBB3_3: # %cond.store1
; WIDEN_AVX2-NEXT:    vpextrq $1, %xmm1, %rax
; WIDEN_AVX2-NEXT:    vextractps $1, %xmm0, (%rax)
; WIDEN_AVX2-NEXT:    retq
  call void @llvm.masked.scatter.v2i32.v2p0i32(<2 x i32> %a1, <2 x i32*> %ptr, i32 4, <2 x i1> %mask)
  ret void
}

define <2 x i32> @test_gather_v2i32_data_index(i32* %base, <2 x i32> %ind, <2 x i1> %mask, <2 x i32> %src0) {
; WIDEN_SKX-LABEL: test_gather_v2i32_data_index:
; WIDEN_SKX:       # %bb.0:
; WIDEN_SKX-NEXT:    vpsllq $63, %xmm1, %xmm1
; WIDEN_SKX-NEXT:    vpmovq2m %xmm1, %k1
; WIDEN_SKX-NEXT:    vpgatherdd (%rdi,%xmm0,4), %xmm2 {%k1}
; WIDEN_SKX-NEXT:    vmovdqa %xmm2, %xmm0
; WIDEN_SKX-NEXT:    retq
;
; WIDEN_KNL-LABEL: test_gather_v2i32_data_index:
; WIDEN_KNL:       # %bb.0:
; WIDEN_KNL-NEXT:    # kill: def $xmm2 killed $xmm2 def $zmm2
; WIDEN_KNL-NEXT:    # kill: def $xmm0 killed $xmm0 def $zmm0
; WIDEN_KNL-NEXT:    vpsllq $63, %xmm1, %xmm1
; WIDEN_KNL-NEXT:    vptestmq %zmm1, %zmm1, %k0
; WIDEN_KNL-NEXT:    kshiftlw $14, %k0, %k0
; WIDEN_KNL-NEXT:    kshiftrw $14, %k0, %k1
; WIDEN_KNL-NEXT:    vpgatherdd (%rdi,%zmm0,4), %zmm2 {%k1}
; WIDEN_KNL-NEXT:    vmovdqa %xmm2, %xmm0
; WIDEN_KNL-NEXT:    vzeroupper
; WIDEN_KNL-NEXT:    retq
;
; WIDEN_AVX2-LABEL: test_gather_v2i32_data_index:
; WIDEN_AVX2:       # %bb.0:
; WIDEN_AVX2-NEXT:    vinsertps {{.*#+}} xmm1 = xmm1[0,2],zero,zero
; WIDEN_AVX2-NEXT:    vpslld $31, %xmm1, %xmm1
; WIDEN_AVX2-NEXT:    vpgatherdd %xmm1, (%rdi,%xmm0,4), %xmm2
; WIDEN_AVX2-NEXT:    vmovdqa %xmm2, %xmm0
; WIDEN_AVX2-NEXT:    retq
  %gep.random = getelementptr i32, i32* %base, <2 x i32> %ind
  %res = call <2 x i32> @llvm.masked.gather.v2i32.v2p0i32(<2 x i32*> %gep.random, i32 4, <2 x i1> %mask, <2 x i32> %src0)
  ret <2 x i32> %res
}

define void @test_scatter_v2i32_data_index(<2 x i32> %a1, i32* %base, <2 x i32> %ind, <2 x i1> %mask) {
; WIDEN_SKX-LABEL: test_scatter_v2i32_data_index:
; WIDEN_SKX:       # %bb.0:
; WIDEN_SKX-NEXT:    vpsllq $63, %xmm2, %xmm2
; WIDEN_SKX-NEXT:    vpmovq2m %xmm2, %k1
; WIDEN_SKX-NEXT:    vpscatterdd %xmm0, (%rdi,%xmm1,4) {%k1}
; WIDEN_SKX-NEXT:    retq
;
; WIDEN_KNL-LABEL: test_scatter_v2i32_data_index:
; WIDEN_KNL:       # %bb.0:
; WIDEN_KNL-NEXT:    # kill: def $xmm1 killed $xmm1 def $zmm1
; WIDEN_KNL-NEXT:    # kill: def $xmm0 killed $xmm0 def $zmm0
; WIDEN_KNL-NEXT:    vpsllq $63, %xmm2, %xmm2
; WIDEN_KNL-NEXT:    vptestmq %zmm2, %zmm2, %k0
; WIDEN_KNL-NEXT:    kshiftlw $14, %k0, %k0
; WIDEN_KNL-NEXT:    kshiftrw $14, %k0, %k1
; WIDEN_KNL-NEXT:    vpscatterdd %zmm0, (%rdi,%zmm1,4) {%k1}
; WIDEN_KNL-NEXT:    vzeroupper
; WIDEN_KNL-NEXT:    retq
;
; WIDEN_AVX2-LABEL: test_scatter_v2i32_data_index:
; WIDEN_AVX2:       # %bb.0:
; WIDEN_AVX2-NEXT:    vpmovsxdq %xmm1, %xmm1
; WIDEN_AVX2-NEXT:    vpsllq $2, %xmm1, %xmm1
; WIDEN_AVX2-NEXT:    vmovq %rdi, %xmm3
; WIDEN_AVX2-NEXT:    vpbroadcastq %xmm3, %xmm3
; WIDEN_AVX2-NEXT:    vpaddq %xmm1, %xmm3, %xmm1
; WIDEN_AVX2-NEXT:    vpsllq $63, %xmm2, %xmm2
; WIDEN_AVX2-NEXT:    vmovmskpd %xmm2, %eax
; WIDEN_AVX2-NEXT:    testb $1, %al
; WIDEN_AVX2-NEXT:    jne .LBB5_1
; WIDEN_AVX2-NEXT:  # %bb.2: # %else
; WIDEN_AVX2-NEXT:    testb $2, %al
; WIDEN_AVX2-NEXT:    jne .LBB5_3
; WIDEN_AVX2-NEXT:  .LBB5_4: # %else2
; WIDEN_AVX2-NEXT:    retq
; WIDEN_AVX2-NEXT:  .LBB5_1: # %cond.store
; WIDEN_AVX2-NEXT:    vmovq %xmm1, %rcx
; WIDEN_AVX2-NEXT:    vmovss %xmm0, (%rcx)
; WIDEN_AVX2-NEXT:    testb $2, %al
; WIDEN_AVX2-NEXT:    je .LBB5_4
; WIDEN_AVX2-NEXT:  .LBB5_3: # %cond.store1
; WIDEN_AVX2-NEXT:    vpextrq $1, %xmm1, %rax
; WIDEN_AVX2-NEXT:    vextractps $1, %xmm0, (%rax)
; WIDEN_AVX2-NEXT:    retq
  %gep = getelementptr i32, i32 *%base, <2 x i32> %ind
  call void @llvm.masked.scatter.v2i32.v2p0i32(<2 x i32> %a1, <2 x i32*> %gep, i32 4, <2 x i1> %mask)
  ret void
}

declare <2 x double> @llvm.masked.gather.v2f64.v2p0f64(<2 x double*>, i32, <2 x i1>, <2 x double>)
declare void @llvm.masked.scatter.v2f64.v2p0f64(<2 x double>, <2 x double*>, i32, <2 x i1>)
declare <2 x i32> @llvm.masked.gather.v2i32.v2p0i32(<2 x i32*>, i32, <2 x i1>, <2 x i32>)
declare void @llvm.masked.scatter.v2i32.v2p0i32(<2 x i32> , <2 x i32*> , i32 , <2 x i1>)
