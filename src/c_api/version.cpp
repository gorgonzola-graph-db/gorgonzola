#include "main/version.h"

#include "c_api/gorgonzola.h"
#include "c_api/helpers.h"
#include "c_api_utils.h"

char* gorgonzola_get_version() {
    GORGONZOLA_C_API_BEGIN

    return convertToOwnedCString(gorgonzola::main::Version::getVersion());
}
catch (...) {
    gorgonzola::c_api::translate_exception();
    return nullptr;
}
}

uint64_t gorgonzola_get_storage_version() {
    GORGONZOLA_C_API_BEGIN

    return gorgonzola::main::Version::getStorageVersion();
}
catch (...) {
    gorgonzola::c_api::translate_exception();
    return 0;
}
}
