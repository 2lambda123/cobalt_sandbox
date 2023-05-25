#ifndef MEMPROF_RAWPROFILE_H_
#define MEMPROF_RAWPROFILE_H_

#include "memprof_mibmap.h"
#include "sanitizer_common/sanitizer_procmaps.h"

namespace __memprof {
// Serialize the in-memory representation of the memprof profile to the raw
// binary format. The format itself is documented memprof_rawprofile.cpp.
u64 SerializeToRawProfile(MIBMapTy &BlockCache, MemoryMappingLayoutBase &Layout,
                          char *&Buffer);
} // namespace __memprof

#endif // MEMPROF_RAWPROFILE_H_
