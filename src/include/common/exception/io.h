#pragma once

#include "exception.h"

namespace gorgonzola {
namespace common {

class GORGONZOLA_API IOException : public Exception {
public:
    explicit IOException(const std::string& msg) : Exception("IO exception: " + msg) {}
};

} // namespace common
} // namespace gorgonzola
