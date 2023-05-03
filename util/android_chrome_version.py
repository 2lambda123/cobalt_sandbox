# Copyright 2019 The Chromium Authors. All rights reserved.
# Use of this source code is governed by a BSD-style license that can be
# found in the LICENSE file.
"""Different build variants of Chrome for Android have different version codes.

For targets that have the same package name (e.g. Chrome, Chrome Modern,
Monochrome, Trichrome), Play Store considers them the same app and will push the
supported app with the highest version code to devices. Note that Play Store
does not support hosting two different apps with same version code and package
name.

Each version code generated by this script will be used by one or more APKs.

Webview channels must have unique version codes for a couple reasons:
a) Play Store does not support having the same version code for different
   versions of a package. Without unique codes, promoting a beta apk to stable
   would require first removing the beta version.
b) Firebase project support (used by official builders) requires unique
   [version code + package name].
   We cannot add new webview package names for new channels because webview
   packages are allowlisted by Android as webview providers.

WEBVIEW_STABLE, WEBVIEW_BETA, WEBVIEW_DEV are all used for standalone webview,
whereas the others are used for various chrome APKs.

Note that a package digit of '3' for Webview is reserved for Trichrome Webview.
The same versionCode is used for both Trichrome Chrome and Trichrome Webview.

Version code values are constructed like this:

  {full BUILD number}{3 digits: PATCH}{1 digit: package}{1 digit: ABIs}.

For example:

  Build 3721, patch 0, ChromeModern (1), on ARM64 (5): 372100015
  Build 3721, patch 9, Monochrome (2), on ARM (0): 372100920

"""

# Package name version bits.
_PACKAGE_NAMES = {
    'CHROME': 0,
    'CHROME_MODERN': 10,
    'MONOCHROME': 20,
    'TRICHROME': 30,
    'WEBVIEW_STABLE': 0,
    'WEBVIEW_BETA': 10,
    'WEBVIEW_DEV': 20,
}

""" "Next" builds get +5 on their package version code digit.

We choose 5 because it won't conflict with values in _PACKAGE_NAMES.
"""
_NEXT_BUILD_VERSION_CODE_DIFF = 50

"""List of version numbers to be created for each build configuration.
Tuple format:

  (version code name), (package name), (supported ABIs)

Here, (supported ABIs) is referring to the combination of browser ABI and
webview library ABI present in a particular APK. For example, 64_32 implies a
64-bit browser with an extra 32-bit Webview library. See also
_ABIS_TO_BIT_MASK.
"""
_APKS = {
    '32': [
        ('CHROME', 'CHROME', '32'),
        ('CHROME_MODERN', 'CHROME_MODERN', '32'),
        ('MONOCHROME', 'MONOCHROME', '32'),
        ('TRICHROME', 'TRICHROME', '32'),
        ('WEBVIEW_STABLE', 'WEBVIEW_STABLE', '32'),
        ('WEBVIEW_BETA', 'WEBVIEW_BETA', '32'),
        ('WEBVIEW_DEV', 'WEBVIEW_DEV', '32'),
    ],
    '64': [
        ('CHROME', 'CHROME', '64'),
        ('CHROME_MODERN', 'CHROME_MODERN', '64'),
        ('MONOCHROME', 'MONOCHROME', '32_64'),
        ('MONOCHROME_32', 'MONOCHROME', '32'),
        ('MONOCHROME_32_64', 'MONOCHROME', '32_64'),
        ('MONOCHROME_64_32', 'MONOCHROME', '64_32'),
        ('MONOCHROME_64', 'MONOCHROME', '64'),
        ('TRICHROME', 'TRICHROME', '32_64'),
        ('TRICHROME_32', 'TRICHROME', '32'),
        ('TRICHROME_32_64', 'TRICHROME', '32_64'),
        ('TRICHROME_64_32', 'TRICHROME', '64_32'),
        ('TRICHROME_64', 'TRICHROME', '64'),
        ('WEBVIEW_STABLE', 'WEBVIEW_STABLE', '32_64'),
        ('WEBVIEW_BETA', 'WEBVIEW_BETA', '32_64'),
        ('WEBVIEW_DEV', 'WEBVIEW_DEV', '32_64'),
        ('WEBVIEW_32_STABLE', 'WEBVIEW_STABLE', '32'),
        ('WEBVIEW_32_BETA', 'WEBVIEW_BETA', '32'),
        ('WEBVIEW_32_DEV', 'WEBVIEW_DEV', '32'),
    ]
}

# Splits input build config architecture to manufacturer and bitness.
_ARCH_TO_MFG_AND_BITNESS = {
    'arm': ('arm', '32'),
    'arm64': ('arm', '64'),
    'x86': ('intel', '32'),
    'x64': ('intel', '64'),
    'mipsel': ('mipsel', '32'),
}

# Expose the available choices to other scripts.
ARCH_CHOICES = _ARCH_TO_MFG_AND_BITNESS.keys()
"""
The architecture preference is encoded into the version_code for devices
that support multiple architectures. (exploiting play store logic that pushes
apk with highest version code)

Detail:
Many Android devices support multiple architectures, and can run applications
built for any of them; the Play Store considers all of the supported
architectures compatible and does not, itself, have any preference for which
is "better". The common cases here:

- All production arm64 devices can also run arm
- All production x64 devices can also run x86
- Pretty much all production x86/x64 devices can also run arm (via a binary
  translator)

Since the Play Store has no particular preferences, you have to encode your own
preferences into the ordering of the version codes. There's a few relevant
things here:

- For any android app, it's theoretically preferable to ship a 64-bit version to
  64-bit devices if it exists, because the 64-bit architectures are supposed to
  be "better" than their 32-bit predecessors (unfortunately this is not always
  true due to the effect on memory usage, but we currently deal with this by
  simply not shipping a 64-bit version *at all* on the configurations where we
  want the 32-bit version to be used).
- For any android app, it's definitely preferable to ship an x86 version to x86
  devices if it exists instead of an arm version, because running things through
  the binary translator is a performance hit.
- For WebView, Monochrome, and Trichrome specifically, they are a special class
  of APK called "multiarch" which means that they actually need to *use* more
  than one architecture at runtime (rather than simply being compatible with
  more than one). The 64-bit builds of these multiarch APKs contain both 32-bit
  and 64-bit code, so that Webview is available for both ABIs. If you're
  multiarch you *must* have a version that supports both 32-bit and 64-bit
  version on a 64-bit device, otherwise it won't work properly. So, the 64-bit
  version needs to be a higher versionCode, as otherwise a 64-bit device would
  prefer the 32-bit version that does not include any 64-bit code, and fail.
- The relative order of mips isn't important, but it needs to be a *distinct*
  value to the other architectures because all builds need unique version codes.
"""
_ABIS_TO_BIT_MASK = {
    'arm': {
        '32': 0,
        '32_64': 3,
        '64_32': 4,
        '64': 5,
    },
    'intel': {
        '32': 1,
        '32_64': 6,
        '64_32': 7,
        '64': 8,
    },
    'mipsel': {
        '32': 2,
    }
}

def GenerateVersionCodes(version_values, arch, is_next_build):
  """Build dict of version codes for the specified build architecture. Eg:

  {
    'CHROME_VERSION_CODE': '378100010',
    'MONOCHROME_VERSION_CODE': '378100013',
    ...
  }

  versionCode values are built like this:
  {full BUILD int}{3 digits: PATCH}{1 digit: package}{1 digit: ABIs}.

  MAJOR and MINOR values are not used for generating versionCode.
  - MINOR is always 0. It was used for something long ago in Chrome's history
    but has not been used since, and has never been nonzero on Android.
  - MAJOR is cosmetic and controlled by the release managers. MAJOR and BUILD
    always have reasonable sort ordering: for two version codes A and B, it's
    always the case that (A.MAJOR < B.MAJOR) implies (A.BUILD < B.BUILD), and
    that (A.MAJOR > B.MAJOR) implies (A.BUILD > B.BUILD). This property is just
    maintained by the humans who set MAJOR.

  Thus, this method is responsible for the final two digits of versionCode.
  """

  base_version_code = int(
      '%s%03d00' % (version_values['BUILD'], int(version_values['PATCH'])))

  if is_next_build:
    base_version_code += _NEXT_BUILD_VERSION_CODE_DIFF

  mfg, bitness = _ARCH_TO_MFG_AND_BITNESS[arch]

  version_codes = {}

  for apk, package, abis in _APKS[bitness]:
    abi_bits = _ABIS_TO_BIT_MASK[mfg][abis]
    package_bits = _PACKAGE_NAMES[package]

    version_code_name = apk + '_VERSION_CODE'
    version_code_val = base_version_code + abi_bits + package_bits
    version_codes[version_code_name] = str(version_code_val)

  return version_codes
