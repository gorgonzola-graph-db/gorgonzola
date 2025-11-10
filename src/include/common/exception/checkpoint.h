#pragma once

#include "common/api.h"
#include "exception.h"

namespace gorgonzola {
namespace common {

class GORGONZOLA_API CheckpointException : public Exception {
public:
    explicit CheckpointException(const std::exception& e) : Exception(e.what()){};
};

} // namespace common
} // namespace gorgonzola
