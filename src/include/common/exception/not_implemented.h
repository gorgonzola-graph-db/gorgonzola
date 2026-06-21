#pragma once

#include "common/api.h"
#include "exception.h"

namespace gorgonzola {
namespace common {

class GORGONZOLA_API NotImplementedException : public Exception {
public:
    explicit NotImplementedException(const std::string& msg) : Exception(msg){};
};

} // namespace common
} // namespace gorgonzola
