#pragma once

#include "gorgonzola/export.h"

namespace gorgonzola {

/**
 * @brief Returns the Gorgonzola engine version string.
 */
GORGONZOLA_API const char* getVersion();

/**
 * @brief Returns the storage format version.
 */
GORGONZOLA_API const char* getStorageVersion();

} // namespace gorgonzola
