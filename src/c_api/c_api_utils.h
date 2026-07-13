#pragma once

#include <unordered_map>
#include <shared_mutex>
#include <mutex>
#include <exception>
#include "c_api/gorgonzola.h"

namespace gorgonzola {
namespace c_api {

enum class HandleType {
    Database,
    Connection,
    PreparedStatement,
    QueryResult,
    FlatTuple,
    LogicalType,
    Value,
    QuerySummary
};

class HandleRegistry {
public:
    static HandleRegistry& getInstance() {
        static HandleRegistry instance;
        return instance;
    }

    void registerHandle(void* ptr, HandleType type) {
        if (!ptr) return;
        std::unique_lock<std::shared_mutex> lock(mutex_);
        registry_[ptr] = type;
    }

    void unregisterHandle(void* ptr) {
        if (!ptr) return;
        std::unique_lock<std::shared_mutex> lock(mutex_);
        registry_.erase(ptr);
    }

    bool isValid(void* ptr, HandleType expectedType) {
        if (!ptr) return false;
        std::shared_lock<std::shared_mutex> lock(mutex_);
        auto it = registry_.find(ptr);
        if (it != registry_.end()) {
            return it->second == expectedType;
        }
        return false;
    }

private:
    HandleRegistry() = default;
    ~HandleRegistry() = default;

    std::shared_mutex mutex_;
    std::unordered_map<void*, HandleType> registry_;
};

// Exception translation
void translate_exception();

// Macros for exception boundaries
#define GORGONZOLA_C_API_BEGIN try {

#define GORGONZOLA_C_API_END(return_val) \
    } catch (...) { \
        gorgonzola::c_api::translate_exception(); \
        return return_val; \
    }

#define GORGONZOLA_C_API_END_VOID \
    } catch (...) { \
        gorgonzola::c_api::translate_exception(); \
    }

// Validation macros
#define VALIDATE_HANDLE_RET(ptr, type, return_val) \
    if (!ptr || !gorgonzola::c_api::HandleRegistry::getInstance().isValid(ptr, gorgonzola::c_api::HandleType::type)) { \
        return return_val; \
    }

#define VALIDATE_HANDLE_VOID(ptr, type) \
    if (!ptr || !gorgonzola::c_api::HandleRegistry::getInstance().isValid(ptr, gorgonzola::c_api::HandleType::type)) { \
        return; \
    }

} // namespace c_api
} // namespace gorgonzola
