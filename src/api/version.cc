#include "gorgonzola/version.h"
#include "main/version.h"

namespace gorgonzola {

const char* getVersion() {
    return main::Version::getVersion();
}

uint64_t getStorageVersion() {
    return main::Version::getStorageVersion();
}

} // namespace gorgonzola
