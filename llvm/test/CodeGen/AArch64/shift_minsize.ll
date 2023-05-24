; NOTE: Assertions have been autogenerated by utils/update_llc_test_checks.py
; RUN: llc < %s -mtriple=aarch64-unknown-unknown | FileCheck %s
; RUN: llc < %s -mtriple=aarch64-windows         | FileCheck %s -check-prefix=CHECK-WIN
; RUN: llc < %s -mtriple=aarch64-apple-darwin    | FileCheck %s -check-prefix=CHECK-DARWIN

; The Windows runtime doesn't have these.
; CHECK-WIN-NOT: __ashlti3
; CHECK-WIN-NOT: __ashrti3

; Darwin compiler-rt excludes these.
; CHECK-DARWIN-NOT: __ashlti3
; CHECK-DARWIN-NOT: __ashrti3

define i64 @f0(i64 %val, i64 %amt) minsize optsize {
; CHECK-LABEL: f0:
; CHECK:       // %bb.0:
; CHECK-NEXT:    lsl x0, x0, x1
; CHECK-NEXT:    ret
  %res = shl i64 %val, %amt
  ret i64 %res
}

define i32 @f1(i64 %x, i64 %y) minsize optsize {
; CHECK-LABEL: f1:
; CHECK:       // %bb.0:
; CHECK-NEXT:    lsl x0, x0, x1
; CHECK-NEXT:    // kill: def $w0 killed $w0 killed $x0
; CHECK-NEXT:    ret
	%a = shl i64 %x, %y
	%b = trunc i64 %a to i32
	ret i32 %b
}

define i32 @f2(i64 %x, i64 %y) minsize optsize {
; CHECK-LABEL: f2:
; CHECK:       // %bb.0:
; CHECK-NEXT:    asr x0, x0, x1
; CHECK-NEXT:    // kill: def $w0 killed $w0 killed $x0
; CHECK-NEXT:    ret
	%a = ashr i64 %x, %y
	%b = trunc i64 %a to i32
	ret i32 %b
}

define i32 @f3(i64 %x, i64 %y) minsize optsize {
; CHECK-LABEL: f3:
; CHECK:       // %bb.0:
; CHECK-NEXT:    lsr x0, x0, x1
; CHECK-NEXT:    // kill: def $w0 killed $w0 killed $x0
; CHECK-NEXT:    ret
	%a = lshr i64 %x, %y
	%b = trunc i64 %a to i32
	ret i32 %b
}

define dso_local { i64, i64 } @shl128(i64 %x.coerce0, i64 %x.coerce1, i8 signext %y) minsize optsize {
; CHECK-LABEL: shl128:
; CHECK:       // %bb.0: // %entry
; CHECK-NEXT:    str x30, [sp, #-16]! // 8-byte Folded Spill
; CHECK-NEXT:    .cfi_def_cfa_offset 16
; CHECK-NEXT:    .cfi_offset w30, -16
; CHECK-NEXT:    // kill: def $w2 killed $w2 def $x2
; CHECK-NEXT:    bl __ashlti3
; CHECK-NEXT:    ldr x30, [sp], #16 // 8-byte Folded Reload
; CHECK-NEXT:    ret

entry:
  %x.sroa.2.0.insert.ext = zext i64 %x.coerce1 to i128
  %x.sroa.2.0.insert.shift = shl nuw i128 %x.sroa.2.0.insert.ext, 64
  %x.sroa.0.0.insert.ext = zext i64 %x.coerce0 to i128
  %x.sroa.0.0.insert.insert = or i128 %x.sroa.2.0.insert.shift, %x.sroa.0.0.insert.ext
  %conv = sext i8 %y to i32
  %sh_prom = zext i32 %conv to i128
  %shl = shl i128 %x.sroa.0.0.insert.insert, %sh_prom
  %retval.sroa.0.0.extract.trunc = trunc i128 %shl to i64
  %retval.sroa.2.0.extract.shift = lshr i128 %shl, 64
  %retval.sroa.2.0.extract.trunc = trunc i128 %retval.sroa.2.0.extract.shift to i64
  %.fca.0.insert = insertvalue { i64, i64 } undef, i64 %retval.sroa.0.0.extract.trunc, 0
  %.fca.1.insert = insertvalue { i64, i64 } %.fca.0.insert, i64 %retval.sroa.2.0.extract.trunc, 1
  ret { i64, i64 } %.fca.1.insert
}

define dso_local { i64, i64 } @ashr128(i64 %x.coerce0, i64 %x.coerce1, i8 signext %y) minsize optsize {
; CHECK-LABEL: ashr128:
; CHECK:       // %bb.0: // %entry
; CHECK-NEXT:    str x30, [sp, #-16]! // 8-byte Folded Spill
; CHECK-NEXT:    .cfi_def_cfa_offset 16
; CHECK-NEXT:    .cfi_offset w30, -16
; CHECK-NEXT:    // kill: def $w2 killed $w2 def $x2
; CHECK-NEXT:    bl __ashrti3
; CHECK-NEXT:    ldr x30, [sp], #16 // 8-byte Folded Reload
; CHECK-NEXT:    ret
entry:
  %x.sroa.2.0.insert.ext = zext i64 %x.coerce1 to i128
  %x.sroa.2.0.insert.shift = shl nuw i128 %x.sroa.2.0.insert.ext, 64
  %x.sroa.0.0.insert.ext = zext i64 %x.coerce0 to i128
  %x.sroa.0.0.insert.insert = or i128 %x.sroa.2.0.insert.shift, %x.sroa.0.0.insert.ext
  %conv = sext i8 %y to i32
  %sh_prom = zext i32 %conv to i128
  %shr = ashr i128 %x.sroa.0.0.insert.insert, %sh_prom
  %retval.sroa.0.0.extract.trunc = trunc i128 %shr to i64
  %retval.sroa.2.0.extract.shift = lshr i128 %shr, 64
  %retval.sroa.2.0.extract.trunc = trunc i128 %retval.sroa.2.0.extract.shift to i64
  %.fca.0.insert = insertvalue { i64, i64 } undef, i64 %retval.sroa.0.0.extract.trunc, 0
  %.fca.1.insert = insertvalue { i64, i64 } %.fca.0.insert, i64 %retval.sroa.2.0.extract.trunc, 1
  ret { i64, i64 } %.fca.1.insert
}

define dso_local { i64, i64 } @lshr128(i64 %x.coerce0, i64 %x.coerce1, i8 signext %y) minsize optsize {
; CHECK-LABEL: lshr128:
; CHECK:       // %bb.0: // %entry
; CHECK-NEXT:    str x30, [sp, #-16]! // 8-byte Folded Spill
; CHECK-NEXT:    .cfi_def_cfa_offset 16
; CHECK-NEXT:    .cfi_offset w30, -16
; CHECK-NEXT:    // kill: def $w2 killed $w2 def $x2
; CHECK-NEXT:    bl __lshrti3
; CHECK-NEXT:    ldr x30, [sp], #16 // 8-byte Folded Reload
; CHECK-NEXT:    ret
entry:
  %x.sroa.2.0.insert.ext = zext i64 %x.coerce1 to i128
  %x.sroa.2.0.insert.shift = shl nuw i128 %x.sroa.2.0.insert.ext, 64
  %x.sroa.0.0.insert.ext = zext i64 %x.coerce0 to i128
  %x.sroa.0.0.insert.insert = or i128 %x.sroa.2.0.insert.shift, %x.sroa.0.0.insert.ext
  %conv = sext i8 %y to i32
  %sh_prom = zext i32 %conv to i128
  %shr = lshr i128 %x.sroa.0.0.insert.insert, %sh_prom
  %retval.sroa.0.0.extract.trunc = trunc i128 %shr to i64
  %retval.sroa.2.0.extract.shift = lshr i128 %shr, 64
  %retval.sroa.2.0.extract.trunc = trunc i128 %retval.sroa.2.0.extract.shift to i64
  %.fca.0.insert = insertvalue { i64, i64 } undef, i64 %retval.sroa.0.0.extract.trunc, 0
  %.fca.1.insert = insertvalue { i64, i64 } %.fca.0.insert, i64 %retval.sroa.2.0.extract.trunc, 1
  ret { i64, i64 } %.fca.1.insert
}
