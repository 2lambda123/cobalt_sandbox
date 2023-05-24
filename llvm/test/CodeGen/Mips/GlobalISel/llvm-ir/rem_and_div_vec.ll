; NOTE: Assertions have been autogenerated by utils/update_llc_test_checks.py
; RUN: llc -O0 -mtriple=mipsel-linux-gnu -global-isel -mcpu=mips32r5 -mattr=+msa,+fp64,+nan2008 -verify-machineinstrs %s -o -| FileCheck %s -check-prefixes=P5600

define void @sdiv_v16i8(<16 x i8>* %a, <16 x i8>* %b, <16 x i8>* %c) {
; P5600-LABEL: sdiv_v16i8:
; P5600:       # %bb.0: # %entry
; P5600-NEXT:    ld.b $w0, 0($4)
; P5600-NEXT:    ld.b $w1, 0($5)
; P5600-NEXT:    div_s.b $w0, $w0, $w1
; P5600-NEXT:    st.b $w0, 0($6)
; P5600-NEXT:    jr $ra
; P5600-NEXT:    nop
entry:
  %0 = load <16 x i8>, <16 x i8>* %a, align 16
  %1 = load <16 x i8>, <16 x i8>* %b, align 16
  %div = sdiv <16 x i8> %0, %1
  store <16 x i8> %div, <16 x i8>* %c, align 16
  ret void
}

define void @sdiv_v8i16(<8 x i16>* %a, <8 x i16>* %b, <8 x i16>* %c) {
; P5600-LABEL: sdiv_v8i16:
; P5600:       # %bb.0: # %entry
; P5600-NEXT:    ld.h $w0, 0($4)
; P5600-NEXT:    ld.h $w1, 0($5)
; P5600-NEXT:    div_s.h $w0, $w0, $w1
; P5600-NEXT:    st.h $w0, 0($6)
; P5600-NEXT:    jr $ra
; P5600-NEXT:    nop
entry:
  %0 = load <8 x i16>, <8 x i16>* %a, align 16
  %1 = load <8 x i16>, <8 x i16>* %b, align 16
  %div = sdiv <8 x i16> %0, %1
  store <8 x i16> %div, <8 x i16>* %c, align 16
  ret void
}

define void @sdiv_v4i32(<4 x i32>* %a, <4 x i32>* %b, <4 x i32>* %c) {
; P5600-LABEL: sdiv_v4i32:
; P5600:       # %bb.0: # %entry
; P5600-NEXT:    ld.w $w0, 0($4)
; P5600-NEXT:    ld.w $w1, 0($5)
; P5600-NEXT:    div_s.w $w0, $w0, $w1
; P5600-NEXT:    st.w $w0, 0($6)
; P5600-NEXT:    jr $ra
; P5600-NEXT:    nop
entry:
  %0 = load <4 x i32>, <4 x i32>* %a, align 16
  %1 = load <4 x i32>, <4 x i32>* %b, align 16
  %div = sdiv <4 x i32> %0, %1
  store <4 x i32> %div, <4 x i32>* %c, align 16
  ret void
}

define void @sdiv_v2i64(<2 x i64>* %a, <2 x i64>* %b, <2 x i64>* %c) {
; P5600-LABEL: sdiv_v2i64:
; P5600:       # %bb.0: # %entry
; P5600-NEXT:    ld.d $w0, 0($4)
; P5600-NEXT:    ld.d $w1, 0($5)
; P5600-NEXT:    div_s.d $w0, $w0, $w1
; P5600-NEXT:    st.d $w0, 0($6)
; P5600-NEXT:    jr $ra
; P5600-NEXT:    nop
entry:
  %0 = load <2 x i64>, <2 x i64>* %a, align 16
  %1 = load <2 x i64>, <2 x i64>* %b, align 16
  %div = sdiv <2 x i64> %0, %1
  store <2 x i64> %div, <2 x i64>* %c, align 16
  ret void
}

define void @srem_v16i8(<16 x i8>* %a, <16 x i8>* %b, <16 x i8>* %c) {
; P5600-LABEL: srem_v16i8:
; P5600:       # %bb.0: # %entry
; P5600-NEXT:    ld.b $w0, 0($4)
; P5600-NEXT:    ld.b $w1, 0($5)
; P5600-NEXT:    mod_s.b $w0, $w0, $w1
; P5600-NEXT:    st.b $w0, 0($6)
; P5600-NEXT:    jr $ra
; P5600-NEXT:    nop
entry:
  %0 = load <16 x i8>, <16 x i8>* %a, align 16
  %1 = load <16 x i8>, <16 x i8>* %b, align 16
  %rem = srem <16 x i8> %0, %1
  store <16 x i8> %rem, <16 x i8>* %c, align 16
  ret void
}

define void @srem_v8i16(<8 x i16>* %a, <8 x i16>* %b, <8 x i16>* %c) {
; P5600-LABEL: srem_v8i16:
; P5600:       # %bb.0: # %entry
; P5600-NEXT:    ld.h $w0, 0($4)
; P5600-NEXT:    ld.h $w1, 0($5)
; P5600-NEXT:    mod_s.h $w0, $w0, $w1
; P5600-NEXT:    st.h $w0, 0($6)
; P5600-NEXT:    jr $ra
; P5600-NEXT:    nop
entry:
  %0 = load <8 x i16>, <8 x i16>* %a, align 16
  %1 = load <8 x i16>, <8 x i16>* %b, align 16
  %rem = srem <8 x i16> %0, %1
  store <8 x i16> %rem, <8 x i16>* %c, align 16
  ret void
}

define void @srem_v4i32(<4 x i32>* %a, <4 x i32>* %b, <4 x i32>* %c) {
; P5600-LABEL: srem_v4i32:
; P5600:       # %bb.0: # %entry
; P5600-NEXT:    ld.w $w0, 0($4)
; P5600-NEXT:    ld.w $w1, 0($5)
; P5600-NEXT:    mod_s.w $w0, $w0, $w1
; P5600-NEXT:    st.w $w0, 0($6)
; P5600-NEXT:    jr $ra
; P5600-NEXT:    nop
entry:
  %0 = load <4 x i32>, <4 x i32>* %a, align 16
  %1 = load <4 x i32>, <4 x i32>* %b, align 16
  %rem = srem <4 x i32> %0, %1
  store <4 x i32> %rem, <4 x i32>* %c, align 16
  ret void
}

define void @srem_v2i64(<2 x i64>* %a, <2 x i64>* %b, <2 x i64>* %c) {
; P5600-LABEL: srem_v2i64:
; P5600:       # %bb.0: # %entry
; P5600-NEXT:    ld.d $w0, 0($4)
; P5600-NEXT:    ld.d $w1, 0($5)
; P5600-NEXT:    mod_s.d $w0, $w0, $w1
; P5600-NEXT:    st.d $w0, 0($6)
; P5600-NEXT:    jr $ra
; P5600-NEXT:    nop
entry:
  %0 = load <2 x i64>, <2 x i64>* %a, align 16
  %1 = load <2 x i64>, <2 x i64>* %b, align 16
  %rem = srem <2 x i64> %0, %1
  store <2 x i64> %rem, <2 x i64>* %c, align 16
  ret void
}

define void @udiv_v16u8(<16 x i8>* %a, <16 x i8>* %b, <16 x i8>* %c) {
; P5600-LABEL: udiv_v16u8:
; P5600:       # %bb.0: # %entry
; P5600-NEXT:    ld.b $w0, 0($4)
; P5600-NEXT:    ld.b $w1, 0($5)
; P5600-NEXT:    div_u.b $w0, $w0, $w1
; P5600-NEXT:    st.b $w0, 0($6)
; P5600-NEXT:    jr $ra
; P5600-NEXT:    nop
entry:
  %0 = load <16 x i8>, <16 x i8>* %a, align 16
  %1 = load <16 x i8>, <16 x i8>* %b, align 16
  %div = udiv <16 x i8> %0, %1
  store <16 x i8> %div, <16 x i8>* %c, align 16
  ret void
}

define void @udiv_v8u16(<8 x i16>* %a, <8 x i16>* %b, <8 x i16>* %c) {
; P5600-LABEL: udiv_v8u16:
; P5600:       # %bb.0: # %entry
; P5600-NEXT:    ld.h $w0, 0($4)
; P5600-NEXT:    ld.h $w1, 0($5)
; P5600-NEXT:    div_u.h $w0, $w0, $w1
; P5600-NEXT:    st.h $w0, 0($6)
; P5600-NEXT:    jr $ra
; P5600-NEXT:    nop
entry:
  %0 = load <8 x i16>, <8 x i16>* %a, align 16
  %1 = load <8 x i16>, <8 x i16>* %b, align 16
  %div = udiv <8 x i16> %0, %1
  store <8 x i16> %div, <8 x i16>* %c, align 16
  ret void
}

define void @udiv_v4u32(<4 x i32>* %a, <4 x i32>* %b, <4 x i32>* %c) {
; P5600-LABEL: udiv_v4u32:
; P5600:       # %bb.0: # %entry
; P5600-NEXT:    ld.w $w0, 0($4)
; P5600-NEXT:    ld.w $w1, 0($5)
; P5600-NEXT:    div_u.w $w0, $w0, $w1
; P5600-NEXT:    st.w $w0, 0($6)
; P5600-NEXT:    jr $ra
; P5600-NEXT:    nop
entry:
  %0 = load <4 x i32>, <4 x i32>* %a, align 16
  %1 = load <4 x i32>, <4 x i32>* %b, align 16
  %div = udiv <4 x i32> %0, %1
  store <4 x i32> %div, <4 x i32>* %c, align 16
  ret void
}

define void @udiv_v2u64(<2 x i64>* %a, <2 x i64>* %b, <2 x i64>* %c) {
; P5600-LABEL: udiv_v2u64:
; P5600:       # %bb.0: # %entry
; P5600-NEXT:    ld.d $w0, 0($4)
; P5600-NEXT:    ld.d $w1, 0($5)
; P5600-NEXT:    div_u.d $w0, $w0, $w1
; P5600-NEXT:    st.d $w0, 0($6)
; P5600-NEXT:    jr $ra
; P5600-NEXT:    nop
entry:
  %0 = load <2 x i64>, <2 x i64>* %a, align 16
  %1 = load <2 x i64>, <2 x i64>* %b, align 16
  %div = udiv <2 x i64> %0, %1
  store <2 x i64> %div, <2 x i64>* %c, align 16
  ret void
}

define void @urem_v16u8(<16 x i8>* %a, <16 x i8>* %b, <16 x i8>* %c) {
; P5600-LABEL: urem_v16u8:
; P5600:       # %bb.0: # %entry
; P5600-NEXT:    ld.b $w0, 0($4)
; P5600-NEXT:    ld.b $w1, 0($5)
; P5600-NEXT:    mod_u.b $w0, $w0, $w1
; P5600-NEXT:    st.b $w0, 0($6)
; P5600-NEXT:    jr $ra
; P5600-NEXT:    nop
entry:
  %0 = load <16 x i8>, <16 x i8>* %a, align 16
  %1 = load <16 x i8>, <16 x i8>* %b, align 16
  %rem = urem <16 x i8> %0, %1
  store <16 x i8> %rem, <16 x i8>* %c, align 16
  ret void
}

define void @urem_v8u16(<8 x i16>* %a, <8 x i16>* %b, <8 x i16>* %c) {
; P5600-LABEL: urem_v8u16:
; P5600:       # %bb.0: # %entry
; P5600-NEXT:    ld.h $w0, 0($4)
; P5600-NEXT:    ld.h $w1, 0($5)
; P5600-NEXT:    mod_u.h $w0, $w0, $w1
; P5600-NEXT:    st.h $w0, 0($6)
; P5600-NEXT:    jr $ra
; P5600-NEXT:    nop
entry:
  %0 = load <8 x i16>, <8 x i16>* %a, align 16
  %1 = load <8 x i16>, <8 x i16>* %b, align 16
  %rem = urem <8 x i16> %0, %1
  store <8 x i16> %rem, <8 x i16>* %c, align 16
  ret void
}

define void @urem_v4u32(<4 x i32>* %a, <4 x i32>* %b, <4 x i32>* %c) {
; P5600-LABEL: urem_v4u32:
; P5600:       # %bb.0: # %entry
; P5600-NEXT:    ld.w $w0, 0($4)
; P5600-NEXT:    ld.w $w1, 0($5)
; P5600-NEXT:    mod_u.w $w0, $w0, $w1
; P5600-NEXT:    st.w $w0, 0($6)
; P5600-NEXT:    jr $ra
; P5600-NEXT:    nop
entry:
  %0 = load <4 x i32>, <4 x i32>* %a, align 16
  %1 = load <4 x i32>, <4 x i32>* %b, align 16
  %rem = urem <4 x i32> %0, %1
  store <4 x i32> %rem, <4 x i32>* %c, align 16
  ret void
}

define void @urem_v2u64(<2 x i64>* %a, <2 x i64>* %b, <2 x i64>* %c) {
; P5600-LABEL: urem_v2u64:
; P5600:       # %bb.0: # %entry
; P5600-NEXT:    ld.d $w0, 0($4)
; P5600-NEXT:    ld.d $w1, 0($5)
; P5600-NEXT:    mod_u.d $w0, $w0, $w1
; P5600-NEXT:    st.d $w0, 0($6)
; P5600-NEXT:    jr $ra
; P5600-NEXT:    nop
entry:
  %0 = load <2 x i64>, <2 x i64>* %a, align 16
  %1 = load <2 x i64>, <2 x i64>* %b, align 16
  %rem = urem <2 x i64> %0, %1
  store <2 x i64> %rem, <2 x i64>* %c, align 16
  ret void
}
