#pragma once

#include "common/api.h"
#include "exception.h"

namespace gorgonzola {
namespace common {

class GORGONZOLA_API RuntimeException : public Exception {
public:
    explicit RuntimeException(const std::string& msg) : Exception("Runtime exception: " + msg){};
};

} // namespace common
} // namespace gorgonzola
