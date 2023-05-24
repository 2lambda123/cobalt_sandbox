// RUN: not llvm-mc -arch=amdgcn -mcpu=gfx900 -show-encoding %s | FileCheck -check-prefix=GFX9 -check-prefix=GCN %s
// RUN: not llvm-mc -arch=amdgcn -mcpu=gfx900 -show-encoding 2>&1 %s | FileCheck -check-prefix=GFX9-ERR -check-prefix=GCNERR %s
// RUN: not llvm-mc -arch=amdgcn -mcpu=tonga -show-encoding 2>&1 %s | FileCheck -check-prefix=VI-ERR -check-prefix=GCNERR %s

// RUN: not llvm-mc -arch=amdgcn -mcpu=gfx1010 -show-encoding %s | FileCheck --check-prefixes=GFX10 %s
// RUN: not llvm-mc -arch=amdgcn -mcpu=gfx1010 -show-encoding %s 2>&1 | FileCheck --check-prefixes=GFX10-ERR %s

global_load_ubyte v1, v[3:4], off
// GFX10: encoding: [0x00,0x80,0x20,0xdc,0x03,0x00,0x7d,0x01]
// GFX9: global_load_ubyte v1, v[3:4], off ; encoding: [0x00,0x80,0x40,0xdc,0x03,0x00,0x7f,0x01]
// VI-ERR: instruction not supported on this GPU

global_load_ubyte v1, v[3:4], off dlc
// GFX10: encoding: [0x00,0x90,0x20,0xdc,0x03,0x00,0x7d,0x01]
// GFX9-ERR: error: failed parsing operand
// VI-ERR: error: invalid operand for instruction

global_load_sbyte v1, v[3:4], off
// GFX10: encoding: [0x00,0x80,0x24,0xdc,0x03,0x00,0x7d,0x01]
// GFX9: global_load_sbyte v1, v[3:4], off ; encoding: [0x00,0x80,0x44,0xdc,0x03,0x00,0x7f,0x01]
// VI-ERR: instruction not supported on this GPU

global_load_sbyte v1, v[3:4], off dlc
// GFX10: encoding: [0x00,0x90,0x24,0xdc,0x03,0x00,0x7d,0x01]
// GFX9-ERR: error: failed parsing operand
// VI-ERR: error: invalid operand for instruction

global_load_ushort v1, v[3:4], off
// GFX10: encoding: [0x00,0x80,0x28,0xdc,0x03,0x00,0x7d,0x01]
// GFX9: global_load_ushort v1, v[3:4], off ; encoding: [0x00,0x80,0x48,0xdc,0x03,0x00,0x7f,0x01]
// VI-ERR: instruction not supported on this GPU

global_load_ushort v1, v[3:4], off dlc
// GFX10: encoding: [0x00,0x90,0x28,0xdc,0x03,0x00,0x7d,0x01]
// GFX9-ERR: error: failed parsing operand
// VI-ERR: error: invalid operand for instruction

global_load_sshort v1, v[3:4], off
// GFX10: encoding: [0x00,0x80,0x2c,0xdc,0x03,0x00,0x7d,0x01]
// GFX9: global_load_sshort v1, v[3:4], off ; encoding: [0x00,0x80,0x4c,0xdc,0x03,0x00,0x7f,0x01]
// VI-ERR: instruction not supported on this GPU

global_load_sshort v1, v[3:4], off dlc
// GFX10: encoding: [0x00,0x90,0x2c,0xdc,0x03,0x00,0x7d,0x01]
// GFX9-ERR: error: failed parsing operand
// VI-ERR: error: invalid operand for instruction

global_load_dword v1, v[3:4], off
// GFX10: encoding: [0x00,0x80,0x30,0xdc,0x03,0x00,0x7d,0x01]
// GFX9: global_load_dword v1, v[3:4], off ; encoding: [0x00,0x80,0x50,0xdc,0x03,0x00,0x7f,0x01]
// VI-ERR: instruction not supported on this GPU

global_load_dword v1, v[3:4], off dlc
// GFX10: encoding: [0x00,0x90,0x30,0xdc,0x03,0x00,0x7d,0x01]
// GFX9-ERR: error: failed parsing operand
// VI-ERR: error: invalid operand for instruction

global_load_dwordx2 v[1:2], v[3:4], off
// GFX10: encoding: [0x00,0x80,0x34,0xdc,0x03,0x00,0x7d,0x01]
// GFX9: global_load_dwordx2 v[1:2], v[3:4], off ; encoding: [0x00,0x80,0x54,0xdc,0x03,0x00,0x7f,0x01]
// VI-ERR: instruction not supported on this GPU

global_load_dwordx2 v[1:2], v[3:4], off dlc
// GFX10: encoding: [0x00,0x90,0x34,0xdc,0x03,0x00,0x7d,0x01]
// GFX9-ERR: error: failed parsing operand
// VI-ERR: error: invalid operand for instruction

global_load_dwordx3 v[1:3], v[3:4], off
// GFX10: encoding: [0x00,0x80,0x3c,0xdc,0x03,0x00,0x7d,0x01]
// GFX9: global_load_dwordx3 v[1:3], v[3:4], off ; encoding: [0x00,0x80,0x58,0xdc,0x03,0x00,0x7f,0x01]
// VI-ERR: instruction not supported on this GPU

global_load_dwordx3 v[1:3], v[3:4], off dlc
// GFX10: encoding: [0x00,0x90,0x3c,0xdc,0x03,0x00,0x7d,0x01]
// GFX9-ERR: error: failed parsing operand
// VI-ERR: error: invalid operand for instruction

global_load_dwordx4 v[1:4], v[3:4], off
// GFX10: encoding: [0x00,0x80,0x38,0xdc,0x03,0x00,0x7d,0x01]
// GFX9: global_load_dwordx4 v[1:4], v[3:4], off   ; encoding: [0x00,0x80,0x5c,0xdc,0x03,0x00,0x7f,0x01]
// VI-ERR: instruction not supported on this GPU

global_load_dwordx4 v[1:4], v[3:4], off dlc
// GFX10: encoding: [0x00,0x90,0x38,0xdc,0x03,0x00,0x7d,0x01]
// GFX9-ERR: error: failed parsing operand
// VI-ERR: error: invalid operand for instruction

// FIXME: VI error should be instruction nto supported
global_load_dword v1, v[3:4], off offset:0
// GFX10: encoding: [0x00,0x80,0x30,0xdc,0x03,0x00,0x7d,0x01]
// GFX9: global_load_dword v1, v[3:4], off    ; encoding: [0x00,0x80,0x50,0xdc,0x03,0x00,0x7f,0x01]
// VI-ERR: :35: error: not a valid operand.

global_load_dword v1, v[3:4], off offset:4095
// GFX10-ERR: :35: error: expected a 12-bit signed offset
// GFX9: global_load_dword v1, v[3:4], off offset:4095 ; encoding: [0xff,0x8f,0x50,0xdc,0x03,0x00,0x7f,0x01]
// VI-ERR: :35: error: not a valid operand.

global_load_dword v1, v[3:4], off offset:-1
// GFX10: encoding: [0xff,0x8f,0x30,0xdc,0x03,0x00,0x7d,0x01]
// GFX9: global_load_dword v1, v[3:4], off offset:-1 ; encoding: [0xff,0x9f,0x50,0xdc,0x03,0x00,0x7f,0x01]
// VI-ERR: :35: error: not a valid operand.

global_load_dword v1, v[3:4], off offset:-4096
// GFX10-ERR: :35: error: expected a 12-bit signed offset
// GFX9: global_load_dword v1, v[3:4], off offset:-4096 ; encoding: [0x00,0x90,0x50,0xdc,0x03,0x00,0x7f,0x01]
// VI-ERR: :35: error: not a valid operand.

global_load_dword v1, v[3:4], off offset:4096
// GFX10-ERR: :35: error: expected a 12-bit signed offset
// GFX9-ERR: :35: error: expected a 13-bit signed offset
// VI-ERR: :35: error: not a valid operand.

global_load_dword v1, v[3:4] off, offset:-4097
// GFX10-ERR: :35: error: expected a 12-bit signed offset
// GFX9-ERR: :35: error: expected a 13-bit signed offset
// VI-ERR: :35: error: not a valid operand.

global_store_byte v[3:4], v1, off
// GFX10: encoding: [0x00,0x80,0x60,0xdc,0x03,0x01,0x7d,0x00]
// GFX9: global_store_byte v[3:4], v1, off ; encoding: [0x00,0x80,0x60,0xdc,0x03,0x01,0x7f,0x00]
// VI-ERR: instruction not supported on this GPU

global_store_byte v[3:4], v1, off dlc
// GFX10: encoding: [0x00,0x90,0x60,0xdc,0x03,0x01,0x7d,0x00]
// GFX9-ERR: error: failed parsing operand
// VI-ERR: error: invalid operand for instruction

global_store_short v[3:4], v1, off
// GFX10: encoding: [0x00,0x80,0x68,0xdc,0x03,0x01,0x7d,0x00]
// GFX9: global_store_short v[3:4], v1, off ; encoding: [0x00,0x80,0x68,0xdc,0x03,0x01,0x7f,0x00]
// VI-ERR: instruction not supported on this GPU

global_store_short v[3:4], v1, off dlc
// GFX10: encoding: [0x00,0x90,0x68,0xdc,0x03,0x01,0x7d,0x00]
// GFX9-ERR: error: failed parsing operand
// VI-ERR: error: invalid operand for instruction

global_store_dword v[3:4], v1, off
// GFX10: encoding: [0x00,0x80,0x70,0xdc,0x03,0x01,0x7d,0x00]
// GFX9: global_store_dword v[3:4], v1, off ; encoding: [0x00,0x80,0x70,0xdc,0x03,0x01,0x7f,0x00]
// VI-ERR: instruction not supported on this GPU

global_store_dword v[3:4], v1, off dlc
// GFX10: encoding: [0x00,0x90,0x70,0xdc,0x03,0x01,0x7d,0x00]
// GFX9-ERR: error: failed parsing operand
// VI-ERR: error: invalid operand for instruction

global_store_dwordx2 v[3:4], v[1:2], off
// GFX10: encoding: [0x00,0x80,0x74,0xdc,0x03,0x01,0x7d,0x00]
// GFX9: global_store_dwordx2 v[3:4], v[1:2], off ; encoding: [0x00,0x80,0x74,0xdc,0x03,0x01,0x7f,0x00]
// VI-ERR: instruction not supported on this GPU

global_store_dwordx2 v[3:4], v[1:2], off dlc
// GFX10: encoding: [0x00,0x90,0x74,0xdc,0x03,0x01,0x7d,0x00]
// GFX9-ERR: error: failed parsing operand
// VI-ERR: error: invalid operand for instruction

global_store_dwordx3 v[3:4], v[1:3], off
// GFX10: encoding: [0x00,0x80,0x7c,0xdc,0x03,0x01,0x7d,0x00]
// GFX9: global_store_dwordx3 v[3:4], v[1:3], off ; encoding: [0x00,0x80,0x78,0xdc,0x03,0x01,0x7f,0x00]
// VI-ERR: instruction not supported on this GPU

global_store_dwordx3 v[3:4], v[1:3], off dlc
// GFX10: encoding: [0x00,0x90,0x7c,0xdc,0x03,0x01,0x7d,0x00]
// GFX9-ERR: error: failed parsing operand
// VI-ERR: error: invalid operand for instruction

global_store_dwordx4 v[3:4], v[1:4], off
// GFX10: encoding: [0x00,0x80,0x78,0xdc,0x03,0x01,0x7d,0x00]
// GFX9: global_store_dwordx4 v[3:4], v[1:4], off ; encoding: [0x00,0x80,0x7c,0xdc,0x03,0x01,0x7f,0x00]
// VI-ERR: instruction not supported on this GPU

global_store_dwordx4 v[3:4], v[1:4], off dlc
// GFX10: encoding: [0x00,0x90,0x78,0xdc,0x03,0x01,0x7d,0x00]
// GFX9-ERR: error: failed parsing operand
// VI-ERR: error: invalid operand for instruction

global_store_dword v[3:4], v1, off offset:12
// GFX10: encoding: [0x0c,0x80,0x70,0xdc,0x03,0x01,0x7d,0x00]
// GFX9: global_store_dword v[3:4], v1, off offset:12 ; encoding: [0x0c,0x80,0x70,0xdc,0x03,0x01,0x7f,0x00]
// VI-ERR: :36: error: not a valid operand

global_load_dword v1, v[3:4], s[2:3]
// GFX10: encoding: [0x00,0x80,0x30,0xdc,0x03,0x00,0x02,0x01]
// GFX9: global_load_dword v1, v[3:4], s[2:3] ; encoding: [0x00,0x80,0x50,0xdc,0x03,0x00,0x02,0x01]
// VI-ERR: instruction not supported on this GPU

global_load_dword v1, v[3:4], s[2:3] offset:24
// GFX10: encoding: [0x18,0x80,0x30,0xdc,0x03,0x00,0x02,0x01]
// GFX9: global_load_dword v1, v[3:4], s[2:3] offset:24 ; encoding: [0x18,0x80,0x50,0xdc,0x03,0x00,0x02,0x01]
// VI-ERR: :38: error: not a valid operand.

global_load_dword v1, v[3:4], s[2:3] offset:-8
// GFX10: encoding: [0xf8,0x8f,0x30,0xdc,0x03,0x00,0x02,0x01]
// GFX9: global_load_dword v1, v[3:4], s[2:3] offset:-8 ; encoding: [0xf8,0x9f,0x50,0xdc,0x03,0x00,0x02,0x01]
// VI-ERR: :38: error: not a valid operand.

global_store_dword v[3:4], v1, s[2:3]
// GFX10: encoding: [0x00,0x80,0x70,0xdc,0x03,0x01,0x02,0x00]
// GFX9: global_store_dword v[3:4], v1, s[2:3] ; encoding: [0x00,0x80,0x70,0xdc,0x03,0x01,0x02,0x00]
// VI-ERR: instruction not supported on this GPU

global_store_dword v[3:4], v1, s[2:3] offset:24
// GFX10: encoding: [0x18,0x80,0x70,0xdc,0x03,0x01,0x02,0x00]
// GFX9: global_store_dword v[3:4], v1, s[2:3] offset:24 ; encoding: [0x18,0x80,0x70,0xdc,0x03,0x01,0x02,0x00]
// VI-ERR: :39: error: not a valid operand.

global_store_dword v[3:4], v1, s[2:3] offset:-8
// GFX10: encoding: [0xf8,0x8f,0x70,0xdc,0x03,0x01,0x02,0x00]
// GFX9: global_store_dword v[3:4], v1, s[2:3] offset:-8 ; encoding: [0xf8,0x9f,0x70,0xdc,0x03,0x01,0x02,0x00]
// VI-ERR: :39: error: not a valid operand.

// XXX: Is this valid?
global_store_dword v[3:4], v1, exec
// GFX10: encoding: [0x00,0x80,0x70,0xdc,0x03,0x01,0x7e,0x00]
// GFX9: global_store_dword v[3:4], v1, exec ; encoding: [0x00,0x80,0x70,0xdc,0x03,0x01,0x7e,0x00]
// VI-ERR: instruction not supported on this GPU

global_load_dword v1, v[3:4], s2
// GFX10-ERR: error: invalid operand for instruction
// GFX9-ERR: :31: error: invalid operand for instruction
// VI-ERR: :31: error: invalid operand for instruction

global_load_dword v1, v[3:4], exec_hi
// GFX10-ERR: error: invalid operand for instruction
// GFX9-ERR: :31: error: invalid operand for instruction
// VI-ERR: :31: error: invalid operand for instruction

global_atomic_cmpswap v[3:4], v[5:6], off
// GFX10: encoding: [0x00,0x80,0xc4,0xdc,0x03,0x05,0x7d,0x00]
// GFX9: global_atomic_cmpswap v[3:4], v[5:6], off ; encoding: [0x00,0x80,0x04,0xdd,0x03,0x05,0x7f,0x00]
// VI-ERR: error: instruction not supported on this GPU

global_atomic_cmpswap_x2 v[3:4], v[5:8], off
// GFX10: encoding: [0x00,0x80,0x44,0xdd,0x03,0x05,0x7d,0x00]
// GFX9: global_atomic_cmpswap_x2 v[3:4], v[5:8], off ; encoding: [0x00,0x80,0x84,0xdd,0x03,0x05,0x7f,0x00]
// VI-ERR: error: instruction not supported on this GPU

global_atomic_swap v[3:4], v5, off
// GFX10: encoding: [0x00,0x80,0xc0,0xdc,0x03,0x05,0x7d,0x00]
// GFX9: global_atomic_swap v[3:4], v5, off   ; encoding: [0x00,0x80,0x00,0xdd,0x03,0x05,0x7f,0x00]
// VI-ERR: error: instruction not supported on this GPU

global_atomic_swap_x2 v[3:4], v[5:6], off
// GFX10: encoding: [0x00,0x80,0x40,0xdd,0x03,0x05,0x7d,0x00]
// GFX9: global_atomic_swap_x2 v[3:4], v[5:6], off ; encoding: [0x00,0x80,0x80,0xdd,0x03,0x05,0x7f,0x00]
// VI-ERR: error: instruction not supported on this GPU

global_atomic_add v[3:4], v5, off
// GFX10: encoding: [0x00,0x80,0xc8,0xdc,0x03,0x05,0x7d,0x00]
// GFX9: global_atomic_add v[3:4], v5, off   ; encoding: [0x00,0x80,0x08,0xdd,0x03,0x05,0x7f,0x00]
// VI-ERR: instruction not supported on this GPU

global_atomic_sub v[3:4], v5, off
// GFX10: encoding: [0x00,0x80,0xcc,0xdc,0x03,0x05,0x7d,0x00]
// GFX9: global_atomic_sub v[3:4], v5, off    ; encoding: [0x00,0x80,0x0c,0xdd,0x03,0x05,0x7f,0x00]
// VI-ERR: instruction not supported on this GPU

global_atomic_smin v[3:4], v5, off
// GFX10: encoding: [0x00,0x80,0xd4,0xdc,0x03,0x05,0x7d,0x00]
// GFX9: global_atomic_smin v[3:4], v5, off   ; encoding: [0x00,0x80,0x10,0xdd,0x03,0x05,0x7f,0x00]
// VI-ERR: instruction not supported on this GPU

global_atomic_umin v[3:4], v5, off
// GFX10: encoding: [0x00,0x80,0xd8,0xdc,0x03,0x05,0x7d,0x00]
// GFX9: global_atomic_umin v[3:4], v5, off   ; encoding: [0x00,0x80,0x14,0xdd,0x03,0x05,0x7f,0x00]
// VI-ERR: instruction not supported on this GPU

global_atomic_smax v[3:4], v5, off
// GFX10: encoding: [0x00,0x80,0xdc,0xdc,0x03,0x05,0x7d,0x00]
// GFX9: global_atomic_smax v[3:4], v5, off   ; encoding: [0x00,0x80,0x18,0xdd,0x03,0x05,0x7f,0x00]
// VI-ERR: instruction not supported on this GPU

global_atomic_umax v[3:4], v5, off
// GFX10: encoding: [0x00,0x80,0xe0,0xdc,0x03,0x05,0x7d,0x00]
// GFX9: global_atomic_umax v[3:4], v5, off   ; encoding: [0x00,0x80,0x1c,0xdd,0x03,0x05,0x7f,0x00]
// VI-ERR: instruction not supported on this GPU

global_atomic_and v[3:4], v5, off
// GFX10: encoding: [0x00,0x80,0xe4,0xdc,0x03,0x05,0x7d,0x00]
// GFX9: global_atomic_and v[3:4], v5, off    ; encoding: [0x00,0x80,0x20,0xdd,0x03,0x05,0x7f,0x00]
// VI-ERR: instruction not supported on this GPU

global_atomic_or v[3:4], v5, off
// GFX10: encoding: [0x00,0x80,0xe8,0xdc,0x03,0x05,0x7d,0x00]
// GFX9: global_atomic_or v[3:4], v5, off     ; encoding: [0x00,0x80,0x24,0xdd,0x03,0x05,0x7f,0x00]
// VI-ERR: instruction not supported on this GPU

global_atomic_xor v[3:4], v5, off
// GFX10: encoding: [0x00,0x80,0xec,0xdc,0x03,0x05,0x7d,0x00]
// GFX9: global_atomic_xor v[3:4], v5, off    ; encoding: [0x00,0x80,0x28,0xdd,0x03,0x05,0x7f,0x00]
// VI-ERR: instruction not supported on this GPU

global_atomic_inc v[3:4], v5, off
// GFX10: encoding: [0x00,0x80,0xf0,0xdc,0x03,0x05,0x7d,0x00]
// GFX9: global_atomic_inc v[3:4], v5, off    ; encoding: [0x00,0x80,0x2c,0xdd,0x03,0x05,0x7f,0x00]
// VI-ERR: instruction not supported on this GPU

global_atomic_dec v[3:4], v5, off
// GFX10: encoding: [0x00,0x80,0xf4,0xdc,0x03,0x05,0x7d,0x00]
// GFX9: global_atomic_dec v[3:4], v5, off    ; encoding: [0x00,0x80,0x30,0xdd,0x03,0x05,0x7f,0x00]
// VI-ERR: instruction not supported on this GPU

global_atomic_add_x2 v[3:4], v[5:6], off
// GFX10: encoding: [0x00,0x80,0x48,0xdd,0x03,0x05,0x7d,0x00]
// GFX9: global_atomic_add_x2 v[3:4], v[5:6], off ; encoding: [0x00,0x80,0x88,0xdd,0x03,0x05,0x7f,0x00]
// VI-ERR: instruction not supported on this GPU

global_atomic_sub_x2 v[3:4], v[5:6], off
// GFX10: encoding: [0x00,0x80,0x4c,0xdd,0x03,0x05,0x7d,0x00]
// GFX9: global_atomic_sub_x2 v[3:4], v[5:6], off ; encoding: [0x00,0x80,0x8c,0xdd,0x03,0x05,0x7f,0x00]
// VI-ERR: instruction not supported on this GPU

global_atomic_smin_x2 v[3:4], v[5:6], off
// GFX10: encoding: [0x00,0x80,0x54,0xdd,0x03,0x05,0x7d,0x00]
// GFX9: global_atomic_smin_x2 v[3:4], v[5:6], off ; encoding: [0x00,0x80,0x90,0xdd,0x03,0x05,0x7f,0x00]
// VI-ERR: instruction not supported on this GPU

global_atomic_umin_x2 v[3:4], v[5:6], off
// GFX10: encoding: [0x00,0x80,0x58,0xdd,0x03,0x05,0x7d,0x00]
// GFX9: global_atomic_umin_x2 v[3:4], v[5:6], off ; encoding: [0x00,0x80,0x94,0xdd,0x03,0x05,0x7f,0x00]
// VI-ERR: instruction not supported on this GPU

global_atomic_smax_x2 v[3:4], v[5:6], off
// GFX10: encoding: [0x00,0x80,0x5c,0xdd,0x03,0x05,0x7d,0x00]
// GFX9: global_atomic_smax_x2 v[3:4], v[5:6], off ; encoding: [0x00,0x80,0x98,0xdd,0x03,0x05,0x7f,0x00]
// VI-ERR: instruction not supported on this GPU

global_atomic_umax_x2 v[3:4], v[5:6], off
// GFX10: encoding: [0x00,0x80,0x60,0xdd,0x03,0x05,0x7d,0x00]
// GFX9: global_atomic_umax_x2 v[3:4], v[5:6], off ; encoding: [0x00,0x80,0x9c,0xdd,0x03,0x05,0x7f,0x00]
// VI-ERR: instruction not supported on this GPU

global_atomic_and_x2 v[3:4], v[5:6], off
// GFX10: encoding: [0x00,0x80,0x64,0xdd,0x03,0x05,0x7d,0x00]
// GFX9: global_atomic_and_x2 v[3:4], v[5:6], off ; encoding: [0x00,0x80,0xa0,0xdd,0x03,0x05,0x7f,0x00]
// VI-ERR: instruction not supported on this GPU

global_atomic_or_x2 v[3:4], v[5:6], off
// GFX10: encoding: [0x00,0x80,0x68,0xdd,0x03,0x05,0x7d,0x00]
// GFX9: global_atomic_or_x2 v[3:4], v[5:6], off ; encoding: [0x00,0x80,0xa4,0xdd,0x03,0x05,0x7f,0x00]
// VI-ERR: instruction not supported on this GPU

global_atomic_xor_x2 v[3:4], v[5:6], off
// GFX10: encoding: [0x00,0x80,0x6c,0xdd,0x03,0x05,0x7d,0x00]
// GFX9: global_atomic_xor_x2 v[3:4], v[5:6], off ; encoding: [0x00,0x80,0xa8,0xdd,0x03,0x05,0x7f,0x00]
// VI-ERR: instruction not supported on this GPU

global_atomic_inc_x2 v[3:4], v[5:6], off
// GFX10: encoding: [0x00,0x80,0x70,0xdd,0x03,0x05,0x7d,0x00]
// GFX9: global_atomic_inc_x2 v[3:4], v[5:6], off ; encoding: [0x00,0x80,0xac,0xdd,0x03,0x05,0x7f,0x00]
// VI-ERR: instruction not supported on this GPU

global_atomic_dec_x2 v[3:4], v[5:6], off
// GFX10: encoding: [0x00,0x80,0x74,0xdd,0x03,0x05,0x7d,0x00]
// GFX9: global_atomic_dec_x2 v[3:4], v[5:6], off ; encoding: [0x00,0x80,0xb0,0xdd,0x03,0x05,0x7f,0x00]
// VI-ERR: error: instruction not supported on this GPU

global_atomic_cmpswap v[3:4], v[5:6], off offset:-16
// GFX10: encoding: [0xf0,0x8f,0xc4,0xdc,0x03,0x05,0x7d,0x00]
// GFX9: global_atomic_cmpswap v[3:4], v[5:6], off offset:-16 ; encoding: [0xf0,0x9f,0x04,0xdd,0x03,0x05,0x7f,0x00]
// VI-ERR: :43: error: not a valid operand.

global_atomic_cmpswap_x2 v[3:4], v[5:8], off offset:-16
// GFX10: encoding: [0xf0,0x8f,0x44,0xdd,0x03,0x05,0x7d,0x00]
// GFX9: global_atomic_cmpswap_x2 v[3:4], v[5:8], off offset:-16 ; encoding: [0xf0,0x9f,0x84,0xdd,0x03,0x05,0x7f,0x00]
// VI-ERR: :46: error: not a valid operand.

global_atomic_swap v[3:4], v5, off offset:-16
// GFX10: encoding: [0xf0,0x8f,0xc0,0xdc,0x03,0x05,0x7d,0x00]
// GFX9: global_atomic_swap v[3:4], v5, off   offset:-16 ; encoding: [0xf0,0x9f,0x00,0xdd,0x03,0x05,0x7f,0x00]
// VI-ERR: :36: error: not a valid operand

global_atomic_swap_x2 v[3:4], v[5:6], off offset:-16
// GFX10: encoding: [0xf0,0x8f,0x40,0xdd,0x03,0x05,0x7d,0x00]
// GFX9: global_atomic_swap_x2 v[3:4], v[5:6], off offset:-16 ; encoding: [0xf0,0x9f,0x80,0xdd,0x03,0x05,0x7f,0x00]
// VI-ERR: :43: error: not a valid operand

global_atomic_add v[3:4], v5, off offset:-16
// GFX10: encoding: [0xf0,0x8f,0xc8,0xdc,0x03,0x05,0x7d,0x00]
// GFX9: global_atomic_add v[3:4], v5, off offset:-16 ; encoding: [0xf0,0x9f,0x08,0xdd,0x03,0x05,0x7f,0x00]
// VI-ERR: :35: error: not a valid operand

global_atomic_sub v[3:4], v5, off offset:-16
// GFX10: encoding: [0xf0,0x8f,0xcc,0xdc,0x03,0x05,0x7d,0x00]
// GFX9: global_atomic_sub v[3:4], v5, off offset:-16 ; encoding: [0xf0,0x9f,0x0c,0xdd,0x03,0x05,0x7f,0x00]
// VI-ERR: :35: error: not a valid operand

global_atomic_smin v[3:4], v5, off offset:-16
// GFX10: encoding: [0xf0,0x8f,0xd4,0xdc,0x03,0x05,0x7d,0x00]
// GFX9: global_atomic_smin v[3:4], v5, off offset:-16 ; encoding: [0xf0,0x9f,0x10,0xdd,0x03,0x05,0x7f,0x00]
// VI-ERR: :36: error: not a valid operand

global_atomic_umin v[3:4], v5, off offset:-16
// GFX10: encoding: [0xf0,0x8f,0xd8,0xdc,0x03,0x05,0x7d,0x00]
// GFX9: global_atomic_umin v[3:4], v5, off offset:-16 ; encoding: [0xf0,0x9f,0x14,0xdd,0x03,0x05,0x7f,0x00]
// VI-ERR: :36: error: not a valid operand

global_atomic_smax v[3:4], v5, off offset:-16
// GFX10: encoding: [0xf0,0x8f,0xdc,0xdc,0x03,0x05,0x7d,0x00]
// GFX9: global_atomic_smax v[3:4], v5, off offset:-16 ; encoding: [0xf0,0x9f,0x18,0xdd,0x03,0x05,0x7f,0x00]
// VI-ERR: :36: error: not a valid operand

global_atomic_umax v[3:4], v5, off offset:-16
// GFX10: encoding: [0xf0,0x8f,0xe0,0xdc,0x03,0x05,0x7d,0x00]
// GFX9: global_atomic_umax v[3:4], v5, off offset:-16 ; encoding: [0xf0,0x9f,0x1c,0xdd,0x03,0x05,0x7f,0x00]
// VI-ERR: :36: error: not a valid operand

global_atomic_and v[3:4], v5, off offset:-16
// GFX10: encoding: [0xf0,0x8f,0xe4,0xdc,0x03,0x05,0x7d,0x00]
// GFX9: global_atomic_and v[3:4], v5, off offset:-16 ; encoding: [0xf0,0x9f,0x20,0xdd,0x03,0x05,0x7f,0x00]
// VI-ERR: :35: error: not a valid operand

global_atomic_or v[3:4], v5, off offset:-16
// GFX10: encoding: [0xf0,0x8f,0xe8,0xdc,0x03,0x05,0x7d,0x00]
// GFX9: global_atomic_or v[3:4], v5, off offset:-16 ; encoding: [0xf0,0x9f,0x24,0xdd,0x03,0x05,0x7f,0x00]
// VI-ERR: :34: error: not a valid operand

global_atomic_xor v[3:4], v5, off offset:-16
// GFX10: encoding: [0xf0,0x8f,0xec,0xdc,0x03,0x05,0x7d,0x00]
// GFX9: global_atomic_xor v[3:4], v5, off  offset:-16 ; encoding: [0xf0,0x9f,0x28,0xdd,0x03,0x05,0x7f,0x00]
// VI-ERR: :35: error: not a valid operand

global_atomic_inc v[3:4], v5, off offset:-16
// GFX10: encoding: [0xf0,0x8f,0xf0,0xdc,0x03,0x05,0x7d,0x00]
// GFX9: global_atomic_inc v[3:4], v5, off offset:-16 ; encoding: [0xf0,0x9f,0x2c,0xdd,0x03,0x05,0x7f,0x00]
// VI-ERR: :35: error: not a valid operand

global_atomic_dec v[3:4], v5, off offset:-16
// GFX10: encoding: [0xf0,0x8f,0xf4,0xdc,0x03,0x05,0x7d,0x00]
// GFX9: global_atomic_dec v[3:4], v5, off offset:-16 ; encoding: [0xf0,0x9f,0x30,0xdd,0x03,0x05,0x7f,0x00]
// VI-ERR: :35: error: not a valid operand

global_atomic_add_x2 v[3:4], v[5:6], off offset:-16
// GFX10: encoding: [0xf0,0x8f,0x48,0xdd,0x03,0x05,0x7d,0x00]
// GFX9: global_atomic_add_x2 v[3:4], v[5:6], off offset:-16 ; encoding: [0xf0,0x9f,0x88,0xdd,0x03,0x05,0x7f,0x00]
// VI-ERR: :42: error: not a valid operand

global_atomic_sub_x2 v[3:4], v[5:6], off offset:-16
// GFX10: encoding: [0xf0,0x8f,0x4c,0xdd,0x03,0x05,0x7d,0x00]
// GFX9: global_atomic_sub_x2 v[3:4], v[5:6], off offset:-16 ; encoding: [0xf0,0x9f,0x8c,0xdd,0x03,0x05,0x7f,0x00]
// VI-ERR: :42: error: not a valid operand

global_atomic_smin_x2 v[3:4], v[5:6], off offset:-16
// GFX10: encoding: [0xf0,0x8f,0x54,0xdd,0x03,0x05,0x7d,0x00]
// GFX9: global_atomic_smin_x2 v[3:4], v[5:6], off offset:-16 ; encoding: [0xf0,0x9f,0x90,0xdd,0x03,0x05,0x7f,0x00]
// VI-ERR: :43: error: not a valid operand

global_atomic_umin_x2 v[3:4], v[5:6], off offset:-16
// GFX10: encoding: [0xf0,0x8f,0x58,0xdd,0x03,0x05,0x7d,0x00]
// GFX9: global_atomic_umin_x2 v[3:4], v[5:6], off offset:-16 ; encoding: [0xf0,0x9f,0x94,0xdd,0x03,0x05,0x7f,0x00]
// VI-ERR: :43: error: not a valid operand

global_atomic_smax_x2 v[3:4], v[5:6], off offset:-16
// GFX10: encoding: [0xf0,0x8f,0x5c,0xdd,0x03,0x05,0x7d,0x00]
// GFX9: global_atomic_smax_x2 v[3:4], v[5:6], off offset:-16 ; encoding: [0xf0,0x9f,0x98,0xdd,0x03,0x05,0x7f,0x00]
// VI-ERR: :43: error: not a valid operand

global_atomic_umax_x2 v[3:4], v[5:6], off offset:-16
// GFX10: encoding: [0xf0,0x8f,0x60,0xdd,0x03,0x05,0x7d,0x00]
// GFX9: global_atomic_umax_x2 v[3:4], v[5:6], off offset:-16 ; encoding: [0xf0,0x9f,0x9c,0xdd,0x03,0x05,0x7f,0x00]
// VI-ERR: :43: error: not a valid operand

global_atomic_and_x2 v[3:4], v[5:6], off offset:-16
// GFX10: encoding: [0xf0,0x8f,0x64,0xdd,0x03,0x05,0x7d,0x00]
// GFX9: global_atomic_and_x2 v[3:4], v[5:6], off offset:-16 ; encoding: [0xf0,0x9f,0xa0,0xdd,0x03,0x05,0x7f,0x00]
// VI-ERR: :42: error: not a valid operand

global_atomic_or_x2 v[3:4], v[5:6], off offset:-16
// GFX10: encoding: [0xf0,0x8f,0x68,0xdd,0x03,0x05,0x7d,0x00]
// GFX9: global_atomic_or_x2 v[3:4], v[5:6], off offset:-16 ; encoding: [0xf0,0x9f,0xa4,0xdd,0x03,0x05,0x7f,0x00]
// VI-ERR: :41: error: not a valid operand

global_atomic_xor_x2 v[3:4], v[5:6], off offset:-16
// GFX10: encoding: [0xf0,0x8f,0x6c,0xdd,0x03,0x05,0x7d,0x00]
// GFX9: global_atomic_xor_x2 v[3:4], v[5:6], off offset:-16 ; encoding: [0xf0,0x9f,0xa8,0xdd,0x03,0x05,0x7f,0x00]
// VI-ERR: :42: error: not a valid operand

global_atomic_inc_x2 v[3:4], v[5:6], off offset:-16
// GFX10: encoding: [0xf0,0x8f,0x70,0xdd,0x03,0x05,0x7d,0x00]
// GFX9: global_atomic_inc_x2 v[3:4], v[5:6], off offset:-16 ; encoding: [0xf0,0x9f,0xac,0xdd,0x03,0x05,0x7f,0x00]
// VI-ERR: :42: error: not a valid operand

global_atomic_dec_x2 v[3:4], v[5:6], off offset:-16
// GFX10: encoding: [0xf0,0x8f,0x74,0xdd,0x03,0x05,0x7d,0x00]
// GFX9: global_atomic_dec_x2 v[3:4], v[5:6], off offset:-16 ; encoding: [0xf0,0x9f,0xb0,0xdd,0x03,0x05,0x7f,0x00]
// VI-ERR: :42: error: not a valid operand

global_load_ubyte_d16 v1, v[3:4], off
// GFX10: encoding: [0x00,0x80,0x80,0xdc,0x03,0x00,0x7d,0x01]
// GFX9: global_load_ubyte_d16 v1, v[3:4], off ; encoding: [0x00,0x80,0x80,0xdc,0x03,0x00,0x7f,0x01]
// VI-ERR: instruction not supported on this GPU

global_load_ubyte_d16_hi v1, v[3:4], off
// GFX10: encoding: [0x00,0x80,0x84,0xdc,0x03,0x00,0x7d,0x01]
// GFX9: global_load_ubyte_d16_hi v1, v[3:4], off ; encoding: [0x00,0x80,0x84,0xdc,0x03,0x00,0x7f,0x01]
// VI-ERR: instruction not supported on this GPU

global_load_sbyte_d16 v1, v[3:4], off
// GFX10: encoding: [0x00,0x80,0x88,0xdc,0x03,0x00,0x7d,0x01]
// GFX9: global_load_sbyte_d16 v1, v[3:4], off ; encoding: [0x00,0x80,0x88,0xdc,0x03,0x00,0x7f,0x01]
// VI-ERR: instruction not supported on this GPU

global_load_sbyte_d16_hi v1, v[3:4], off
// GFX10: encoding: [0x00,0x80,0x8c,0xdc,0x03,0x00,0x7d,0x01]
// GFX9: global_load_sbyte_d16_hi v1, v[3:4], off ; encoding: [0x00,0x80,0x8c,0xdc,0x03,0x00,0x7f,0x01]
// VI-ERR: instruction not supported on this GPU

global_load_short_d16 v1, v[3:4], off
// GFX10: encoding: [0x00,0x80,0x90,0xdc,0x03,0x00,0x7d,0x01]
// GFX9: global_load_short_d16 v1, v[3:4], off ; encoding: [0x00,0x80,0x90,0xdc,0x03,0x00,0x7f,0x01]
// VI-ERR: instruction not supported on this GPU

global_load_short_d16_hi v1, v[3:4], off
// GFX10: encoding: [0x00,0x80,0x94,0xdc,0x03,0x00,0x7d,0x01]
// GFX9: global_load_short_d16_hi v1, v[3:4], off ; encoding: [0x00,0x80,0x94,0xdc,0x03,0x00,0x7f,0x01]
// VI-ERR: instruction not supported on this GPU

global_store_byte_d16_hi v[3:4], v1, off
// GFX10: encoding: [0x00,0x80,0x64,0xdc,0x03,0x01,0x7d,0x00]
// GFX9: global_store_byte_d16_hi v[3:4], v1, off ; encoding: [0x00,0x80,0x64,0xdc,0x03,0x01,0x7f,0x00]
// VI-ERR: instruction not supported on this GPU

global_store_short_d16_hi v[3:4], v1, off
// GFX10: encoding: [0x00,0x80,0x6c,0xdc,0x03,0x01,0x7d,0x00]
// GFX9: global_store_short_d16_hi v[3:4], v1, off ; encoding: [0x00,0x80,0x6c,0xdc,0x03,0x01,0x7f,0x00]
// VI-ERR: instruction not supported on this GPU

global_atomic_add v0, v[1:2], v2, off glc slc
// GFX10: global_atomic_add v0, v[1:2], v2, off glc slc ; encoding: [0x00,0x80,0xcb,0xdc,0x01,0x02,0x7d,0x00]
// GFX9: global_atomic_add v0, v[1:2], v2, off glc slc ; encoding: [0x00,0x80,0x0b,0xdd,0x01,0x02,0x7f,0x00]
// VI-ERR: error: invalid operand for instruction
