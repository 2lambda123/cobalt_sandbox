# Copyright 2017 The Chromium Authors. All rights reserved.
# Use of this source code is governed by a BSD-style license that can be
# found in the LICENSE file.

from __future__ import absolute_import
from pylib.base import test_instance

import mock  # pylint: disable=import-error


MockTestInstance = mock.MagicMock(test_instance.TestInstance)
