; Runs original SDAG test with -global-isel
; RUN: llc -march=amdgcn -mcpu=gfx1010 -mattr=+wavefrontsize32,-wavefrontsize64 -verify-machineinstrs < %S/../llvm.amdgcn.init.exec.wave32.ll | FileCheck -check-prefixes=GCN,GFX1032  %S/../llvm.amdgcn.init.exec.wave32.ll
