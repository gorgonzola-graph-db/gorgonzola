#include "main/version.h"

#include "c_api/helpers.h"
#include "c_api/gorgonzola.h"
#include "c_api_utils.h"

char* gorgonzola_get_version() {
    return convertToOwnedCString(gorgonzola::main::Version::getVersion());
}

uint64_t gorgonzola_get_storage_version() {
    return gorgonzola::main::Version::getStorageVersion();
}
