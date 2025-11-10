#pragma once

#include "common/api.h"
#include "exception.h"

namespace gorgonzola {
namespace common {

class GORGONZOLA_API StorageException : public Exception {
public:
    explicit StorageException(const std::string& msg) : Exception("Storage exception: " + msg){};
};

} // namespace common
} // namespace gorgonzola
