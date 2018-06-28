#!/usr/bin/python
#
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
"""Tool to create a new Starboard build deriving from an existing one.

When this tool is provided with input giving the desired existing build path to
derive from as well as a destination build path to create, it will generate
the files necessary for a new build to be created in the destination build
path.  The generated build will be identical to the previous build since it
derives all configuration from it.  The intended usage of this script is to
make it easy to initialize new Starboard builds that are later manually tweaked
by hand.
"""

import argparse
import datetime
import logging
import os
import sys
import textwrap

import _env  # pylint: disable=unused-import
from starboard.tools import paths
import starboard.tools.environment as environment

TEMPLATE_DIRECTORY = (
    os.path.join(os.path.dirname(__file__), 'create_derived_build_templates'))

# The list of files we will generate in the output directory.  For each of
# these files it is expected that there exists a corresponding template file in
# the directory TEMPLATE_DIRECTORY.
FILES_TO_GENERATE = [
    'atomic_public.h',
    'configuration_public.h',
    'gyp_configuration.gypi',
    'gyp_configuration.py',
    'starboard_platform.gyp',
    'starboard_platform_tests.gyp',
    'thread_types_public.h',
]


def _GenerateFile(output_file_path, template_file_path, template_dictionary):
  """Applies the dictionary to the template file and outputs to output file."""
  if not os.path.exists(os.path.dirname(output_file_path)):
    os.makedirs(os.path.dirname(output_file_path))

  with open(template_file_path, 'r') as f:
    template_file_contents = f.read()

  with open(output_file_path, 'w') as f:
    f.write(template_file_contents.format(**template_dictionary))


def _GetCopyrightYear():
  """Returns the year to be used in the copyright header comment."""
  return datetime.datetime.now().year


def _GetAutoGeneratedPreambleComment(language):
  """Returns the comment about how a current file is auto-generated."""
  if language == 'cc':
    comment_start = '//'
  elif language == 'python':
    comment_start = '#'
  script_path_relative_to_repository = (
      _GetPathRelativeToRepository(os.path.abspath(__file__)))
  return ('{comment_start} This file was initially generated by {},\n'
          '{comment_start} though it may have been modified since its '
          'creation.'.format(
              script_path_relative_to_repository, comment_start=comment_start))


def _GetPathRelativeToStarboardRoot(path):
  abspath = os.path.abspath(path)
  # This is a bit tricky because there can be multiple Starboard roots.
  for root in environment.GetStarboardPortRoots():
    absroot = os.path.abspath(root)
    if os.path.commonprefix([absroot, abspath]) == absroot:
      return os.path.relpath(abspath, start=absroot)

  raise Exception('Path not relative to any Starboard root.')


def _GetPathRelativeToRepository(path):
  return os.path.relpath(os.path.abspath(path), start=paths.REPOSITORY_ROOT)


def _GetIncludeGuard(output_file_path):
  """Returns the include guard define to be used in generated C++ headers."""
  repository_relative_path = _GetPathRelativeToRepository(output_file_path)
  return (repository_relative_path.upper().replace(os.sep, '_').replace(
      '.', '_') + '_')


def _GetBuildName(output_build_path):
  """Returns the name of the build (e.g. how you identify the build to gyp)."""
  starboard_relative_path = _GetPathRelativeToStarboardRoot(output_build_path)
  return starboard_relative_path.lower().replace(os.sep, '-')


def _GetParentImportPath(parent_build_path):
  repository_relative_path = _GetPathRelativeToRepository(parent_build_path)
  return repository_relative_path.replace(os.sep, '.')


def _CreateDerivedBuild(parent_build_path, parent_configuration_class_name,
                        output_build_path):
  general_template_dictionary = {
      'copyright_year':
          _GetCopyrightYear(),
      'auto_generated_cc_preamble_comment':
          _GetAutoGeneratedPreambleComment('cc'),
      'auto_generated_python_preamble_comment':
          _GetAutoGeneratedPreambleComment('python'),
      'parent_build_path':
          _GetPathRelativeToRepository(parent_build_path),
      'build_name':
          _GetBuildName(output_build_path),
      'parent_import_path':
          _GetParentImportPath(parent_build_path),
      'parent_configuration_class_name':
          parent_configuration_class_name,
  }

  # Go through each file in the list of files to generate and for each one
  # apply the dictionary to the template to produce the generated file.
  for gen_file in FILES_TO_GENERATE:
    output_file_path = os.path.join(output_build_path, gen_file)

    # Apply per-file customizations to the template dictionary.
    file_template_dictionary = general_template_dictionary
    file_template_dictionary['include_guard'] = (
        _GetIncludeGuard(output_file_path))

    _GenerateFile(output_file_path,
                  os.path.join(TEMPLATE_DIRECTORY, gen_file) + '.template',
                  file_template_dictionary)


def main():
  logging.basicConfig(
      level=logging.INFO,
      format=('[%(filename)s:%(lineno)s - %(asctime)s %(levelname)-8s] '
              '%(message)s'),
      datefmt='%m-%d %H:%M')

  parser = argparse.ArgumentParser(
      formatter_class=argparse.ArgumentDefaultsHelpFormatter,
      description=textwrap.dedent(__doc__))
  parser.add_argument(
      '-p',
      '--parent_build_path',
      required=True,
      type=str,
      help='Path to the build that the destination build will inherit from.')
  parser.add_argument(
      '-c',
      '--parent_configuration_class_name',
      required=True,
      type=str,
      help='The name of the class in the parent build\'s gyp_configuration.py '
      'file that the derived build\'s gyp_configuration.py should '
      'derive from.  You may have to open the parent build\'s '
      'gyp_configuration.py file to know what to pass in for this '
      'parameter.')
  parser.add_argument(
      '-o',
      '--output_build_path',
      required=True,
      type=str,
      help='Path to the directory where the destination build\'s files will '
      'be created.')
  arguments = parser.parse_args()

  return _CreateDerivedBuild(
      os.path.normpath(arguments.parent_build_path),
      arguments.parent_configuration_class_name,
      os.path.normpath(arguments.output_build_path))


if __name__ == '__main__':
  sys.exit(main())
