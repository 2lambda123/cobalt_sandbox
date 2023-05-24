; NOTE: Assertions have been autogenerated by utils/update_llc_test_checks.py
; RUN: llc < %s -fast-isel -mtriple=i386-unknown-unknown -mattr=+avx512f,+avx512vbmi,+avx512vl | FileCheck %s --check-prefixes=CHECK,X86
; RUN: llc < %s -fast-isel -mtriple=x86_64-unknown-unknown -mattr=+avx512f,+avx512vbmi,+avx512vl | FileCheck %s --check-prefixes=CHECK,X64

; NOTE: This should use IR equivalent to what is generated by clang/test/CodeGen/avx512vlvbmi-builtins.c

define <2 x i64> @test_mm_mask2_permutex2var_epi8(<2 x i64> %__A, <2 x i64> %__I, i16 zeroext %__U, <2 x i64> %__B) {
; X86-LABEL: test_mm_mask2_permutex2var_epi8:
; X86:       # %bb.0: # %entry
; X86-NEXT:    kmovw {{[0-9]+}}(%esp), %k1
; X86-NEXT:    vpermi2b %xmm2, %xmm0, %xmm1 {%k1}
; X86-NEXT:    vmovdqa %xmm1, %xmm0
; X86-NEXT:    retl
;
; X64-LABEL: test_mm_mask2_permutex2var_epi8:
; X64:       # %bb.0: # %entry
; X64-NEXT:    kmovd %edi, %k1
; X64-NEXT:    vpermi2b %xmm2, %xmm0, %xmm1 {%k1}
; X64-NEXT:    vmovdqa %xmm1, %xmm0
; X64-NEXT:    retq
entry:
  %0 = bitcast <2 x i64> %__A to <16 x i8>
  %1 = bitcast <2 x i64> %__I to <16 x i8>
  %2 = bitcast <2 x i64> %__B to <16 x i8>
  %3 = tail call <16 x i8> @llvm.x86.avx512.vpermi2var.qi.128(<16 x i8> %0, <16 x i8> %1, <16 x i8> %2)
  %4 = bitcast i16 %__U to <16 x i1>
  %5 = select <16 x i1> %4, <16 x i8> %3, <16 x i8> %1
  %6 = bitcast <16 x i8> %5 to <2 x i64>
  ret <2 x i64> %6
}

define <4 x i64> @test_mm256_mask2_permutex2var_epi8(<4 x i64> %__A, <4 x i64> %__I, i32 %__U, <4 x i64> %__B) {
; X86-LABEL: test_mm256_mask2_permutex2var_epi8:
; X86:       # %bb.0: # %entry
; X86-NEXT:    kmovd {{[0-9]+}}(%esp), %k1
; X86-NEXT:    vpermi2b %ymm2, %ymm0, %ymm1 {%k1}
; X86-NEXT:    vmovdqa %ymm1, %ymm0
; X86-NEXT:    retl
;
; X64-LABEL: test_mm256_mask2_permutex2var_epi8:
; X64:       # %bb.0: # %entry
; X64-NEXT:    kmovd %edi, %k1
; X64-NEXT:    vpermi2b %ymm2, %ymm0, %ymm1 {%k1}
; X64-NEXT:    vmovdqa %ymm1, %ymm0
; X64-NEXT:    retq
entry:
  %0 = bitcast <4 x i64> %__A to <32 x i8>
  %1 = bitcast <4 x i64> %__I to <32 x i8>
  %2 = bitcast <4 x i64> %__B to <32 x i8>
  %3 = tail call <32 x i8> @llvm.x86.avx512.vpermi2var.qi.256(<32 x i8> %0, <32 x i8> %1, <32 x i8> %2)
  %4 = bitcast i32 %__U to <32 x i1>
  %5 = select <32 x i1> %4, <32 x i8> %3, <32 x i8> %1
  %6 = bitcast <32 x i8> %5 to <4 x i64>
  ret <4 x i64> %6
}

define <2 x i64> @test_mm_permutex2var_epi8(<2 x i64> %__A, <2 x i64> %__I, <2 x i64> %__B) {
; CHECK-LABEL: test_mm_permutex2var_epi8:
; CHECK:       # %bb.0: # %entry
; CHECK-NEXT:    vpermt2b %xmm2, %xmm1, %xmm0
; CHECK-NEXT:    ret{{[l|q]}}
entry:
  %0 = bitcast <2 x i64> %__A to <16 x i8>
  %1 = bitcast <2 x i64> %__I to <16 x i8>
  %2 = bitcast <2 x i64> %__B to <16 x i8>
  %3 = tail call <16 x i8> @llvm.x86.avx512.vpermi2var.qi.128(<16 x i8> %0, <16 x i8> %1, <16 x i8> %2)
  %4 = bitcast <16 x i8> %3 to <2 x i64>
  ret <2 x i64> %4
}

define <2 x i64> @test_mm_mask_permutex2var_epi8(<2 x i64> %__A, i16 zeroext %__U, <2 x i64> %__I, <2 x i64> %__B) {
; X86-LABEL: test_mm_mask_permutex2var_epi8:
; X86:       # %bb.0: # %entry
; X86-NEXT:    kmovw {{[0-9]+}}(%esp), %k1
; X86-NEXT:    vpermt2b %xmm2, %xmm1, %xmm0 {%k1}
; X86-NEXT:    retl
;
; X64-LABEL: test_mm_mask_permutex2var_epi8:
; X64:       # %bb.0: # %entry
; X64-NEXT:    kmovd %edi, %k1
; X64-NEXT:    vpermt2b %xmm2, %xmm1, %xmm0 {%k1}
; X64-NEXT:    retq
entry:
  %0 = bitcast <2 x i64> %__A to <16 x i8>
  %1 = bitcast <2 x i64> %__I to <16 x i8>
  %2 = bitcast <2 x i64> %__B to <16 x i8>
  %3 = tail call <16 x i8> @llvm.x86.avx512.vpermi2var.qi.128(<16 x i8> %0, <16 x i8> %1, <16 x i8> %2)
  %4 = bitcast i16 %__U to <16 x i1>
  %5 = select <16 x i1> %4, <16 x i8> %3, <16 x i8> %0
  %6 = bitcast <16 x i8> %5 to <2 x i64>
  ret <2 x i64> %6
}

define <2 x i64> @test_mm_maskz_permutex2var_epi8(i16 zeroext %__U, <2 x i64> %__A, <2 x i64> %__I, <2 x i64> %__B) {
; X86-LABEL: test_mm_maskz_permutex2var_epi8:
; X86:       # %bb.0: # %entry
; X86-NEXT:    kmovw {{[0-9]+}}(%esp), %k1
; X86-NEXT:    vpermt2b %xmm2, %xmm1, %xmm0 {%k1} {z}
; X86-NEXT:    retl
;
; X64-LABEL: test_mm_maskz_permutex2var_epi8:
; X64:       # %bb.0: # %entry
; X64-NEXT:    kmovd %edi, %k1
; X64-NEXT:    vpermt2b %xmm2, %xmm1, %xmm0 {%k1} {z}
; X64-NEXT:    retq
entry:
  %0 = bitcast <2 x i64> %__A to <16 x i8>
  %1 = bitcast <2 x i64> %__I to <16 x i8>
  %2 = bitcast <2 x i64> %__B to <16 x i8>
  %3 = tail call <16 x i8> @llvm.x86.avx512.vpermi2var.qi.128(<16 x i8> %0, <16 x i8> %1, <16 x i8> %2)
  %4 = bitcast i16 %__U to <16 x i1>
  %5 = select <16 x i1> %4, <16 x i8> %3, <16 x i8> zeroinitializer
  %6 = bitcast <16 x i8> %5 to <2 x i64>
  ret <2 x i64> %6
}

define <4 x i64> @test_mm256_permutex2var_epi8(<4 x i64> %__A, <4 x i64> %__I, <4 x i64> %__B) {
; CHECK-LABEL: test_mm256_permutex2var_epi8:
; CHECK:       # %bb.0: # %entry
; CHECK-NEXT:    vpermt2b %ymm2, %ymm1, %ymm0
; CHECK-NEXT:    ret{{[l|q]}}
entry:
  %0 = bitcast <4 x i64> %__A to <32 x i8>
  %1 = bitcast <4 x i64> %__I to <32 x i8>
  %2 = bitcast <4 x i64> %__B to <32 x i8>
  %3 = tail call <32 x i8> @llvm.x86.avx512.vpermi2var.qi.256(<32 x i8> %0, <32 x i8> %1, <32 x i8> %2)
  %4 = bitcast <32 x i8> %3 to <4 x i64>
  ret <4 x i64> %4
}

define <4 x i64> @test_mm256_mask_permutex2var_epi8(<4 x i64> %__A, i32 %__U, <4 x i64> %__I, <4 x i64> %__B) {
; X86-LABEL: test_mm256_mask_permutex2var_epi8:
; X86:       # %bb.0: # %entry
; X86-NEXT:    kmovd {{[0-9]+}}(%esp), %k1
; X86-NEXT:    vpermt2b %ymm2, %ymm1, %ymm0 {%k1}
; X86-NEXT:    retl
;
; X64-LABEL: test_mm256_mask_permutex2var_epi8:
; X64:       # %bb.0: # %entry
; X64-NEXT:    kmovd %edi, %k1
; X64-NEXT:    vpermt2b %ymm2, %ymm1, %ymm0 {%k1}
; X64-NEXT:    retq
entry:
  %0 = bitcast <4 x i64> %__A to <32 x i8>
  %1 = bitcast <4 x i64> %__I to <32 x i8>
  %2 = bitcast <4 x i64> %__B to <32 x i8>
  %3 = tail call <32 x i8> @llvm.x86.avx512.vpermi2var.qi.256(<32 x i8> %0, <32 x i8> %1, <32 x i8> %2)
  %4 = bitcast i32 %__U to <32 x i1>
  %5 = select <32 x i1> %4, <32 x i8> %3, <32 x i8> %0
  %6 = bitcast <32 x i8> %5 to <4 x i64>
  ret <4 x i64> %6
}

define <4 x i64> @test_mm256_maskz_permutex2var_epi8(i32 %__U, <4 x i64> %__A, <4 x i64> %__I, <4 x i64> %__B) {
; X86-LABEL: test_mm256_maskz_permutex2var_epi8:
; X86:       # %bb.0: # %entry
; X86-NEXT:    kmovd {{[0-9]+}}(%esp), %k1
; X86-NEXT:    vpermt2b %ymm2, %ymm1, %ymm0 {%k1} {z}
; X86-NEXT:    retl
;
; X64-LABEL: test_mm256_maskz_permutex2var_epi8:
; X64:       # %bb.0: # %entry
; X64-NEXT:    kmovd %edi, %k1
; X64-NEXT:    vpermt2b %ymm2, %ymm1, %ymm0 {%k1} {z}
; X64-NEXT:    retq
entry:
  %0 = bitcast <4 x i64> %__A to <32 x i8>
  %1 = bitcast <4 x i64> %__I to <32 x i8>
  %2 = bitcast <4 x i64> %__B to <32 x i8>
  %3 = tail call <32 x i8> @llvm.x86.avx512.vpermi2var.qi.256(<32 x i8> %0, <32 x i8> %1, <32 x i8> %2)
  %4 = bitcast i32 %__U to <32 x i1>
  %5 = select <32 x i1> %4, <32 x i8> %3, <32 x i8> zeroinitializer
  %6 = bitcast <32 x i8> %5 to <4 x i64>
  ret <4 x i64> %6
}

define <2 x i64> @test_mm_mask_multishift_epi64_epi8(<2 x i64> %__W, i16 zeroext %__M, <2 x i64> %__X, <2 x i64> %__Y) {
; X86-LABEL: test_mm_mask_multishift_epi64_epi8:
; X86:       # %bb.0: # %entry
; X86-NEXT:    kmovw {{[0-9]+}}(%esp), %k1
; X86-NEXT:    vpmultishiftqb %xmm2, %xmm1, %xmm0 {%k1}
; X86-NEXT:    retl
;
; X64-LABEL: test_mm_mask_multishift_epi64_epi8:
; X64:       # %bb.0: # %entry
; X64-NEXT:    kmovd %edi, %k1
; X64-NEXT:    vpmultishiftqb %xmm2, %xmm1, %xmm0 {%k1}
; X64-NEXT:    retq
entry:
  %0 = bitcast <2 x i64> %__X to <16 x i8>
  %1 = bitcast <2 x i64> %__Y to <16 x i8>
  %2 = tail call <16 x i8> @llvm.x86.avx512.pmultishift.qb.128(<16 x i8> %0, <16 x i8> %1)
  %3 = bitcast <2 x i64> %__W to <16 x i8>
  %4 = bitcast i16 %__M to <16 x i1>
  %5 = select <16 x i1> %4, <16 x i8> %2, <16 x i8> %3
  %6 = bitcast <16 x i8> %5 to <2 x i64>
  ret <2 x i64> %6
}

define <2 x i64> @test_mm_maskz_multishift_epi64_epi8(i16 zeroext %__M, <2 x i64> %__X, <2 x i64> %__Y) {
; X86-LABEL: test_mm_maskz_multishift_epi64_epi8:
; X86:       # %bb.0: # %entry
; X86-NEXT:    kmovw {{[0-9]+}}(%esp), %k1
; X86-NEXT:    vpmultishiftqb %xmm1, %xmm0, %xmm0 {%k1} {z}
; X86-NEXT:    retl
;
; X64-LABEL: test_mm_maskz_multishift_epi64_epi8:
; X64:       # %bb.0: # %entry
; X64-NEXT:    kmovd %edi, %k1
; X64-NEXT:    vpmultishiftqb %xmm1, %xmm0, %xmm0 {%k1} {z}
; X64-NEXT:    retq
entry:
  %0 = bitcast <2 x i64> %__X to <16 x i8>
  %1 = bitcast <2 x i64> %__Y to <16 x i8>
  %2 = tail call <16 x i8> @llvm.x86.avx512.pmultishift.qb.128(<16 x i8> %0, <16 x i8> %1)
  %3 = bitcast i16 %__M to <16 x i1>
  %4 = select <16 x i1> %3, <16 x i8> %2, <16 x i8> zeroinitializer
  %5 = bitcast <16 x i8> %4 to <2 x i64>
  ret <2 x i64> %5
}

define <2 x i64> @test_mm_multishift_epi64_epi8(<2 x i64> %__X, <2 x i64> %__Y) {
; CHECK-LABEL: test_mm_multishift_epi64_epi8:
; CHECK:       # %bb.0: # %entry
; CHECK-NEXT:    vpmultishiftqb %xmm1, %xmm0, %xmm0
; CHECK-NEXT:    ret{{[l|q]}}
entry:
  %0 = bitcast <2 x i64> %__X to <16 x i8>
  %1 = bitcast <2 x i64> %__Y to <16 x i8>
  %2 = tail call <16 x i8> @llvm.x86.avx512.pmultishift.qb.128(<16 x i8> %0, <16 x i8> %1)
  %3 = bitcast <16 x i8> %2 to <2 x i64>
  ret <2 x i64> %3
}

define <4 x i64> @test_mm256_mask_multishift_epi64_epi8(<4 x i64> %__W, i32 %__M, <4 x i64> %__X, <4 x i64> %__Y) {
; X86-LABEL: test_mm256_mask_multishift_epi64_epi8:
; X86:       # %bb.0: # %entry
; X86-NEXT:    kmovd {{[0-9]+}}(%esp), %k1
; X86-NEXT:    vpmultishiftqb %ymm2, %ymm1, %ymm0 {%k1}
; X86-NEXT:    retl
;
; X64-LABEL: test_mm256_mask_multishift_epi64_epi8:
; X64:       # %bb.0: # %entry
; X64-NEXT:    kmovd %edi, %k1
; X64-NEXT:    vpmultishiftqb %ymm2, %ymm1, %ymm0 {%k1}
; X64-NEXT:    retq
entry:
  %0 = bitcast <4 x i64> %__X to <32 x i8>
  %1 = bitcast <4 x i64> %__Y to <32 x i8>
  %2 = tail call <32 x i8> @llvm.x86.avx512.pmultishift.qb.256(<32 x i8> %0, <32 x i8> %1)
  %3 = bitcast <4 x i64> %__W to <32 x i8>
  %4 = bitcast i32 %__M to <32 x i1>
  %5 = select <32 x i1> %4, <32 x i8> %2, <32 x i8> %3
  %6 = bitcast <32 x i8> %5 to <4 x i64>
  ret <4 x i64> %6
}

define <4 x i64> @test_mm256_maskz_multishift_epi64_epi8(i32 %__M, <4 x i64> %__X, <4 x i64> %__Y) {
; X86-LABEL: test_mm256_maskz_multishift_epi64_epi8:
; X86:       # %bb.0: # %entry
; X86-NEXT:    kmovd {{[0-9]+}}(%esp), %k1
; X86-NEXT:    vpmultishiftqb %ymm1, %ymm0, %ymm0 {%k1} {z}
; X86-NEXT:    retl
;
; X64-LABEL: test_mm256_maskz_multishift_epi64_epi8:
; X64:       # %bb.0: # %entry
; X64-NEXT:    kmovd %edi, %k1
; X64-NEXT:    vpmultishiftqb %ymm1, %ymm0, %ymm0 {%k1} {z}
; X64-NEXT:    retq
entry:
  %0 = bitcast <4 x i64> %__X to <32 x i8>
  %1 = bitcast <4 x i64> %__Y to <32 x i8>
  %2 = tail call <32 x i8> @llvm.x86.avx512.pmultishift.qb.256(<32 x i8> %0, <32 x i8> %1) #3
  %3 = bitcast i32 %__M to <32 x i1>
  %4 = select <32 x i1> %3, <32 x i8> %2, <32 x i8> zeroinitializer
  %5 = bitcast <32 x i8> %4 to <4 x i64>
  ret <4 x i64> %5
}

define <4 x i64> @test_mm256_multishift_epi64_epi8(<4 x i64> %__X, <4 x i64> %__Y) {
; CHECK-LABEL: test_mm256_multishift_epi64_epi8:
; CHECK:       # %bb.0: # %entry
; CHECK-NEXT:    vpmultishiftqb %ymm1, %ymm0, %ymm0
; CHECK-NEXT:    ret{{[l|q]}}
entry:
  %0 = bitcast <4 x i64> %__X to <32 x i8>
  %1 = bitcast <4 x i64> %__Y to <32 x i8>
  %2 = tail call <32 x i8> @llvm.x86.avx512.pmultishift.qb.256(<32 x i8> %0, <32 x i8> %1)
  %3 = bitcast <32 x i8> %2 to <4 x i64>
  ret <4 x i64> %3
}

declare <16 x i8> @llvm.x86.avx512.vpermi2var.qi.128(<16 x i8>, <16 x i8>, <16 x i8>)
declare <32 x i8> @llvm.x86.avx512.vpermi2var.qi.256(<32 x i8>, <32 x i8>, <32 x i8>)
declare <16 x i8> @llvm.x86.avx512.pmultishift.qb.128(<16 x i8>, <16 x i8>)
declare <32 x i8> @llvm.x86.avx512.pmultishift.qb.256(<32 x i8>, <32 x i8>)
