#pragma once

#include "common/api.h"
#include "exception.h"

namespace gorgonzola {
namespace common {

class GORGONZOLA_API CopyException : public Exception {
public:
    explicit CopyException(const std::string& msg) : Exception("Copy exception: " + msg){};
};

} // namespace common
} // namespace gorgonzola
