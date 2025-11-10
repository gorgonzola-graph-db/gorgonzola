#pragma once

#include "catalog/catalog.h"

namespace gorgonzola {
namespace extension {

class GORGONZOLA_API CatalogExtension : public catalog::Catalog {
public:
    CatalogExtension() : Catalog() {}

    virtual void init() = 0;

    void invalidateCache();
};

} // namespace extension
} // namespace gorgonzola
