/*
* Test case inputs for: long double __floattitf (__int128_t)
* Conversion from 128 bit integer to long double (IBM double-double).
*/

#define INIT_U128(HI, LO) (((__uint128_t) (HI) << 64) | (LO))

struct testCase {
  __int128_t input128;
  double hi;
  double lo;
};

struct testCase tests[] = {
  { INIT_U128( 0x0000000000000000, 0x0000000000000000 ), 0x0p+0, 0x0p+0 },
  { INIT_U128( 0x0000000000000000, 0x0000000000000001 ), 0x1p+0, 0x0p+0 },
  { INIT_U128( 0x0000000000000000, 0x1000000000000000 ), 0x1p+60, 0x0p+0 },
  { INIT_U128( 0x0000000000000000, 0xffffffffffffffff ), 0x1p+64, -0x1p+0 },
  { INIT_U128( 0x0000000000000000, 0x7fffffffffffffff ), 0x1p+63, -0x1p+0 },
  { INIT_U128( 0x0000000000000001, 0x0000000000000000 ), 0x1p+64, 0x0p+0 },
  { INIT_U128( 0x0000000000000001, 0x0000000000000001 ), 0x1p+64, 0x1p+0 },
  { INIT_U128( 0x0000000000000001, 0x7fffffffffffffff ), 0x1.8p+64, -0x1p+0 },
  { INIT_U128( 0x0000000000000001, 0x1000000000000000 ), 0x1.1p+64, 0x0p+0 },
  { INIT_U128( 0x0000000000000001, 0xffffffffffffffff ), 0x1p+65, -0x1p+0 },
  { INIT_U128( 0x7fffffffffffffff, 0x0000000000000000 ), 0x1p+127, -0x1p+64 },
  { INIT_U128( 0x7fffffffffffffff, 0x0000000000000001 ), 0x1p+127, -0x1p+64 },
  { INIT_U128( 0x7fffffffffffffff, 0x7fffffffffffffff ), 0x1p+127, -0x1p+63 },
  { INIT_U128( 0x7fffffffffffffff, 0x1000000000000000 ), 0x1p+127, -0x1.ep+63 },
  { INIT_U128( 0x7fffffffffffffff, 0xffffffffffffffff ), 0x1p+127, -0x1p+0 },
  { INIT_U128( 0x1000000000000000, 0x0000000000000000 ), 0x1p+124, 0x0p+0 },
  { INIT_U128( 0x1000000000000000, 0x0000000000000001 ), 0x1p+124, 0x1p+0 },
  { INIT_U128( 0x1000000000000000, 0x7fffffffffffffff ), 0x1p+124, 0x1p+63 },
  { INIT_U128( 0x1000000000000000, 0x1000000000000000 ), 0x1p+124, 0x1p+60 },
  { INIT_U128( 0x1000000000000000, 0xffffffffffffffff ), 0x1p+124, 0x1p+64 },
  { INIT_U128( 0xffffffffffffffff, 0x0000000000000000 ), -0x1p+64, 0x0p+0 },
  { INIT_U128( 0xffffffffffffffff, 0x0000000000000001 ), -0x1p+64, 0x1p+0 },
  { INIT_U128( 0xffffffffffffffff, 0x7fffffffffffffff ), -0x1p+63, -0x1p+0 },
  { INIT_U128( 0xffffffffffffffff, 0x1000000000000000 ), -0x1.ep+63, 0x0p+0 },
  { INIT_U128( 0xffffffffffffffff, 0xffffffffffffffff ), -0x1p+0, 0x0p+0 },
  { INIT_U128( 0x0000000000000061, 0x0000000000000061 ), 0x1.84p+70, 0x1.84p+6 },
  { INIT_U128( 0x0000000000000000, 0x0000000000000057 ), 0x1.5cp+6, 0x0p+0 },
  { INIT_U128( 0x0000000000000000, 0xffffffffffffff9e ), 0x1p+64, -0x1.88p+6 },
  { INIT_U128( 0x0000000000000000, 0x0000000000000062 ), 0x1.88p+6, 0x0p+0 },
  { INIT_U128( 0x0000000000000000, 0x0000000000000403 ), 0x1.00cp+10, 0x0p+0 },
  { INIT_U128( 0x0000000000000000, 0xfffffffffffffbfc ), 0x1.fffffffffffffp+63, 0x1.fep+9 },
  { INIT_U128( 0x0000000000000000, 0x000000000000040a ), 0x1.028p+10, 0x0p+0 },
  { INIT_U128( 0x0000000000000000, 0x000000000000040c ), 0x1.03p+10, 0x0p+0 },
  { INIT_U128( 0x0000000000000000, 0x000000000000041a ), 0x1.068p+10, 0x0p+0 },
  { INIT_U128( 0x0000000000000000, 0x0000000000000510 ), 0x1.44p+10, 0x0p+0 },
  { INIT_U128( 0x0000000000000000, 0x0000000000000518 ), 0x1.46p+10, 0x0p+0 },
  { INIT_U128( 0x0000000000000000, 0xfffffffffffffae7 ), 0x1.fffffffffffffp+63, 0x1.738p+9 },
  { INIT_U128( 0x0000000000000000, 0x0000000000000602 ), 0x1.808p+10, 0x0p+0 },
  { INIT_U128( 0x0000000000000000, 0xfffffffffffff9fd ), 0x1.fffffffffffffp+63, 0x1.fdp+8 },
  { INIT_U128( 0x0000000000000000, 0x0000000000002090 ), 0x1.048p+13, 0x0p+0 },
  { INIT_U128( 0x0000000000000000, 0xffffffffffffdbdb ), 0x1.ffffffffffffbp+63, 0x1.ed8p+9 },
  { INIT_U128( 0x0000000000000000, 0x0000000000002430 ), 0x1.218p+13, 0x0p+0 },
  { INIT_U128( 0x0000000000000000, 0x0000000000020202 ), 0x1.0101p+17, 0x0p+0 },
  { INIT_U128( 0x0000000000000000, 0xfffffffffffdeef7 ), 0x1.fffffffffffbep+63, -0x1.09p+8 },
  { INIT_U128( 0x0000000000000000, 0x0000000000032004 ), 0x1.9002p+17, 0x0p+0 },
  { INIT_U128( 0x0000000000000000, 0xfffffffffffcdffb ), 0x1.fffffffffff9cp+63, -0x1.4p+2 },
  { INIT_U128( 0x0000000000000000, 0x0000000000032040 ), 0x1.902p+17, 0x0p+0 },
  { INIT_U128( 0x0000000000000000, 0xffffffffffbfdfbb ), 0x1.ffffffffff7fcp+63, -0x1.14p+6 },
  { INIT_U128( 0x0000000000000000, 0x0000000000402088 ), 0x1.00822p+22, 0x0p+0 },
  { INIT_U128( 0x0000000000000000, 0x0000000080080088 ), 0x1.0010011p+31, 0x0p+0 },
  { INIT_U128( 0xffffffff7ff7ff77, 0xffffffff7ff7ff77 ), -0x1.0010011p+95, -0x1.00100112p+31 },
  { INIT_U128( 0x0000000000000000, 0x0000000850000008 ), 0x1.0a000001p+35, 0x0p+0 },
  { INIT_U128( 0x0000000000000000, 0x0000002000000448 ), 0x1.000000224p+37, 0x0p+0 },
  { INIT_U128( 0x0000000000000000, 0xffffffdfff7dbfff ), 0x1.ffffffbffefb8p+63, -0x1p+0 },
  { INIT_U128( 0x0000004002001100, 0x0000004002001100 ), 0x1.00080044p+102, 0x1.00080044p+38 },
  { INIT_U128( 0x000008020000000c, 0x000008020000000c ), 0x1.00400000018p+107, 0x1.00400000018p+43 },
  { INIT_U128( 0x0000000000000000, 0xfffff7fdfffffff3 ), 0x1.ffffeffcp+63, -0x1.ap+3 },
  { INIT_U128( 0x0000000000000000, 0x0000800000000824 ), 0x1.000000001048p+47, 0x0p+0 },
  { INIT_U128( 0x0000000000000000, 0xfffdffffff7ffcff ), 0x1.fffbffffffp+63, -0x1.808p+9 },
  { INIT_U128( 0x0000000000000000, 0x4002040010000000 ), 0x1.000810004p+62, 0x0p+0 },
  { INIT_U128( 0x0000000000000000, 0xbffdfbffefffffff ), 0x1.7ffbf7ffep+63, -0x1p+0 },
  { INIT_U128( 0x0000000000000000, 0x8000000000000000 ), 0x1p+63, 0x0p+0 },
  { INIT_U128( 0x0000000000000000, 0xfffffffff418c5c1 ), 0x1.ffffffffe8319p+63, -0x1.1f8p+9 },
  { INIT_U128( 0x000000000000000f, 0xffffffffffffffff ), 0x1p+68, -0x1p+0 },
  { INIT_U128( 0x00000000000000ff, 0xffffffffffffffff ), 0x1p+72, -0x1p+0 },
  { INIT_U128( 0x0000000000000fff, 0xffffffffffffffff ), 0x1p+76, -0x1p+0 },
  { INIT_U128( 0x000000000000ffff, 0xffffffffffffffff ), 0x1p+80, -0x1p+0 },
  { INIT_U128( 0x00000000000fffff, 0xffffffffffffffff ), 0x1p+84, -0x1p+0 },
  { INIT_U128( 0x0000000000ffffff, 0xffffffffffffffff ), 0x1p+88, -0x1p+0 },
  { INIT_U128( 0x000000000fffffff, 0xffffffffffffffff ), 0x1p+92, -0x1p+0 },
  { INIT_U128( 0x00000000ffffffff, 0xffffffffffffffff ), 0x1p+96, -0x1p+0 },
  { INIT_U128( 0x0000000fffffffff, 0xffffffffffffffff ), 0x1p+100, -0x1p+0 },
  { INIT_U128( 0x000000ffffffffff, 0xffffffffffffffff ), 0x1p+104, -0x1p+0 },
  { INIT_U128( 0x00000fffffffffff, 0xffffffffffffffff ), 0x1p+108, -0x1p+0 },
  { INIT_U128( 0x0000ffffffffffff, 0xffffffffffffffff ), 0x1p+112, -0x1p+0 },
  { INIT_U128( 0x000fffffffffffff, 0xffffffffffffffff ), 0x1p+116, -0x1p+0 },
  { INIT_U128( 0x00ffffffffffffff, 0xffffffffffffffff ), 0x1p+120, -0x1p+0 },
  { INIT_U128( 0x0fffffffffffffff, 0xffffffffffffffff ), 0x1p+124, -0x1p+0 },
  { INIT_U128( 0x1000000000000000, 0x0000000000000000 ), 0x1p+124, 0x0p+0 },
  { INIT_U128( 0x1000000000000000, 0x0000000000000011 ), 0x1p+124, 0x1.1p+4 },
  { INIT_U128( 0x1000000000000000, 0x0000000000000111 ), 0x1p+124, 0x1.11p+8 },
  { INIT_U128( 0x1000000000000000, 0x0000000000001111 ), 0x1p+124, 0x1.111p+12 },
  { INIT_U128( 0x1000000000000000, 0x0000000000011111 ), 0x1p+124, 0x1.1111p+16 },
  { INIT_U128( 0x1000000000000000, 0x0000000000111111 ), 0x1p+124, 0x1.11111p+20 },
  { INIT_U128( 0x1000000000000000, 0x0000000001111111 ), 0x1p+124, 0x1.111111p+24 },
  { INIT_U128( 0x1000000000000000, 0x0000000011111111 ), 0x1p+124, 0x1.1111111p+28 },
  { INIT_U128( 0x1000000000000000, 0x0000000111111111 ), 0x1p+124, 0x1.11111111p+32 },
  { INIT_U128( 0x1000000000000000, 0x0000001111111111 ), 0x1p+124, 0x1.111111111p+36 },
  { INIT_U128( 0x1000000000000000, 0x0000011111111111 ), 0x1p+124, 0x1.1111111111p+40 },
  { INIT_U128( 0x1000000000000000, 0x0000111111111111 ), 0x1p+124, 0x1.11111111111p+44 },
  { INIT_U128( 0x1000000000000000, 0x0001111111111111 ), 0x1p+124, 0x1.111111111111p+48 },
  { INIT_U128( 0x1000000000000000, 0x0011111111111111 ), 0x1p+124, 0x1.1111111111111p+52 },
  { INIT_U128( 0x1000000000000000, 0x0111111111111111 ), 0x1p+124, 0x1.11111111111111p+56 },
  { INIT_U128( 0x1000000000000000, 0x1111111111111111 ), 0x1p+124, 0x1.111111111111111p+60 },
  { INIT_U128( 0xffffffffffffffff, 0xf000000000000000 ), -0x1p+60, 0x0p+0 },
  { INIT_U128( 0xffffffffffffffff, 0xff00000000000000 ), -0x1p+56, 0x0p+0 },
  { INIT_U128( 0x0000000000000000, 0xb7ffffff77ffffff ), 0x1.6ffffffefp+63, -0x1p+0 },
  { INIT_U128( 0xb7ffffff3fffffff, 0xb7ffffff3fffffff ), -0x1.20000003p+126, -0x1.20000003p+62 },
  { INIT_U128( 0xb7fffffefffffffe, 0xb7fffffefffffffe ), -0x1.20000004p+126, -0x1.48000001p+64 },
  { INIT_U128( 0xb7fffffeffffefff, 0xb7fffffeffffefff ), -0x1.2000000400004p+126, -0x1.2000000400004p+62 },
  { INIT_U128( 0x0000040000000000, 0x0000000000000000 ), 0x1p+106, 0x0p+0 },
  { INIT_U128( 0x0000000000000000, 0x000000003ff00000 ), 0x1.ff8p+29, 0x0p+0 },
  { INIT_U128( 0x0000000000000000, 0xb7fbffffffffdfff ), 0x1.6ff7ffffffffcp+63, -0x1p+0 },
  { INIT_U128( 0x0000000000000000, 0x4804000000010000 ), 0x1.201000000004p+62, 0x0p+0 },
  { INIT_U128( 0x0000000000000000, 0x4900000000001000 ), 0x1.2400000000004p+62, 0x0p+0 },
  { INIT_U128( 0x0000000000000000, 0xb6ffffffffffefff ), 0x1.6dffffffffffep+63, -0x1p+0 },
  { INIT_U128( 0x0000000000000000, 0x4900000000008000 ), 0x1.240000000002p+62, 0x0p+0 },
  { INIT_U128( 0xb6ffffffffff7fff, 0xb6ffffffffff7fff ), -0x1.240000000002p+126, -0x1.240000000002p+62 },
  { INIT_U128( 0x4900000000040000, 0x4900000000040000 ), 0x1.24000000001p+126, 0x1.24000000001p+62 },
  { INIT_U128( 0xb6fffffffffbffff, 0xb6fffffffffbffff ), -0x1.24000000001p+126, -0x1.24000000001p+62 },
  { INIT_U128( 0x4900000000200000, 0x4900000000200000 ), 0x1.24000000008p+126, 0x1.24000000008p+62 },
  { INIT_U128( 0xb6ffffffffdfffff, 0xb6ffffffffdfffff ), -0x1.24000000008p+126, -0x1.24000000008p+62 },
  { INIT_U128( 0x4900000000800000, 0x4900000000800000 ), 0x1.2400000002p+126, 0x1.2400000002p+62 },
  { INIT_U128( 0xb6ffffffff7fffff, 0xb6ffffffff7fffff ), -0x1.2400000002p+126, -0x1.2400000002p+62 },
  { INIT_U128( 0x4900000008000000, 0x4900000008000000 ), 0x1.240000002p+126, 0x1.240000002p+62 },
  { INIT_U128( 0xb6fffffff7ffffff, 0xb6fffffff7ffffff ), -0x1.240000002p+126, -0x1.240000002p+62 },
  { INIT_U128( 0x4900000040000000, 0x4900000040000000 ), 0x1.24000001p+126, 0x1.24000001p+62 },
  { INIT_U128( 0xb6ffffffbfffffff, 0xb6ffffffbfffffff ), -0x1.24000001p+126, -0x1.24000001p+62 },
  { INIT_U128( 0x4900000080000000, 0x4900000080000000 ), 0x1.24000002p+126, 0x1.24000002p+62 },
  { INIT_U128( 0xb6ffffff7fffffff, 0xb6ffffff7fffffff ), -0x1.24000002p+126, -0x1.24000002p+62 },
  { INIT_U128( 0x4900000400000000, 0x4900000400000000 ), 0x1.2400001p+126, 0x1.2400001p+62 },
  { INIT_U128( 0xb6fffffbffffffff, 0xb6fffffbffffffff ), -0x1.2400001p+126, -0x1.2400001p+62 },
  { INIT_U128( 0x4900001000000000, 0x4900001000000000 ), 0x1.2400004p+126, 0x1.2400004p+62 },
  { INIT_U128( 0xb6ffffefffffffff, 0xb6ffffefffffffff ), -0x1.2400004p+126, -0x1.2400004p+62 },
  { INIT_U128( 0x4900004000000000, 0x4900004000000000 ), 0x1.240001p+126, 0x1.240001p+62 },
  { INIT_U128( 0xb6ffffbfffffffff, 0xb6ffffbfffffffff ), -0x1.240001p+126, -0x1.240001p+62 },
  { INIT_U128( 0x4900040000000000, 0x4900040000000000 ), 0x1.24001p+126, 0x1.24001p+62 },
  { INIT_U128( 0xb6fffbffffffffff, 0xb6fffbffffffffff ), -0x1.24001p+126, -0x1.24001p+62 },
  { INIT_U128( 0x4900200000000000, 0x4900200000000000 ), 0x1.24008p+126, 0x1.24008p+62 },
  { INIT_U128( 0xb6ffdfffffffffff, 0xb6ffdfffffffffff ), -0x1.24008p+126, -0x1.24008p+62 },
  { INIT_U128( 0x4901000000000000, 0x4901000000000000 ), 0x1.2404p+126, 0x1.2404p+62 },
  { INIT_U128( 0xb6feffffffffffff, 0xb6feffffffffffff ), -0x1.2404p+126, -0x1.2404p+62 },
  { INIT_U128( 0x4910000000000000, 0x4910000000000000 ), 0x1.244p+126, 0x1.244p+62 },
  { INIT_U128( 0xb6efffffffffffff, 0xb6efffffffffffff ), -0x1.244p+126, -0x1.244p+62 },
  { INIT_U128( 0x4980000000000000, 0x4980000000000000 ), 0x1.26p+126, 0x1.26p+62 },
  { INIT_U128( 0xb67fffffffffffff, 0xb67fffffffffffff ), -0x1.26p+126, -0x1.26p+62 },
  { INIT_U128( 0x4c00000000000000, 0x4c00000000000000 ), 0x1.3p+126, 0x1.3p+62 },
  { INIT_U128( 0xb3ffffffffffffff, 0xb3ffffffffffffff ), -0x1.3p+126, -0x1.3p+62 },
  { INIT_U128( 0x9fffffffffffbfff, 0x9fffffffffffbfff ), -0x1.800000000001p+126, -0x1.800000000001p+62 },
  { INIT_U128( 0x6000000000004001, 0x6000000000004001 ), 0x1.800000000001p+126, 0x1.6000000000004p+64 },
  { INIT_U128( 0x8fffffffffffffbf, 0x8fffffffffffffbf ), -0x1.cp+126, -0x1.01cp+70 },
  { INIT_U128( 0x7000000000000080, 0x7000000000000080 ), 0x1.cp+126, 0x1.00ep+71 },
  { INIT_U128( 0x8fffffffffffff7f, 0x8fffffffffffff7f ), -0x1.cp+126, -0x1.00ep+71 },
  { INIT_U128( 0x7000000000000800, 0x7000000000000800 ), 0x1.c000000000002p+126, 0x1.c000000000002p+62 },
  { INIT_U128( 0x8ffffffffffff7ff, 0x8ffffffffffff7ff ), -0x1.c000000000002p+126, -0x1.c000000000002p+62 },
  { INIT_U128( 0x7000000000002000, 0x7000000000002000 ), 0x1.c000000000008p+126, 0x1.c000000000008p+62 },
  { INIT_U128( 0x8fffffffffffdfff, 0x8fffffffffffdfff ), -0x1.c000000000008p+126, -0x1.c000000000008p+62 },
  { INIT_U128( 0x7000000000010000, 0x7000000000010000 ), 0x1.c00000000004p+126, 0x1.c00000000004p+62 },
  { INIT_U128( 0x8ffffffffffeffff, 0x8ffffffffffeffff ), -0x1.c00000000004p+126, -0x1.c00000000004p+62 },
  { INIT_U128( 0x7000000000040000, 0x7000000000040000 ), 0x1.c0000000001p+126, 0x1.c0000000001p+62 },
  { INIT_U128( 0x8ffffffffffbffff, 0x8ffffffffffbffff ), -0x1.c0000000001p+126, -0x1.c0000000001p+62 },
  { INIT_U128( 0x7000000000080000, 0x7000000000080000 ), 0x1.c0000000002p+126, 0x1.c0000000002p+62 },
  { INIT_U128( 0x8ffffffffff7ffff, 0x8ffffffffff7ffff ), -0x1.c0000000002p+126, -0x1.c0000000002p+62 },
  { INIT_U128( 0x7000000000800000, 0x7000000000800000 ), 0x1.c000000002p+126, 0x1.c000000002p+62 },
  { INIT_U128( 0x8fffffffff7fffff, 0x8fffffffff7fffff ), -0x1.c000000002p+126, -0x1.c000000002p+62 },
  { INIT_U128( 0x7000000008000000, 0x7000000008000000 ), 0x1.c00000002p+126, 0x1.c00000002p+62 },
  { INIT_U128( 0x8ffffffff7ffffff, 0x8ffffffff7ffffff ), -0x1.c00000002p+126, -0x1.c00000002p+62 },
  { INIT_U128( 0x7000000010000000, 0x7000000010000000 ), 0x1.c00000004p+126, 0x1.c00000004p+62 },
  { INIT_U128( 0x8fffffffefffffff, 0x8fffffffefffffff ), -0x1.c00000004p+126, -0x1.c00000004p+62 },
  { INIT_U128( 0x7000000100000000, 0x7000000100000000 ), 0x1.c0000004p+126, 0x1.c0000004p+62 },
  { INIT_U128( 0x8ffffffeffffffff, 0x8ffffffeffffffff ), -0x1.c0000004p+126, -0x1.c0000004p+62 },
  { INIT_U128( 0x7000000200000000, 0x7000000200000000 ), 0x1.c0000008p+126, 0x1.c0000008p+62 },
  { INIT_U128( 0x8ffffffdffffffff, 0x8ffffffdffffffff ), -0x1.c0000008p+126, -0x1.c0000008p+62 },
  { INIT_U128( 0x7000000800000000, 0x7000000800000000 ), 0x1.c000002p+126, 0x1.c000002p+62 },
  { INIT_U128( 0x8ffffff7ffffffff, 0x8ffffff7ffffffff ), -0x1.c000002p+126, -0x1.c000002p+62 },
  { INIT_U128( 0x7000008000000000, 0x7000008000000000 ), 0x1.c00002p+126, 0x1.c00002p+62 },
  { INIT_U128( 0x8fffff7fffffffff, 0x8fffff7fffffffff ), -0x1.c00002p+126, -0x1.c00002p+62 },
  { INIT_U128( 0x7000010000000000, 0x7000010000000000 ), 0x1.c00004p+126, 0x1.c00004p+62 },
  { INIT_U128( 0x8ffffeffffffffff, 0x8ffffeffffffffff ), -0x1.c00004p+126, -0x1.c00004p+62 },
  { INIT_U128( 0x7000080000000000, 0x7000080000000000 ), 0x1.c0002p+126, 0x1.c0002p+62 },
  { INIT_U128( 0x8ffff7ffffffffff, 0x8ffff7ffffffffff ), -0x1.c0002p+126, -0x1.c0002p+62 },
  { INIT_U128( 0x7000800000000000, 0x7000800000000000 ), 0x1.c002p+126, 0x1.c002p+62 },
  { INIT_U128( 0x8fff7fffffffffff, 0x8fff7fffffffffff ), -0x1.c002p+126, -0x1.c002p+62 },
  { INIT_U128( 0x7002000000000000, 0x7002000000000000 ), 0x1.c008p+126, 0x1.c008p+62 },
  { INIT_U128( 0x8ffdffffffffffff, 0x8ffdffffffffffff ), -0x1.c008p+126, -0x1.c008p+62 },
  { INIT_U128( 0x7008000000000000, 0x7008000000000000 ), 0x1.c02p+126, 0x1.c02p+62 },
  { INIT_U128( 0x8ff7ffffffffffff, 0x8ff7ffffffffffff ), -0x1.c02p+126, -0x1.c02p+62 },
  { INIT_U128( 0x7080000000000000, 0x7080000000000000 ), 0x1.c2p+126, 0x1.c2p+62 },
  { INIT_U128( 0x8f7fffffffffffff, 0x8f7fffffffffffff ), -0x1.c2p+126, -0x1.c2p+62 },
  { INIT_U128( 0x7400000000000000, 0x7400000000000000 ), 0x1.dp+126, 0x1.dp+62 }
};

static const int numTests = sizeof(tests) / sizeof(struct testCase);
