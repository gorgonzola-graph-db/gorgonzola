#pragma once

#include <cstdint>

#include "gorgonzola/export.h"

namespace gorgonzola {

/**
 * @brief Configuration options for the database engine.
 * These options govern memory, threading, and storage behavior.
 */
struct GORGONZOLA_API SystemConfig {
    /** @brief Max size of the buffer pool in bytes (default: 80% of system RAM). */
    uint64_t bufferPoolSize;
    /** @brief Max number of threads to use for query execution (0 = auto-detect). */
    uint64_t maxNumThreads;
    /** @brief Whether to compress data on disk. */
    bool enableCompression;
    /** @brief Whether to open the database in read-only mode. */
    bool readOnly;
    /** @brief Max allowed size of the database on disk in bytes. */
    uint64_t maxDBSize;
    /** @brief Whether to automatically checkpoint the WAL. */
    bool autoCheckpoint;
    /** @brief Checkpoint threshold in bytes (default: 16MB). */
    uint64_t checkpointThreshold;
    /** @brief Whether to force a checkpoint when the database is closed. */
    bool forceCheckpointOnClose;
    /** @brief Whether to throw an exception if WAL replay fails. */
    bool throwOnWalReplayFailure;
    /** @brief Whether to compute and verify page checksums. */
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
