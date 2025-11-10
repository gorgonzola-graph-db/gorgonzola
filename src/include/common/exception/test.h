#pragma once

#include "exception.h"

namespace gorgonzola {
namespace common {

class TestException : public Exception {
public:
    explicit TestException(const std::string& msg) : Exception("Test exception: " + msg){};
};

} // namespace common
} // namespace gorgonzola
