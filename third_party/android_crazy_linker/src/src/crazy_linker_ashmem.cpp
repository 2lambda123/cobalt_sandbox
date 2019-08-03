// Copyright 2014 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "crazy_linker_ashmem.h"

#include <fcntl.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#include <linux/ashmem.h>

#include "crazy_linker_system.h"
#include "crazy_linker_memory_mapping.h"

namespace crazy {

bool AshmemRegion::Allocate(size_t region_size, const char* region_name) {
  int fd = TEMP_FAILURE_RETRY(open("/dev/ashmem", O_RDWR));
  if (fd < 0)
    return false;

  if (ioctl(fd, ASHMEM_SET_SIZE, region_size) < 0)
    goto ERROR;

  if (region_name) {
    char buf[256];
    strlcpy(buf, region_name, sizeof(buf));
    if (ioctl(fd, ASHMEM_SET_NAME, buf) < 0)
      goto ERROR;
  }

  Reset(fd);
  return true;

ERROR:
  ::close(fd);
  return false;
}

bool AshmemRegion::SetProtectionFlags(int prot) {
  return ioctl(fd_, ASHMEM_SET_PROT_MASK, prot) == 0;
}

// static
bool AshmemRegion::CheckFileDescriptorIsReadOnly(int fd) {
  const size_t map_size = PAGE_SIZE;

  // First, check that trying to map a page of the region with PROT_WRITE
  // fails with EPERM.
  MemoryMapping map =
      MemoryMapping::Create(nullptr, map_size, MemoryMapping::CAN_WRITE, fd);
  if (map.IsValid()) {
    LOG("Region could be mapped writable. Should not happen.");
    errno = EPERM;
    return false;
  }
  if (errno != EPERM) {
    LOG_ERRNO("Region failed writable mapping with unexpected error");
    return false;
  }

  // Second, check that it can be mapped PROT_READ, but cannot be remapped
  // with PROT_READ | PROT_WRITE through mprotect().
  map = MemoryMapping::Create(nullptr, map_size, MemoryMapping::CAN_READ, fd);
  if (!map.IsValid()) {
    LOG_ERRNO("Failed to map region read-only");
    return false;
  }
  if (map.SetProtection(MemoryMapping::CAN_READ_WRITE)) {
    LOG_ERRNO("Region could be remapped read-write. Should not happen.");
    return false;
  }
  if (errno != EACCES) {
    LOG_ERRNO("Region failed to be remapped read-write with unexpected error");
    return false;
  }

  // Everything's good.
  return true;
}

}  // namespace crazy
