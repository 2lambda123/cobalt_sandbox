# Copyright 2017 The Chromium Authors. All rights reserved.
# Use of this source code is governed by a BSD-style license that can be
# found in the LICENSE file.

from __future__ import absolute_import
from pylib import constants
from pylib.output import local_output_manager
from pylib.output import remote_output_manager
from pylib.utils import local_utils


def CreateOutputManager(args):
  if args.local_output or not local_utils.IsOnSwarming():
    return local_output_manager.LocalOutputManager(
        output_dir=constants.GetOutDirectory())
  else:
    return remote_output_manager.RemoteOutputManager(
        bucket=args.gs_results_bucket)
