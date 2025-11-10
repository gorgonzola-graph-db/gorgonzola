#include "main/prepared_statement.h"

#include "c_api/helpers.h"
#include "c_api/gorgonzola.h"
#include "common/types/value/value.h"

using namespace gorgonzola::common;
using namespace gorgonzola::main;

void gorgonzola_prepared_statement_bind_cpp_value(gorgonzola_prepared_statement* prepared_statement,
    const char* param_name, std::unique_ptr<Value> value) {
    auto* bound_values = static_cast<std::unordered_map<std::string, std::unique_ptr<Value>>*>(
        prepared_statement->_bound_values);
    bound_values->erase(param_name);
    bound_values->insert({param_name, std::move(value)});
}

void gorgonzola_prepared_statement_destroy(gorgonzola_prepared_statement* prepared_statement) {
    if (prepared_statement == nullptr) {
        return;
    }
    if (prepared_statement->_prepared_statement != nullptr) {
        delete static_cast<PreparedStatement*>(prepared_statement->_prepared_statement);
    }
    if (prepared_statement->_bound_values != nullptr) {
        delete static_cast<std::unordered_map<std::string, std::unique_ptr<Value>>*>(
            prepared_statement->_bound_values);
    }
}

bool gorgonzola_prepared_statement_is_success(gorgonzola_prepared_statement* prepared_statement) {
    return static_cast<PreparedStatement*>(prepared_statement->_prepared_statement)->isSuccess();
}

char* gorgonzola_prepared_statement_get_error_message(gorgonzola_prepared_statement* prepared_statement) {
    auto error_message =
        static_cast<PreparedStatement*>(prepared_statement->_prepared_statement)->getErrorMessage();
    if (error_message.empty()) {
        return nullptr;
    }
    return convertToOwnedCString(error_message);
}

gorgonzola_state gorgonzola_prepared_statement_bind_bool(gorgonzola_prepared_statement* prepared_statement,
    const char* param_name, bool value) {
    try {
        auto value_ptr = std::make_unique<Value>(value);
        gorgonzola_prepared_statement_bind_cpp_value(prepared_statement, param_name,
            std::move(value_ptr));
        return GorgonzolaSuccess;
    } catch (Exception& e) {
        return GorgonzolaError;
    }
}

gorgonzola_state gorgonzola_prepared_statement_bind_int64(gorgonzola_prepared_statement* prepared_statement,
    const char* param_name, int64_t value) {
    try {
        auto value_ptr = std::make_unique<Value>(value);
        gorgonzola_prepared_statement_bind_cpp_value(prepared_statement, param_name,
            std::move(value_ptr));
        return GorgonzolaSuccess;
    } catch (Exception& e) {
        return GorgonzolaError;
    }
}

gorgonzola_state gorgonzola_prepared_statement_bind_int32(gorgonzola_prepared_statement* prepared_statement,
    const char* param_name, int32_t value) {
    try {
        auto value_ptr = std::make_unique<Value>(value);
        gorgonzola_prepared_statement_bind_cpp_value(prepared_statement, param_name,
            std::move(value_ptr));
        return GorgonzolaSuccess;
    } catch (Exception& e) {
        return GorgonzolaError;
    }
}

gorgonzola_state gorgonzola_prepared_statement_bind_int16(gorgonzola_prepared_statement* prepared_statement,
    const char* param_name, int16_t value) {
    try {
        auto value_ptr = std::make_unique<Value>(value);
        gorgonzola_prepared_statement_bind_cpp_value(prepared_statement, param_name,
            std::move(value_ptr));
        return GorgonzolaSuccess;
    } catch (Exception& e) {
        return GorgonzolaError;
    }
}

gorgonzola_state gorgonzola_prepared_statement_bind_int8(gorgonzola_prepared_statement* prepared_statement,
    const char* param_name, int8_t value) {
    try {
        auto value_ptr = std::make_unique<Value>(value);
        gorgonzola_prepared_statement_bind_cpp_value(prepared_statement, param_name,
            std::move(value_ptr));
        return GorgonzolaSuccess;
    } catch (Exception& e) {
        return GorgonzolaError;
    }
}

gorgonzola_state gorgonzola_prepared_statement_bind_uint64(gorgonzola_prepared_statement* prepared_statement,
    const char* param_name, uint64_t value) {
    try {
        auto value_ptr = std::make_unique<Value>(value);
        gorgonzola_prepared_statement_bind_cpp_value(prepared_statement, param_name,
            std::move(value_ptr));
        return GorgonzolaSuccess;
    } catch (Exception& e) {
        return GorgonzolaError;
    }
}

gorgonzola_state gorgonzola_prepared_statement_bind_uint32(gorgonzola_prepared_statement* prepared_statement,
    const char* param_name, uint32_t value) {
    try {
        auto value_ptr = std::make_unique<Value>(value);
        gorgonzola_prepared_statement_bind_cpp_value(prepared_statement, param_name,
            std::move(value_ptr));
        return GorgonzolaSuccess;
    } catch (Exception& e) {
        return GorgonzolaError;
    }
}

gorgonzola_state gorgonzola_prepared_statement_bind_uint16(gorgonzola_prepared_statement* prepared_statement,
    const char* param_name, uint16_t value) {
    try {
        auto value_ptr = std::make_unique<Value>(value);
        gorgonzola_prepared_statement_bind_cpp_value(prepared_statement, param_name,
            std::move(value_ptr));
        return GorgonzolaSuccess;
    } catch (Exception& e) {
        return GorgonzolaError;
    }
}

gorgonzola_state gorgonzola_prepared_statement_bind_uint8(gorgonzola_prepared_statement* prepared_statement,
    const char* param_name, uint8_t value) {
    try {
        auto value_ptr = std::make_unique<Value>(value);
        gorgonzola_prepared_statement_bind_cpp_value(prepared_statement, param_name,
            std::move(value_ptr));
        return GorgonzolaSuccess;
    } catch (Exception& e) {
        return GorgonzolaError;
    }
}

gorgonzola_state gorgonzola_prepared_statement_bind_double(gorgonzola_prepared_statement* prepared_statement,
    const char* param_name, double value) {
    try {
        auto value_ptr = std::make_unique<Value>(value);
        gorgonzola_prepared_statement_bind_cpp_value(prepared_statement, param_name,
            std::move(value_ptr));
        return GorgonzolaSuccess;
    } catch (Exception& e) {
        return GorgonzolaError;
    }
}

gorgonzola_state gorgonzola_prepared_statement_bind_float(gorgonzola_prepared_statement* prepared_statement,
    const char* param_name, float value) {
    try {
        auto value_ptr = std::make_unique<Value>(value);
        gorgonzola_prepared_statement_bind_cpp_value(prepared_statement, param_name,
            std::move(value_ptr));
        return GorgonzolaSuccess;
    } catch (Exception& e) {
        return GorgonzolaError;
    }
}

gorgonzola_state gorgonzola_prepared_statement_bind_date(gorgonzola_prepared_statement* prepared_statement,
    const char* param_name, gorgonzola_date_t value) {
    try {
        auto value_ptr = std::make_unique<Value>(date_t(value.days));
        gorgonzola_prepared_statement_bind_cpp_value(prepared_statement, param_name,
            std::move(value_ptr));
        return GorgonzolaSuccess;
    } catch (Exception& e) {
        return GorgonzolaError;
    }
}

gorgonzola_state gorgonzola_prepared_statement_bind_timestamp_ns(gorgonzola_prepared_statement* prepared_statement,
    const char* param_name, gorgonzola_timestamp_ns_t value) {
    try {
        auto value_ptr = std::make_unique<Value>(timestamp_ns_t(value.value));
        gorgonzola_prepared_statement_bind_cpp_value(prepared_statement, param_name,
            std::move(value_ptr));
        return GorgonzolaSuccess;
    } catch (Exception& e) {
        return GorgonzolaError;
    }
}

gorgonzola_state gorgonzola_prepared_statement_bind_timestamp_ms(gorgonzola_prepared_statement* prepared_statement,
    const char* param_name, gorgonzola_timestamp_ms_t value) {
    try {
        auto value_ptr = std::make_unique<Value>(timestamp_ms_t(value.value));
        gorgonzola_prepared_statement_bind_cpp_value(prepared_statement, param_name,
            std::move(value_ptr));
        return GorgonzolaSuccess;
    } catch (Exception& e) {
        return GorgonzolaError;
    }
}

gorgonzola_state gorgonzola_prepared_statement_bind_timestamp_sec(gorgonzola_prepared_statement* prepared_statement,
    const char* param_name, gorgonzola_timestamp_sec_t value) {
    try {
        auto value_ptr = std::make_unique<Value>(timestamp_sec_t(value.value));
        gorgonzola_prepared_statement_bind_cpp_value(prepared_statement, param_name,
            std::move(value_ptr));
        return GorgonzolaSuccess;
    } catch (Exception& e) {
        return GorgonzolaError;
    }
}

gorgonzola_state gorgonzola_prepared_statement_bind_timestamp_tz(gorgonzola_prepared_statement* prepared_statement,
    const char* param_name, gorgonzola_timestamp_tz_t value) {
    try {
        auto value_ptr = std::make_unique<Value>(timestamp_tz_t(value.value));
        gorgonzola_prepared_statement_bind_cpp_value(prepared_statement, param_name,
            std::move(value_ptr));
        return GorgonzolaSuccess;
    } catch (Exception& e) {
        return GorgonzolaError;
    }
}

gorgonzola_state gorgonzola_prepared_statement_bind_timestamp(gorgonzola_prepared_statement* prepared_statement,
    const char* param_name, gorgonzola_timestamp_t value) {
    try {
        auto value_ptr = std::make_unique<Value>(timestamp_t(value.value));
        gorgonzola_prepared_statement_bind_cpp_value(prepared_statement, param_name,
            std::move(value_ptr));
        return GorgonzolaSuccess;
    } catch (Exception& e) {
        return GorgonzolaError;
    }
}

gorgonzola_state gorgonzola_prepared_statement_bind_interval(gorgonzola_prepared_statement* prepared_statement,
    const char* param_name, gorgonzola_interval_t value) {
    try {
        auto value_ptr =
            std::make_unique<Value>(interval_t(value.months, value.days, value.micros));
        gorgonzola_prepared_statement_bind_cpp_value(prepared_statement, param_name,
            std::move(value_ptr));
        return GorgonzolaSuccess;
    } catch (Exception& e) {
        return GorgonzolaError;
    }
}

gorgonzola_state gorgonzola_prepared_statement_bind_string(gorgonzola_prepared_statement* prepared_statement,
    const char* param_name, const char* value) {
    try {
        auto value_ptr = std::make_unique<Value>(std::string(value));
        gorgonzola_prepared_statement_bind_cpp_value(prepared_statement, param_name,
            std::move(value_ptr));
        return GorgonzolaSuccess;
    } catch (Exception& e) {
        return GorgonzolaError;
    }
}

gorgonzola_state gorgonzola_prepared_statement_bind_value(gorgonzola_prepared_statement* prepared_statement,
    const char* param_name, gorgonzola_value* value) {
    try {
        auto value_ptr = std::make_unique<Value>(*static_cast<Value*>(value->_value));
        gorgonzola_prepared_statement_bind_cpp_value(prepared_statement, param_name,
            std::move(value_ptr));
        return GorgonzolaSuccess;
    } catch (Exception& e) {
        return GorgonzolaError;
    }
}
