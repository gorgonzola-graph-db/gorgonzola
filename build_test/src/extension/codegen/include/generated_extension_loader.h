#pragma once

#include "main/client_context.h"


namespace gorgonzola {
namespace extension {

void loadLinkedExtensions(main::ClientContext* context, std::vector<LoadedExtension>& loadedExtensions);

} // namespace extension
} // namespace gorgonzola
