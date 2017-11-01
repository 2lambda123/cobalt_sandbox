#!/usr/bin/python
# Copyright 2014 Google Inc. All Rights Reserved.
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
"""Initialization for the config package."""

import sys

import _env  # pylint: disable=unused-import
import base
from base import GetPlatformConfig
import starboard

# This bit of voodoo fakes out the config.starboard module so that legacy
# platforms will still GYP properly.
sys.modules['config.starboard'] = starboard
starboard.PlatformConfigStarboard = base.PlatformConfigBase
