#if 0
//
// Generated by Microsoft (R) HLSL Shader Compiler 6.3.9600.16384
//
//
//
// Input signature:
//
// Name                 Index   Mask Register SysValue  Format   Used
// -------------------- ----- ------ -------- -------- ------- ------
// SV_VertexID              0   x           0   VERTID    uint   x   
//
//
// Output signature:
//
// Name                 Index   Mask Register SysValue  Format   Used
// -------------------- ----- ------ -------- -------- ------- ------
// SV_Position              0   xyzw        0      POS   float   xyzw
// TEXCOORD                 0   xy          1     NONE   float   xy  
//
vs_4_1
dcl_globalFlags refactoringAllowed
dcl_immediateConstantBuffer { { -1.000000, 1.000000, 0, 0},
                              { 1.000000, -1.000000, 0, 0},
                              { -1.000000, -1.000000, 0, 0},
                              { -1.000000, 1.000000, 0, 0},
                              { 1.000000, 1.000000, 0, 0},
                              { 1.000000, -1.000000, 0, 0} }
dcl_input_sgv v0.x, vertex_id
dcl_output_siv o0.xyzw, position
dcl_output o1.xy
dcl_temps 1
mov o0.zw, l(0,0,0,1.000000)
mov r0.x, v0.x
mov o0.xy, icb[r0.x + 0].xyxx
add r0.y, l(1.000000), icb[r0.x + 0].x
add r0.x, l(1.000000), -icb[r0.x + 0].y
mul o1.xy, r0.yxyy, l(0.500000, 0.500000, 0.000000, 0.000000)
ret 
// Approximately 7 instruction slots used
#endif

const BYTE g_VS_ResolveDepthStencil[] = {
    68,  88,  66,  67,  205, 15,  103, 70,  202, 235, 195, 98,  255, 82,  84,  239, 130, 6,   12,
    104, 1,   0,   0,   0,   0,   3,   0,   0,   5,   0,   0,   0,   52,  0,   0,   0,   140, 0,
    0,   0,   192, 0,   0,   0,   24,  1,   0,   0,   132, 2,   0,   0,   82,  68,  69,  70,  80,
    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   28,  0,   0,   0,
    1,   4,   254, 255, 0,   1,   0,   0,   28,  0,   0,   0,   77,  105, 99,  114, 111, 115, 111,
    102, 116, 32,  40,  82,  41,  32,  72,  76,  83,  76,  32,  83,  104, 97,  100, 101, 114, 32,
    67,  111, 109, 112, 105, 108, 101, 114, 32,  54,  46,  51,  46,  57,  54,  48,  48,  46,  49,
    54,  51,  56,  52,  0,   171, 171, 73,  83,  71,  78,  44,  0,   0,   0,   1,   0,   0,   0,
    8,   0,   0,   0,   32,  0,   0,   0,   0,   0,   0,   0,   6,   0,   0,   0,   1,   0,   0,
    0,   0,   0,   0,   0,   1,   1,   0,   0,   83,  86,  95,  86,  101, 114, 116, 101, 120, 73,
    68,  0,   79,  83,  71,  78,  80,  0,   0,   0,   2,   0,   0,   0,   8,   0,   0,   0,   56,
    0,   0,   0,   0,   0,   0,   0,   1,   0,   0,   0,   3,   0,   0,   0,   0,   0,   0,   0,
    15,  0,   0,   0,   68,  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   3,   0,   0,
    0,   1,   0,   0,   0,   3,   12,  0,   0,   83,  86,  95,  80,  111, 115, 105, 116, 105, 111,
    110, 0,   84,  69,  88,  67,  79,  79,  82,  68,  0,   171, 171, 171, 83,  72,  68,  82,  100,
    1,   0,   0,   65,  0,   1,   0,   89,  0,   0,   0,   106, 8,   0,   1,   53,  24,  0,   0,
    26,  0,   0,   0,   0,   0,   128, 191, 0,   0,   128, 63,  0,   0,   0,   0,   0,   0,   0,
    0,   0,   0,   128, 63,  0,   0,   128, 191, 0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
    128, 191, 0,   0,   128, 191, 0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   128, 191, 0,
    0,   128, 63,  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   128, 63,  0,   0,   128, 63,
    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   128, 63,  0,   0,   128, 191, 0,   0,   0,
    0,   0,   0,   0,   0,   96,  0,   0,   4,   18,  16,  16,  0,   0,   0,   0,   0,   6,   0,
    0,   0,   103, 0,   0,   4,   242, 32,  16,  0,   0,   0,   0,   0,   1,   0,   0,   0,   101,
    0,   0,   3,   50,  32,  16,  0,   1,   0,   0,   0,   104, 0,   0,   2,   1,   0,   0,   0,
    54,  0,   0,   8,   194, 32,  16,  0,   0,   0,   0,   0,   2,   64,  0,   0,   0,   0,   0,
    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   128, 63,  54,  0,   0,   5,   18,  0,
    16,  0,   0,   0,   0,   0,   10,  16,  16,  0,   0,   0,   0,   0,   54,  0,   0,   6,   50,
    32,  16,  0,   0,   0,   0,   0,   70,  144, 144, 0,   10,  0,   16,  0,   0,   0,   0,   0,
    0,   0,   0,   8,   34,  0,   16,  0,   0,   0,   0,   0,   1,   64,  0,   0,   0,   0,   128,
    63,  10,  144, 144, 0,   10,  0,   16,  0,   0,   0,   0,   0,   0,   0,   0,   9,   18,  0,
    16,  0,   0,   0,   0,   0,   1,   64,  0,   0,   0,   0,   128, 63,  26,  144, 144, 128, 65,
    0,   0,   0,   10,  0,   16,  0,   0,   0,   0,   0,   56,  0,   0,   10,  50,  32,  16,  0,
    1,   0,   0,   0,   22,  5,   16,  0,   0,   0,   0,   0,   2,   64,  0,   0,   0,   0,   0,
    63,  0,   0,   0,   63,  0,   0,   0,   0,   0,   0,   0,   0,   62,  0,   0,   1,   83,  84,
    65,  84,  116, 0,   0,   0,   7,   0,   0,   0,   1,   0,   0,   0,   6,   0,   0,   0,   3,
    0,   0,   0,   3,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   1,   0,   0,   0,
    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
    0,   0,   0,   0,   0,   0,   3,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
    0,   0,   0,   0,   0,   0,   0,   0};
