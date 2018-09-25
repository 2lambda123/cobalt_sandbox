# Copyright 2018 The Cobalt Authors. All Rights Reserved.
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

{
  'conditions': [
    ['target_arch == "x64"', {
       'variables': {
          'musl_arch': 'x86_64'
        }
    }],
    ['target_arch == "x86" or target_arch == "ia32"', {
      'variables': {
         'musl_arch': 'i386'
       }
    }],
    ['target_arch == "arm"', {
      'variables': {
         'musl_arch': 'arm'
       }
    }],
    ['target_arch == "arm64"', {
      'variables': {
         'musl_arch': 'aarch64'
       }
    }],
    ['target_arch == "mips"', {
      'variables': {
         'musl_arch': 'mips'
       }
    }],
    # Not yet supported:
    # target_arch == ps3
    # target_arch == win
  ],
  'targets': [
    {
      'target_name': 'c',
      'type': 'static_library',
      'include_dirs': [
        'src/internal',
        'arch/<(musl_arch)/',
      ],
      'cflags': [
        # no-unknown-pragmas  -w shift-op-parentheses
        '-nobuiltininc',
        '-isystem<(DEPTH)/third_party/musl/include',
        '-isystem<(DEPTH)/third_party/musl/arch/x86_64',
        '-isystem<(DEPTH)/third_party/musl/arch/generic',
        '-isystem<(DEPTH)/third_party/musl/generated/include',
        '-w',
      ],
      'direct_dependent_settings': {
        'include_dirs': [
          'musl/include',
          'arch/<(musl_arch)/',
          'arch/generic/',
       ],
      },
      'defines' : [
        # Enables temporary situation where main cobalt is built with
        # glibc headers but then linked with some musl components.
        # Expected to go away when main cobalt is also build with
        # stand-alone libc++.
        'COBALT_MUSL_W_GLIBC_HEADERS'
      ],
      'conditions': [
        ['musl_arch in ["i386", "x86_64", "aarch64"]', {
          'sources': [
            'src/fenv/<(musl_arch)/fenv.s'
          ],
        }],
        ['musl_arch == "arm"', {
          'sources': [
            'src/fenv/arm/fenv.c',
            'src/fenv/arm/fenv-hf.S'
          ],
        }],
        ['musl_arch == "mips"', {
          'sources': [
            'src/fenv/mips/fenv.Sa,'
            'src/fenv/mips/fenv-sf.c'
          ],
        }]
      ],
      'sources': [
        'src/fenv/fegetexceptflag.c',
        'src/fenv/feholdexcept.c',
        'src/fenv/fesetexceptflag.c',
        'src/fenv/fesetround.c',
        'src/fenv/feupdateenv.c',
        'src/fenv/__flt_rounds.c',
        'src/math/acos.c',
        'src/math/acosf.c',
        'src/math/acosh.c',
        'src/math/acoshf.c',
        'src/math/acoshl.c',
        'src/math/acosl.c',
        'src/math/asin.c',
        'src/math/asinf.c',
        'src/math/asinh.c',
        'src/math/asinhf.c',
        'src/math/asinhl.c',
        'src/math/asinl.c',
        'src/math/atan2.c',
        'src/math/atan2f.c',
        'src/math/atan2l.c',
        'src/math/atan.c',
        'src/math/atanf.c',
        'src/math/atanh.c',
        'src/math/atanhf.c',
        'src/math/atanhl.c',
        'src/math/atanl.c',
        'src/math/cbrt.c',
        'src/math/cbrtf.c',
        'src/math/cbrtl.c',
        'src/math/ceil.c',
        'src/math/ceilf.c',
        'src/math/ceill.c',
        'src/math/copysign.c',
        'src/math/copysignf.c',
        'src/math/copysignl.c',
        'src/math/__cos.c',
        'src/math/cos.c',
        'src/math/__cosdf.c',
        'src/math/cosf.c',
        'src/math/cosh.c',
        'src/math/coshf.c',
        'src/math/coshl.c',
        'src/math/__cosl.c',
        'src/math/cosl.c',
        'src/math/erf.c',
        'src/math/erff.c',
        'src/math/erfl.c',
        'src/math/exp10.c',
        'src/math/exp10f.c',
        'src/math/exp10l.c',
        'src/math/exp2.c',
        'src/math/exp2f.c',
        'src/math/exp2l.c',
        'src/math/exp.c',
        'src/math/expf.c',
        'src/math/expl.c',
        'src/math/expm1.c',
        'src/math/expm1f.c',
        'src/math/expm1l.c',
        'src/math/__expo2.c',
        'src/math/__expo2f.c',
        'src/math/fabs.c',
        'src/math/fabsf.c',
        'src/math/fabsl.c',
        'src/math/fdim.c',
        'src/math/fdimf.c',
        'src/math/fdiml.c',
        'src/math/finite.c',
        'src/math/finitef.c',
        'src/math/floor.c',
        'src/math/floorf.c',
        'src/math/floorl.c',
        'src/math/fma.c',
        'src/math/fmaf.c',
        'src/math/fmal.c',
        'src/math/fmax.c',
        'src/math/fmaxf.c',
        'src/math/fmaxl.c',
        'src/math/fmin.c',
        'src/math/fminf.c',
        'src/math/fminl.c',
        'src/math/fmod.c',
        'src/math/fmodf.c',
        'src/math/fmodl.c',
        'src/math/__fpclassify.c',
        'src/math/__fpclassifyf.c',
        'src/math/__fpclassifyl.c',
        'src/math/frexp.c',
        'src/math/frexpf.c',
        'src/math/frexpl.c',
        'src/math/hypot.c',
        'src/math/hypotf.c',
        'src/math/hypotl.c',
        'src/math/ilogb.c',
        'src/math/ilogbf.c',
        'src/math/ilogbl.c',
        'src/math/__invtrigl.c',
        'src/math/j0.c',
        'src/math/j0f.c',
        'src/math/j1.c',
        'src/math/j1f.c',
        'src/math/jn.c',
        'src/math/jnf.c',
        'src/math/ldexp.c',
        'src/math/ldexpf.c',
        'src/math/ldexpl.c',
        'src/math/lgamma.c',
        'src/math/lgammaf.c',
        'src/math/lgammaf_r.c',
        'src/math/lgammal.c',
        'src/math/lgamma_r.c',
        'src/math/llrint.c',
        'src/math/llrintf.c',
        'src/math/llrintl.c',
        'src/math/llround.c',
        'src/math/llroundf.c',
        'src/math/llroundl.c',
        'src/math/log10.c',
        'src/math/log10f.c',
        'src/math/log10l.c',
        'src/math/log1p.c',
        'src/math/log1pf.c',
        'src/math/log1pl.c',
        'src/math/log2.c',
        'src/math/log2f.c',
        'src/math/log2l.c',
        'src/math/logb.c',
        'src/math/logbf.c',
        'src/math/logbl.c',
        'src/math/log.c',
        'src/math/logf.c',
        'src/math/logl.c',
        'src/math/lrint.c',
        'src/math/lrintf.c',
        'src/math/lrintl.c',
        'src/math/lround.c',
        'src/math/lroundf.c',
        'src/math/lroundl.c',
        'src/math/modf.c',
        'src/math/modff.c',
        'src/math/modfl.c',
        'src/math/nan.c',
        'src/math/nanf.c',
        'src/math/nanl.c',
        'src/math/nearbyint.c',
        'src/math/nearbyintf.c',
        'src/math/nearbyintl.c',
        'src/math/nextafter.c',
        'src/math/nextafterf.c',
        'src/math/nextafterl.c',
        'src/math/nexttoward.c',
        'src/math/nexttowardf.c',
        'src/math/nexttowardl.c',
        'src/math/__polevll.c',
        'src/math/pow.c',
        'src/math/powf.c',
        'src/math/powl.c',
        'src/math/remainder.c',
        'src/math/remainderf.c',
        'src/math/remainderl.c',
        'src/math/__rem_pio2.c',
        'src/math/__rem_pio2f.c',
        'src/math/__rem_pio2_large.c',
        'src/math/__rem_pio2l.c',
        'src/math/remquo.c',
        'src/math/remquof.c',
        'src/math/remquol.c',
        'src/math/rint.c',
        'src/math/rintf.c',
        'src/math/rintl.c',
        'src/math/round.c',
        'src/math/roundf.c',
        'src/math/roundl.c',
        'src/math/scalb.c',
        'src/math/scalbf.c',
        'src/math/scalbln.c',
        'src/math/scalblnf.c',
        'src/math/scalblnl.c',
        'src/math/scalbn.c',
        'src/math/scalbnf.c',
        'src/math/scalbnl.c',
        'src/math/__signbit.c',
        'src/math/__signbitf.c',
        'src/math/__signbitl.c',
        'src/math/signgam.c',
        'src/math/significand.c',
        'src/math/significandf.c',
        'src/math/__sin.c',
        'src/math/sin.c',
        'src/math/sincos.c',
        'src/math/sincosf.c',
        'src/math/sincosl.c',
        'src/math/__sindf.c',
        'src/math/sinf.c',
        'src/math/sinh.c',
        'src/math/sinhf.c',
        'src/math/sinhl.c',
        'src/math/__sinl.c',
        'src/math/sinl.c',
        'src/math/sqrt.c',
        'src/math/sqrtf.c',
        'src/math/sqrtl.c',
        'src/math/__tan.c',
        'src/math/tan.c',
        'src/math/__tandf.c',
        'src/math/tanf.c',
        'src/math/tanh.c',
        'src/math/tanhf.c',
        'src/math/tanhl.c',
        'src/math/__tanl.c',
        'src/math/tanl.c',
        'src/math/tgamma.c',
        'src/math/tgammaf.c',
        'src/math/tgammal.c',
        'src/math/trunc.c',
        'src/math/truncf.c',
        'src/math/truncl.c',
      ]
    }
  ]
}

