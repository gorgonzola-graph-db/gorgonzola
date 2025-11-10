#pragma once

#include "common/api.h"
#include "exception.h"

namespace gorgonzola {
namespace common {

class GORGONZOLA_API InterruptException : public Exception {
public:
    explicit InterruptException() : Exception("Interrupted."){};
};

} // namespace common
} // namespace gorgonzola
