#include "main/delta_extension.h"

#include "function/delta_scan.h"
#include "main/client_context.h"
#include "main/database.h"
#include "main/duckdb_extension.h"

namespace gorgonzola {
namespace delta_extension {

void DeltaExtension::load(main::ClientContext* context) {
    auto& db = *context->getDatabase();
    extension::ExtensionUtils::addTableFunc<DeltaScanFunction>(db);
    duckdb_extension::DuckdbExtension::loadRemoteFSOptions(context);
}

} // namespace delta_extension
} // namespace gorgonzola

#if defined(BUILD_DYNAMIC_LOAD)
extern "C" {
// Because we link against the static library on windows, we implicitly inherit GORGONZOLA_STATIC_DEFINE,
// which cancels out any exporting, so we can't use GORGONZOLA_API.
#if defined(_WIN32)
#define INIT_EXPORT __declspec(dllexport)
#else
#define INIT_EXPORT __attribute__((visibility("default")))
#endif
INIT_EXPORT void init(gorgonzola::main::ClientContext* context) {
    gorgonzola::delta_extension::DeltaExtension::load(context);
}

INIT_EXPORT const char* name() {
    return gorgonzola::delta_extension::DeltaExtension::EXTENSION_NAME;
}
}
#endif
