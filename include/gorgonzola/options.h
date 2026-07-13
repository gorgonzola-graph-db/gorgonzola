#pragma once

#include <cstdint>
#include "gorgonzola/export.h"

namespace gorgonzola {

struct GORGONZOLA_API SystemConfig {
    uint64_t bufferPoolSize;
    uint64_t maxNumThreads;
    bool enableCompression;
    bool readOnly;
    uint64_t maxDBSize;
    bool autoCheckpoint;
    uint64_t checkpointThreshold;
    bool forceCheckpointOnClose;
    bool throwOnWalReplayFailure;
    bool enableChecksums;

    SystemConfig(uint64_t bufferPoolSize = -1u, uint64_t maxNumThreads = 0,
                 bool enableCompression = true, bool readOnly = false, uint64_t maxDBSize = -1u,
                 bool autoCheckpoint = true, uint64_t checkpointThreshold = 16777216 /* 16MB */,
                 bool forceCheckpointOnClose = true, bool throwOnWalReplayFailure = true,
                 bool enableChecksums = true)
        : bufferPoolSize(bufferPoolSize), maxNumThreads(maxNumThreads),
          enableCompression(enableCompression), readOnly(readOnly), maxDBSize(maxDBSize),
          autoCheckpoint(autoCheckpoint), checkpointThreshold(checkpointThreshold),
          forceCheckpointOnClose(forceCheckpointOnClose),
          throwOnWalReplayFailure(throwOnWalReplayFailure), enableChecksums(enableChecksums) {}
};

} // namespace gorgonzola
