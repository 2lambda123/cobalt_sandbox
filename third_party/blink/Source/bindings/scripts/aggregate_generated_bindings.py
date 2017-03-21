#!/usr/bin/python
#
# Copyright (C) 2009 Google Inc. All rights reserved.
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
#
# Copyright (c) 2009 The Chromium Authors. All rights reserved.
# Use of this source code is governed by a BSD-style license that can be
# found in the LICENSE file.

"""Generates a .cpp file that includes all V8 binding .cpp files for interfaces.

It is expected to preserve symbol space, and to be acceptable to make static
build on Windows.

Usage:
 $ aggregate_generated_bindings.py --component COMPONENT IDL_FILES_LIST OUTPUT_FILE

 COMPONENT is the relative directory of a component, e.g., 'core', 'modules'.
 IDL_FILES_LIST is a text file containing the IDL file paths
 OUTPUT_FILE is the filename of output file.

 Design doc: http://www.chromium.org/developers/design-documents/idl-build
"""

import errno
import optparse
import os
import re
import sys
from utilities import idl_filename_to_interface_name
from utilities import read_idl_files_list_from_file

COPYRIGHT_TEMPLATE = """/*
 * THIS FILE WAS AUTOMATICALLY GENERATED, DO NOT EDIT.
 *
 * This file was generated by the action_derivedsourcesallinone.py script.
 *
 * Copyright (C) 2009 Google Inc.  All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY APPLE COMPUTER, INC. ``AS IS'' AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL APPLE COMPUTER, INC. OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY
 * OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
"""

def parse_options():
    parser = optparse.OptionParser()
    parser.add_option('--component')

    options, args = parser.parse_args()
    if len(args) < 2:
        raise Exception('Expected 2 filenames; one is for input, and the other is for output.')

    return options, args


def generate_content(component, interface_names):
    # Add fixed content.
    output = [COPYRIGHT_TEMPLATE,
              '#define NO_IMPLICIT_ATOMICSTRING\n\n']

    interface_names.sort()
    output.extend('#include "bindings/%s/v8/V8%s.cpp"\n' % (component, interface)
                  for interface in interface_names)

    return ''.join(output)


def write_content(content, output_file_name):
    parent_path, file_name = os.path.split(output_file_name)
    if not os.path.exists(parent_path):
        print 'Creating directory: %s' % parent_path
        os.makedirs(parent_path)
    with open(output_file_name, 'w') as f:
        f.write(content)


def main():
    options, filenames = parse_options()
    component = options.component
    idl_filenames = read_idl_files_list_from_file(filenames[0],
                                                  is_gyp_format=False)
    interface_names = [idl_filename_to_interface_name(file_path)
                       for file_path in idl_filenames]
    file_contents = generate_content(component, interface_names)
    write_content(file_contents, filenames[1])


if __name__ == '__main__':
    sys.exit(main())
