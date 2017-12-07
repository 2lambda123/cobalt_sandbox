# Copyright 2017 Google Inc. All Rights Reserved.
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
"""Starboard Android Cobalt shared configuration."""

import os

from cobalt.build import cobalt_configuration
from starboard.tools.testing import test_filter


class CobaltAndroidConfiguration(cobalt_configuration.CobaltConfiguration):
  """Starboard Android Cobalt shared configuration."""

  def __init__(self, platform_configuration, application_name,
               application_directory):
    super(CobaltAndroidConfiguration, self).__init__(
        platform_configuration, application_name, application_directory)

  def GetPostIncludes(self):
    # If there isn't a configuration.gypi found in the usual place, we'll
    # supplement with our shared implementation.
    includes = super(CobaltAndroidConfiguration, self).GetPostIncludes()
    for include in includes:
      if os.path.basename(include) == 'configuration.gypi':
        return includes

    shared_gypi_path = os.path.join(os.path.dirname(__file__),
                                    'configuration.gypi')
    if os.path.isfile(shared_gypi_path):
      includes.append(shared_gypi_path)
    return includes

  def GetTestFilters(self):
    filters = super(CobaltAndroidConfiguration, self).GetTestFilters()
    filters.extend(self._FAILING_TESTS)
    del filters  # TODO: Return this when buildbot testing works on Android.
    return [test_filter.DISABLE_TESTING]

  # This is a list of unit tests that are confirmed to fail on Android.  When
  # buildbot testing is enabled, return this list of test filters from
  # GetTestFilters().
  _FAILING_TESTS = [
      # This list was generated by tracking crashes and failures from the
      # android-x86 platform using the "devel" configuration.  If any additional
      # failures are found on other platform, create a PlatformConfig class in
      # its gyp_configuration.py file and override this method.

      # Delete when Webdriver Benchmarks for Android are enabled.
      test_filter.TestFilter('webdriver_test', test_filter.FILTER_ALL),

      # Crash occurs on android-x86_devel
      test_filter.TestFilter('nb_test', test_filter.FILTER_ALL),

      # All of the following tests fail on android-x86_devel.
      test_filter.TestFilter('net_unittests',
                             'HostResolverImplDnsTest.DnsTaskUnspec'),

      # Crash occurs on android-x86_devel
      test_filter.TestFilter('poem_unittests', test_filter.FILTER_ALL)
  ]

  def GetTestEnvVariables(self):
    return {
        'base_unittests': {'ASAN_OPTIONS': 'detect_leaks=0'},
        'crypto_unittests': {'ASAN_OPTIONS': 'detect_leaks=0'},
        'net_unittests': {'ASAN_OPTIONS': 'detect_leaks=0'}
    }
