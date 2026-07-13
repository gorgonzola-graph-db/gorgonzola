#pragma once

#include <string>
#include "gorgonzola/export.h"

namespace gorgonzola {

enum class StatusCode {
    SUCCESS = 0,
    ERROR = 1,
    INVALID_QUERY = 2,
    TRANSACTION_ERROR = 3,
    NOT_IMPLEMENTED = 4,
};

class GORGONZOLA_API Status {
public:
    Status() : code_(StatusCode::SUCCESS) {}
    Status(StatusCode code, std::string message) 
        : code_(code), message_(std::move(message)) {}

    static Status Success() { return Status(); }
    static Status Error(std::string message) { 
        return Status(StatusCode::ERROR, std::move(message)); 
    }

    bool ok() const { return code_ == StatusCode::SUCCESS; }
    StatusCode code() const { return code_; }
    const std::string& message() const { return message_; }

private:
    StatusCode code_;
    std::string message_;
};

} // namespace gorgonzola
