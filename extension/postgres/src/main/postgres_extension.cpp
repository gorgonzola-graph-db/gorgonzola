#include "main/postgres_extension.h"

#include "function/sql_query.h"
#include "main/client_context.h"
#include "main/database.h"
#include "storage/postgres_storage.h"

namespace gorgonzola {
namespace postgres_extension {

void PostgresExtension::load(main::ClientContext* context) {
    auto db = context->getDatabase();
    db->registerStorageExtension(EXTENSION_NAME, std::make_unique<PostgresStorageExtension>(*db));
    extension::ExtensionUtils::addTableFunc<SqlQueryFunction>(*db);
}

} // namespace postgres_extension
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
    gorgonzola::postgres_extension::PostgresExtension::load(context);
}

INIT_EXPORT const char* name() {
    return gorgonzola::postgres_extension::PostgresExtension::EXTENSION_NAME;
}
}
#endif
