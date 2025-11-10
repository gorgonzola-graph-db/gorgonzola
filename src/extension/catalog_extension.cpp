#include "extension/catalog_extension.h"

namespace gorgonzola {
namespace extension {

void CatalogExtension::invalidateCache() {
    tables = std::make_unique<catalog::CatalogSet>();
    init();
}

} // namespace extension
} // namespace gorgonzola
