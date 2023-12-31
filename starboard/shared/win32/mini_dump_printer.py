#!/usr/bin/python
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
"""Module for mini dump analysis and printing"""

from __future__ import print_function

import os
import subprocess
import sys

# This cdb tool is part of Microsoft's debugging toolset, it will allow mini
# dump analysis and printing.
_DEFAULT_TOOL_PATH = ('C:/Program Files (x86)/Windows Kits/10/Debuggers/x64'
                      '/cdb.exe')


def PrintMiniDump(mini_dump_path,
                  out_stream=sys.stdout,
                  tool_path=_DEFAULT_TOOL_PATH):
  out_stream.write('\n*** Found crash dump! ***\nMinDumpPath:' +
                   mini_dump_path + '\n')

  tool_path = os.path.abspath(tool_path)
  if not os.path.isfile(tool_path):
    out_stream.write('Could not perform crash analysis because ' + tool_path\
                     + ' does not exist.\n')
    return

  dump_log = mini_dump_path + '.log'
  cmd = (f'"{tool_path}" -z "{mini_dump_path}" -c "!analyze -v;q" > '
         f'{dump_log}')

  try:
    out_stream.write('Running command:\n' + cmd + '\n')
    subprocess.check_output(cmd, shell=True, universal_newlines=True)

    if not os.path.exists(dump_log):
      out_stream.write('Error - mini dump log ' + dump_log\
                       + ' was not found.\n')
      return
    with open(dump_log, encoding='utf-8') as f:
      out_stream.write(f.read() + '\n')
      out_stream.write('*** Finished printing minidump ' + mini_dump_path +
                       ' ***\n' + 'For more information, use VisualStudio ' +
                       'to load the minidump.\n')
    os.remove(dump_log)
  except Exception as e:  # pylint: disable=broad-except
    out_stream.write('Error: ' + str(e))
