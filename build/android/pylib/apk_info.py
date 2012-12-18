# Copyright (c) 2012 The Chromium Authors. All rights reserved.
# Use of this source code is governed by a BSD-style license that can be
# found in the LICENSE file.

"""Gathers information about APKs."""

import collections
import logging
import os
import pickle
import re

import cmd_helper

# If you change the cached output of proguard, increment this number
PICKLE_FORMAT_VERSION = 1

def GetPackageNameForApk(apk_path):
  """Returns the package name of the apk file."""
  aapt_output = cmd_helper.GetCmdOutput(
      ['aapt', 'dump', 'badging', apk_path]).split('\n')
  package_name_re = re.compile(r'package: .*name=\'(\S*)\'')
  for line in aapt_output:
    m = package_name_re.match(line)
    if m:
      return m.group(1)
  raise Exception('Failed to determine package name of %s' % apk_path)


class ApkInfo(object):
  """Helper class for inspecting APKs."""

  def __init__(self, apk_path, jar_path):
    self._PROGUARD_PATH = os.path.join(os.environ['ANDROID_SDK_ROOT'],
                                       'tools/proguard/bin/proguard.sh')
    if not os.path.exists(self._PROGUARD_PATH):
      self._PROGUARD_PATH = os.path.join(os.environ['ANDROID_BUILD_TOP'],
                                         'external/proguard/bin/proguard.sh')
    self._PROGUARD_CLASS_RE = re.compile(r'\s*?- Program class:\s*([\S]+)$')
    self._PROGUARD_METHOD_RE = re.compile(r'\s*?- Method:\s*(\S*)[(].*$')
    self._PROGUARD_ANNOTATION_RE = re.compile(r'\s*?- Annotation \[L(\S*);\]:$')
    self._PROGUARD_ANNOTATION_CONST_RE = (
        re.compile(r'\s*?- Constant element value.*$'))
    self._PROGUARD_ANNOTATION_VALUE_RE = re.compile(r'\s*?- \S+? \[(.*)\]$')

    if not os.path.exists(apk_path):
      raise Exception('%s not found, please build it' % apk_path)
    self._apk_path = apk_path
    if not os.path.exists(jar_path):
      raise Exception('%s not found, please build it' % jar_path)
    self._jar_path = jar_path
    self._annotation_map = collections.defaultdict(list)
    self._pickled_proguard_name = self._jar_path + '-proguard.pickle'
    self._test_methods = []
    self._Initialize()

  def _Initialize(self):
    if not self._GetCachedProguardData():
      self._GetProguardData()

  def _GetCachedProguardData(self):
    if (os.path.exists(self._pickled_proguard_name) and
        (os.path.getmtime(self._pickled_proguard_name) >
         os.path.getmtime(self._jar_path))):
      logging.info('Loading cached proguard output from %s',
                   self._pickled_proguard_name)
      try:
        with open(self._pickled_proguard_name, 'r') as r:
          d = pickle.loads(r.read())
        if d['VERSION'] == PICKLE_FORMAT_VERSION:
          self._annotation_map = d['ANNOTATION_MAP']
          self._test_methods = d['TEST_METHODS']
          return True
      except:
        logging.warning('PICKLE_FORMAT_VERSION has changed, ignoring cache')
    return False

  def _GetProguardData(self):
    proguard_output = cmd_helper.GetCmdOutput([self._PROGUARD_PATH,
                                               '-injars', self._jar_path,
                                               '-dontshrink',
                                               '-dontoptimize',
                                               '-dontobfuscate',
                                               '-dontpreverify',
                                               '-dump',
                                              ]).split('\n')
    clazz = None
    method = None
    annotation = None
    has_value = False
    qualified_method = None
    for line in proguard_output:
      m = self._PROGUARD_CLASS_RE.match(line)
      if m:
        clazz = m.group(1).replace('/', '.')  # Change package delim.
        annotation = None
        continue

      m = self._PROGUARD_METHOD_RE.match(line)
      if m:
        method = m.group(1)
        annotation = None
        qualified_method = clazz + '#' + method
        if method.startswith('test') and clazz.endswith('Test'):
          self._test_methods += [qualified_method]
        continue

      if not qualified_method:
        # Ignore non-method annotations.
        continue

      m = self._PROGUARD_ANNOTATION_RE.match(line)
      if m:
        annotation = m.group(1).split('/')[-1]  # Ignore the annotation package.
        self._annotation_map[qualified_method].append(annotation)
        has_value = False
        continue
      if annotation:
        if not has_value:
          m = self._PROGUARD_ANNOTATION_CONST_RE.match(line)
          if m:
            has_value = True
        else:
          m = self._PROGUARD_ANNOTATION_VALUE_RE.match(line)
          if m:
            value = m.group(1)
            self._annotation_map[qualified_method].append(
                annotation + ':' + value)
            has_value = False

    logging.info('Storing proguard output to %s', self._pickled_proguard_name)
    d = {'VERSION': PICKLE_FORMAT_VERSION,
         'ANNOTATION_MAP': self._annotation_map,
         'TEST_METHODS': self._test_methods}
    with open(self._pickled_proguard_name, 'w') as f:
      f.write(pickle.dumps(d))

  def _GetAnnotationMap(self):
    return self._annotation_map

  def _IsTestMethod(self, test):
    class_name, method = test.split('#')
    return class_name.endswith('Test') and method.startswith('test')

  def GetApkPath(self):
    return self._apk_path

  def GetPackageName(self):
    """Returns the package name of this APK."""
    return GetPackageNameForApk(self._apk_path)

  def GetTestAnnotations(self, test):
    """Returns a list of all annotations for the given |test|. May be empty."""
    if not self._IsTestMethod(test):
      return []
    return self._GetAnnotationMap()[test]

  def _AnnotationsMatchFilters(self, annotation_filter_list, annotations):
    """Checks if annotations match any of the filters."""
    if not annotation_filter_list:
      return True
    for annotation_filter in annotation_filter_list:
      filters = annotation_filter.split('=')
      if len(filters) == 2:
        key = filters[0]
        value_list = filters[1].split(',')
        for value in value_list:
          if key + ':' + value in annotations:
            return True
      elif annotation_filter in annotations:
        return True
    return False

  def GetAnnotatedTests(self, annotation_filter_list):
    """Returns a list of all tests that match the given annotation filters."""
    return [test for test, annotations in self._GetAnnotationMap().iteritems()
            if self._IsTestMethod(test) and self._AnnotationsMatchFilters(
                annotation_filter_list, annotations)]

  def GetTestMethods(self):
    """Returns a list of all test methods in this apk as Class#testMethod."""
    return self._test_methods

  @staticmethod
  def IsPythonDrivenTest(test):
    return 'pythonDrivenTests' in test
