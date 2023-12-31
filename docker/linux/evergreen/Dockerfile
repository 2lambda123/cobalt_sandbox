# Copyright 2021 The Cobalt Authors. All Rights Reserved.
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

FROM cobalt-build-base

RUN apt update -qqy \
    && apt install -qqy --no-install-recommends \
        clang \
        libxml2 \
        binutils-aarch64-linux-gnu \
        binutils-arm-linux-gnueabi \
        g++-multilib \
    && /opt/clean-after-apt.sh

CMD gn gen ${OUTDIR}/${PLATFORM}${SB_API_VERSION:+-sbversion-$SB_API_VERSION}_${CONFIG:-debug} --args="target_platform=\"${PLATFORM}\" build_type=\"${CONFIG:-debug}\" target_cpu=\"${TARGET_CPU}\" ${SB_API_VERSION:+sb_api_version=$SB_API_VERSION}" && \
    ninja -v -j ${NINJA_PARALLEL} -C ${OUTDIR}/${PLATFORM}${SB_API_VERSION:+-sbversion-$SB_API_VERSION}_${CONFIG:-debug} ${TARGET:-cobalt_install}
