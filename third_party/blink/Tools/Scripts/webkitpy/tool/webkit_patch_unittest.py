# Copyright 2016 The Chromium Authors. All rights reserved.
# Use of this source code is governed by a BSD-style license that can be
# found in the LICENSE file.

import unittest

from webkitpy.common.system.output_capture import OutputCapture
from webkitpy.tool.webkit_patch import WebKitPatch


class WebKitPatchTest(unittest.TestCase):

    def test_split_args_basic(self):
        self.assertEqual(
            WebKitPatch._split_command_name_from_args(['--global-option', 'command', '--option', 'arg']),
            ('command', ['--global-option', '--option', 'arg']))

    def test_split_args_empty(self):
        self.assertEqual(
            WebKitPatch._split_command_name_from_args([]),
            (None, []))

    def test_split_args_with_no_options(self):
        self.assertEqual(
            WebKitPatch._split_command_name_from_args(['command', 'arg']),
            ('command', ['arg']))

    def test_command_by_name(self):
        tool = WebKitPatch('path')
        self.assertEqual(tool.command_by_name('help').name, 'help')
        self.assertIsNone(tool.command_by_name('non-existent'))

    def test_help_command(self):
        oc = OutputCapture()
        oc.capture_output()
        tool = WebKitPatch('path')
        tool.main(['tool', 'help'])
        out, err, logs = oc.restore_output()
        self.assertTrue(out.startswith('Usage: '))
        self.assertEqual('', err)
        self.assertEqual('', logs)

    def test_help_argument(self):
        oc = OutputCapture()
        oc.capture_output()
        tool = WebKitPatch('path')
        try:
            tool.main(['tool', '--help'])
        except SystemExit:
            pass  # optparse calls sys.exit after showing help.
        finally:
            out, err, logs = oc.restore_output()
        self.assertTrue(out.startswith('Usage: '))
        self.assertEqual('', err)
        self.assertEqual('', logs)
