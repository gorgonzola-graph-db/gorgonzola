#pragma once
#include <cstdint>

#include "common/api.h"
namespace gorgonzola {
namespace main {

struct Version {
public:
    /**
     * @brief Get the version of the Gorgonzola library.
     * @return const char* The version of the Gorgonzola library.
     */
    GORGONZOLA_API static const char* getVersion();

    /**
     * @brief Get the storage version of the Gorgonzola library.
     * @return uint64_t The storage version of the Gorgonzola library.
     */
    GORGONZOLA_API static uint64_t getStorageVersion();
};
} // namespace main
} // namespace gorgonzola
