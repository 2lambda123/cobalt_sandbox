# Copyright (C) 2010 Google Inc. All rights reserved.
#
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions are
# met:
#
#     * Redistributions of source code must retain the above copyright
# notice, this list of conditions and the following disclaimer.
#     * Redistributions in binary form must reproduce the above
# copyright notice, this list of conditions and the following disclaimer
# in the documentation and/or other materials provided with the
# distribution.
#     * Neither the name of Google Inc. nor the names of its
# contributors may be used to endorse or promote products derived from
# this software without specific prior written permission.
#
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
# "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
# LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
# A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
# OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
# SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
# LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
# DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
# THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
# (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
# OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

import logging

from webkitpy.layout_tests.breakpad.dump_reader_multipart import DumpReaderLinux
from webkitpy.layout_tests.port import base
from webkitpy.layout_tests.port import win


_log = logging.getLogger(__name__)


class LinuxPort(base.Port):
    port_name = 'linux'

    SUPPORTED_VERSIONS = ('trusty',)

    FALLBACK_PATHS = {}
    FALLBACK_PATHS['trusty'] = ['linux'] + win.WinPort.latest_platform_fallback_path()

    DEFAULT_BUILD_DIRECTORIES = ('out',)

    BUILD_REQUIREMENTS_URL = 'https://chromium.googlesource.com/chromium/src/+/master/docs/linux_build_instructions.md'

    @classmethod
    def determine_full_port_name(cls, host, options, port_name):
        if port_name.endswith('linux'):
            assert host.platform.is_linux()
            version = host.platform.os_version
            return port_name + '-' + version
        return port_name

    def __init__(self, host, port_name, **kwargs):
        super(LinuxPort, self).__init__(host, port_name, **kwargs)
        self._version = port_name[port_name.index('linux-') + len('linux-'):]
        self._architecture = 'x86_64'
        assert self._version in self.SUPPORTED_VERSIONS

        if not self.get_option('disable_breakpad'):
            self._dump_reader = DumpReaderLinux(host, self._build_path())
        self._original_home = None

    def additional_driver_flag(self):
        flags = super(LinuxPort, self).additional_driver_flag()
        if not self.get_option('disable_breakpad'):
            flags += ['--enable-crash-reporter', '--crash-dumps-dir=%s' % self._dump_reader.crash_dumps_directory()]
        return flags

    def check_build(self, needs_http, printer):
        result = super(LinuxPort, self).check_build(needs_http, printer)

        if result:
            _log.error('For complete Linux build requirements, please see:')
            _log.error('')
            _log.error('    https://chromium.googlesource.com/chromium/src/+/master/docs/linux_build_instructions.md')
        return result

    def look_for_new_crash_logs(self, crashed_processes, start_time):
        if self.get_option('disable_breakpad'):
            return None
        return self._dump_reader.look_for_new_crash_logs(crashed_processes, start_time)

    def clobber_old_port_specific_results(self):
        if not self.get_option('disable_breakpad'):
            self._dump_reader.clobber_old_results()

    def operating_system(self):
        return 'linux'

    def path_to_apache(self):
        # The Apache binary path can vary depending on OS and distribution
        # See http://wiki.apache.org/httpd/DistrosDefaultLayout
        for path in ['/usr/sbin/httpd', '/usr/sbin/apache2']:
            if self._filesystem.exists(path):
                return path
        _log.error('Could not find apache. Not installed or unknown path.')
        return None

    def setup_test_run(self):
        super(LinuxPort, self).setup_test_run()
        self._setup_dummy_home_dir()

    def clean_up_test_run(self):
        super(LinuxPort, self).clean_up_test_run()
        self._clean_up_dummy_home_dir()

    #
    # PROTECTED METHODS
    #

    def _setup_dummy_home_dir(self):
        """Creates a dummy home directory for running the test.

        This is a workaround for crbug.com/595504; see crbug.com/612730.
        If crbug.com/612730 is resolved in another way, then this may be
        unnecessary.
        """
        self._original_home = self.host.environ.get('HOME')
        dummy_home = str(self._filesystem.mkdtemp())
        self.host.environ['HOME'] = dummy_home
        self._copy_files_to_dummy_home_dir(dummy_home)

    def _copy_files_to_dummy_home_dir(self, dummy_home):
        # Note: This may be unnecessary.
        fs = self._filesystem
        for filename in ['.Xauthority']:
            original_path = fs.join(self._original_home, filename)
            if not fs.exists(original_path):
                continue
            fs.copyfile(original_path, fs.join(dummy_home, filename))

    def _clean_up_dummy_home_dir(self):
        """Cleans up the dummy dir and resets the HOME environment variable."""
        dummy_home = self.host.environ['HOME']
        assert dummy_home != self._original_home
        self._filesystem.rmtree(dummy_home)
        self.host.environ['HOME'] = self._original_home

    def _path_to_driver(self, target=None):
        binary_name = self.driver_name()
        return self._build_path_with_target(target, binary_name)
