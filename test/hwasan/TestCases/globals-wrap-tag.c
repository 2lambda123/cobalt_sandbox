// Tests that global tags wrap around successfully after tag 0xff on aarch64 or
// 0x3f on x86_64.
// RUN: %clang_hwasan %s -o %t
// RUN: %run %t 0

#include <stdlib.h>

// Create >256 globals to ensure every tag is used.
int x0, x1, x2, x3, x4, x5, x6, x7, x8, x9, x10, x11, x12, x13, x14, x15, x16,
    x17, x18, x19, x20, x21, x22, x23, x24, x25, x26, x27, x28, x29, x30, x31,
    x32, x33, x34, x35, x36, x37, x38, x39, x40, x41, x42, x43, x44, x45, x46,
    x47, x48, x49, x50, x51, x52, x53, x54, x55, x56, x57, x58, x59, x60, x61,
    x62, x63, x64, x65, x66, x67, x68, x69, x70, x71, x72, x73, x74, x75, x76,
    x77, x78, x79, x80, x81, x82, x83, x84, x85, x86, x87, x88, x89, x90, x91,
    x92, x93, x94, x95, x96, x97, x98, x99, x100, x101, x102, x103, x104, x105,
    x106, x107, x108, x109, x110, x111, x112, x113, x114, x115, x116, x117,
    x118, x119, x120, x121, x122, x123, x124, x125, x126, x127, x128, x129,
    x130, x131, x132, x133, x134, x135, x136, x137, x138, x139, x140, x141,
    x142, x143, x144, x145, x146, x147, x148, x149, x150, x151, x152, x153,
    x154, x155, x156, x157, x158, x159, x160, x161, x162, x163, x164, x165,
    x166, x167, x168, x169, x170, x171, x172, x173, x174, x175, x176, x177,
    x178, x179, x180, x181, x182, x183, x184, x185, x186, x187, x188, x189,
    x190, x191, x192, x193, x194, x195, x196, x197, x198, x199, x200, x201,
    x202, x203, x204, x205, x206, x207, x208, x209, x210, x211, x212, x213,
    x214, x215, x216, x217, x218, x219, x220, x221, x222, x223, x224, x225,
    x226, x227, x228, x229, x230, x231, x232, x233, x234, x235, x236, x237,
    x238, x239, x240, x241, x242, x243, x244, x245, x246, x247, x248, x249,
    x250, x251, x252, x253, x254, x255, x256;

int main(int argc, char **argv) {
  int i = atoi(argv[1]);

  // Write to every global to make sure tags match even after wrapping around
  // to 0.
  (&x0)[i] = 1;
  (&x1)[i] = 1;
  (&x2)[i] = 1;
  (&x3)[i] = 1;
  (&x4)[i] = 1;
  (&x5)[i] = 1;
  (&x6)[i] = 1;
  (&x7)[i] = 1;
  (&x8)[i] = 1;
  (&x9)[i] = 1;
  (&x10)[i] = 1;
  (&x11)[i] = 1;
  (&x12)[i] = 1;
  (&x13)[i] = 1;
  (&x14)[i] = 1;
  (&x15)[i] = 1;
  (&x16)[i] = 1;
  (&x17)[i] = 1;
  (&x18)[i] = 1;
  (&x19)[i] = 1;
  (&x20)[i] = 1;
  (&x21)[i] = 1;
  (&x22)[i] = 1;
  (&x23)[i] = 1;
  (&x24)[i] = 1;
  (&x25)[i] = 1;
  (&x26)[i] = 1;
  (&x27)[i] = 1;
  (&x28)[i] = 1;
  (&x29)[i] = 1;
  (&x30)[i] = 1;
  (&x31)[i] = 1;
  (&x32)[i] = 1;
  (&x33)[i] = 1;
  (&x34)[i] = 1;
  (&x35)[i] = 1;
  (&x36)[i] = 1;
  (&x37)[i] = 1;
  (&x38)[i] = 1;
  (&x39)[i] = 1;
  (&x40)[i] = 1;
  (&x41)[i] = 1;
  (&x42)[i] = 1;
  (&x43)[i] = 1;
  (&x44)[i] = 1;
  (&x45)[i] = 1;
  (&x46)[i] = 1;
  (&x47)[i] = 1;
  (&x48)[i] = 1;
  (&x49)[i] = 1;
  (&x50)[i] = 1;
  (&x51)[i] = 1;
  (&x52)[i] = 1;
  (&x53)[i] = 1;
  (&x54)[i] = 1;
  (&x55)[i] = 1;
  (&x56)[i] = 1;
  (&x57)[i] = 1;
  (&x58)[i] = 1;
  (&x59)[i] = 1;
  (&x60)[i] = 1;
  (&x61)[i] = 1;
  (&x62)[i] = 1;
  (&x63)[i] = 1;
  (&x64)[i] = 1;
  (&x65)[i] = 1;
  (&x66)[i] = 1;
  (&x67)[i] = 1;
  (&x68)[i] = 1;
  (&x69)[i] = 1;
  (&x70)[i] = 1;
  (&x71)[i] = 1;
  (&x72)[i] = 1;
  (&x73)[i] = 1;
  (&x74)[i] = 1;
  (&x75)[i] = 1;
  (&x76)[i] = 1;
  (&x77)[i] = 1;
  (&x78)[i] = 1;
  (&x79)[i] = 1;
  (&x80)[i] = 1;
  (&x81)[i] = 1;
  (&x82)[i] = 1;
  (&x83)[i] = 1;
  (&x84)[i] = 1;
  (&x85)[i] = 1;
  (&x86)[i] = 1;
  (&x87)[i] = 1;
  (&x88)[i] = 1;
  (&x89)[i] = 1;
  (&x90)[i] = 1;
  (&x91)[i] = 1;
  (&x92)[i] = 1;
  (&x93)[i] = 1;
  (&x94)[i] = 1;
  (&x95)[i] = 1;
  (&x96)[i] = 1;
  (&x97)[i] = 1;
  (&x98)[i] = 1;
  (&x99)[i] = 1;
  (&x100)[i] = 1;
  (&x101)[i] = 1;
  (&x102)[i] = 1;
  (&x103)[i] = 1;
  (&x104)[i] = 1;
  (&x105)[i] = 1;
  (&x106)[i] = 1;
  (&x107)[i] = 1;
  (&x108)[i] = 1;
  (&x109)[i] = 1;
  (&x110)[i] = 1;
  (&x111)[i] = 1;
  (&x112)[i] = 1;
  (&x113)[i] = 1;
  (&x114)[i] = 1;
  (&x115)[i] = 1;
  (&x116)[i] = 1;
  (&x117)[i] = 1;
  (&x118)[i] = 1;
  (&x119)[i] = 1;
  (&x120)[i] = 1;
  (&x121)[i] = 1;
  (&x122)[i] = 1;
  (&x123)[i] = 1;
  (&x124)[i] = 1;
  (&x125)[i] = 1;
  (&x126)[i] = 1;
  (&x127)[i] = 1;
  (&x128)[i] = 1;
  (&x129)[i] = 1;
  (&x130)[i] = 1;
  (&x131)[i] = 1;
  (&x132)[i] = 1;
  (&x133)[i] = 1;
  (&x134)[i] = 1;
  (&x135)[i] = 1;
  (&x136)[i] = 1;
  (&x137)[i] = 1;
  (&x138)[i] = 1;
  (&x139)[i] = 1;
  (&x140)[i] = 1;
  (&x141)[i] = 1;
  (&x142)[i] = 1;
  (&x143)[i] = 1;
  (&x144)[i] = 1;
  (&x145)[i] = 1;
  (&x146)[i] = 1;
  (&x147)[i] = 1;
  (&x148)[i] = 1;
  (&x149)[i] = 1;
  (&x150)[i] = 1;
  (&x151)[i] = 1;
  (&x152)[i] = 1;
  (&x153)[i] = 1;
  (&x154)[i] = 1;
  (&x155)[i] = 1;
  (&x156)[i] = 1;
  (&x157)[i] = 1;
  (&x158)[i] = 1;
  (&x159)[i] = 1;
  (&x160)[i] = 1;
  (&x161)[i] = 1;
  (&x162)[i] = 1;
  (&x163)[i] = 1;
  (&x164)[i] = 1;
  (&x165)[i] = 1;
  (&x166)[i] = 1;
  (&x167)[i] = 1;
  (&x168)[i] = 1;
  (&x169)[i] = 1;
  (&x170)[i] = 1;
  (&x171)[i] = 1;
  (&x172)[i] = 1;
  (&x173)[i] = 1;
  (&x174)[i] = 1;
  (&x175)[i] = 1;
  (&x176)[i] = 1;
  (&x177)[i] = 1;
  (&x178)[i] = 1;
  (&x179)[i] = 1;
  (&x180)[i] = 1;
  (&x181)[i] = 1;
  (&x182)[i] = 1;
  (&x183)[i] = 1;
  (&x184)[i] = 1;
  (&x185)[i] = 1;
  (&x186)[i] = 1;
  (&x187)[i] = 1;
  (&x188)[i] = 1;
  (&x189)[i] = 1;
  (&x190)[i] = 1;
  (&x191)[i] = 1;
  (&x192)[i] = 1;
  (&x193)[i] = 1;
  (&x194)[i] = 1;
  (&x195)[i] = 1;
  (&x196)[i] = 1;
  (&x197)[i] = 1;
  (&x198)[i] = 1;
  (&x199)[i] = 1;
  (&x200)[i] = 1;
  (&x201)[i] = 1;
  (&x202)[i] = 1;
  (&x203)[i] = 1;
  (&x204)[i] = 1;
  (&x205)[i] = 1;
  (&x206)[i] = 1;
  (&x207)[i] = 1;
  (&x208)[i] = 1;
  (&x209)[i] = 1;
  (&x210)[i] = 1;
  (&x211)[i] = 1;
  (&x212)[i] = 1;
  (&x213)[i] = 1;
  (&x214)[i] = 1;
  (&x215)[i] = 1;
  (&x216)[i] = 1;
  (&x217)[i] = 1;
  (&x218)[i] = 1;
  (&x219)[i] = 1;
  (&x220)[i] = 1;
  (&x221)[i] = 1;
  (&x222)[i] = 1;
  (&x223)[i] = 1;
  (&x224)[i] = 1;
  (&x225)[i] = 1;
  (&x226)[i] = 1;
  (&x227)[i] = 1;
  (&x228)[i] = 1;
  (&x229)[i] = 1;
  (&x230)[i] = 1;
  (&x231)[i] = 1;
  (&x232)[i] = 1;
  (&x233)[i] = 1;
  (&x234)[i] = 1;
  (&x235)[i] = 1;
  (&x236)[i] = 1;
  (&x237)[i] = 1;
  (&x238)[i] = 1;
  (&x239)[i] = 1;
  (&x240)[i] = 1;
  (&x241)[i] = 1;
  (&x242)[i] = 1;
  (&x243)[i] = 1;
  (&x244)[i] = 1;
  (&x245)[i] = 1;
  (&x246)[i] = 1;
  (&x247)[i] = 1;
  (&x248)[i] = 1;
  (&x249)[i] = 1;
  (&x250)[i] = 1;
  (&x251)[i] = 1;
  (&x252)[i] = 1;
  (&x253)[i] = 1;
  (&x254)[i] = 1;
  (&x255)[i] = 1;
  (&x256)[i] = 1;

  return 0;
}
