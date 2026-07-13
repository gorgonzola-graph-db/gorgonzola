#include "c_api/gorgonzola.h"
#include "c_api_utils.h"
#include "common/exception/exception.h"
#include "main/gorgonzola.h"

namespace gorgonzola {
namespace common {
class Value;
}
} // namespace gorgonzola

using namespace gorgonzola::common;
using namespace gorgonzola::main;

gorgonzola_state gorgonzola_connection_init(gorgonzola_database* database, gorgonzola_connection* out_connection) {
    if (!database || !out_connection) {
        if (out_connection) out_connection->_connection = nullptr;
        return GorgonzolaError;
    }
    VALIDATE_HANDLE_RET(database->_database, Database, GorgonzolaError)
        GORGONZOLA_C_API_BEGIN
        out_connection->_connection = new Connection(static_cast<Database*>(database->_database));
        gorgonzola::c_api::HandleRegistry::getInstance().registerHandle(out_connection->_connection, gorgonzola::c_api::HandleType::Connection);
    } catch (...) {
        gorgonzola::c_api::translate_exception();
        out_connection->_connection = nullptr;
        return GorgonzolaError;
    }
    return GorgonzolaSuccess;
}

void gorgonzola_connection_destroy(gorgonzola_connection* connection) {
    if (connection == nullptr) {
        return;
    }
    if (connection->_connection != nullptr) {
        gorgonzola::c_api::HandleRegistry::getInstance().unregisterHandle(connection->_connection);
        delete static_cast<Connection*>(connection->_connection);
    }
}

gorgonzola_state gorgonzola_connection_set_max_num_thread_for_exec(gorgonzola_connection* connection,
    uint64_t num_threads) {
    if (!connection) return GorgonzolaError;
    VALIDATE_HANDLE_RET(connection->_connection, Connection, GorgonzolaError)
        GORGONZOLA_C_API_BEGIN
        static_cast<Connection*>(connection->_connection)->setMaxNumThreadForExec(num_threads);
    } catch (...) {
        gorgonzola::c_api::translate_exception();
        return GorgonzolaError;
    }
    return GorgonzolaSuccess;
}

gorgonzola_state gorgonzola_connection_get_max_num_thread_for_exec(gorgonzola_connection* connection,
    uint64_t* out_result) {
    if (!connection) return GorgonzolaError;
    VALIDATE_HANDLE_RET(connection->_connection, Connection, GorgonzolaError)
        GORGONZOLA_C_API_BEGIN
        *out_result = static_cast<Connection*>(connection->_connection)->getMaxNumThreadForExec();
    } catch (...) {
        gorgonzola::c_api::translate_exception();
        return GorgonzolaError;
    }
    return GorgonzolaSuccess;
}

gorgonzola_state gorgonzola_connection_query(gorgonzola_connection* connection, const char* query,
    gorgonzola_query_result* out_query_result) {
    if (!connection) return GorgonzolaError;
    VALIDATE_HANDLE_RET(connection->_connection, Connection, GorgonzolaError)
        GORGONZOLA_C_API_BEGIN
        auto query_result =
            static_cast<Connection*>(connection->_connection)->query(query).release();
        if (query_result == nullptr) {
            return GorgonzolaError;
        }
        out_query_result->_query_result = query_result;
        gorgonzola::c_api::HandleRegistry::getInstance().registerHandle(query_result, gorgonzola::c_api::HandleType::QueryResult);
        out_query_result->_is_owned_by_cpp = false;
        if (!query_result->isSuccess()) {
            return GorgonzolaError;
        }
        return GorgonzolaSuccess;
    } catch (...) {
        gorgonzola::c_api::translate_exception();
        return GorgonzolaError;
    }
}

gorgonzola_state gorgonzola_connection_prepare(gorgonzola_connection* connection, const char* query,
    gorgonzola_prepared_statement* out_prepared_statement) {
    if (!connection) return GorgonzolaError;
    VALIDATE_HANDLE_RET(connection->_connection, Connection, GorgonzolaError)
        GORGONZOLA_C_API_BEGIN
        auto prepared_statement =
            static_cast<Connection*>(connection->_connection)->prepare(query).release();
        if (prepared_statement == nullptr) {
            return GorgonzolaError;
        }
        out_prepared_statement->_prepared_statement = prepared_statement;
        gorgonzola::c_api::HandleRegistry::getInstance().registerHandle(prepared_statement, gorgonzola::c_api::HandleType::PreparedStatement);
        out_prepared_statement->_bound_values =
            new std::unordered_map<std::string, std::unique_ptr<Value>>;
        return GorgonzolaSuccess;
    } catch (...) {
        gorgonzola::c_api::translate_exception();
        return GorgonzolaError;
    }
    return GorgonzolaSuccess;
}

gorgonzola_state gorgonzola_connection_execute(gorgonzola_connection* connection,
    gorgonzola_prepared_statement* prepared_statement, gorgonzola_query_result* out_query_result) {
    if (connection == nullptr || connection->_connection == nullptr ||
        prepared_statement == nullptr || prepared_statement->_prepared_statement == nullptr ||
        prepared_statement->_bound_values == nullptr) {
        return GorgonzolaError;
    }
        GORGONZOLA_C_API_BEGIN
        auto prepared_statement_ptr =
            static_cast<PreparedStatement*>(prepared_statement->_prepared_statement);
        auto bound_values = static_cast<std::unordered_map<std::string, std::unique_ptr<Value>>*>(
            prepared_statement->_bound_values);

        // Must copy the parameters for safety, and so that the parameters in the prepared statement
        // stay the same.
        std::unordered_map<std::string, std::unique_ptr<Value>> copied_bound_values;
        for (auto& [name, value] : *bound_values) {
            copied_bound_values.emplace(name, value->copy());
        }

        auto query_result =
            static_cast<Connection*>(connection->_connection)
                ->executeWithParams(prepared_statement_ptr, std::move(copied_bound_values))
                .release();
        if (query_result == nullptr) {
            return GorgonzolaError;
        }
        out_query_result->_query_result = query_result;
        gorgonzola::c_api::HandleRegistry::getInstance().registerHandle(query_result, gorgonzola::c_api::HandleType::QueryResult);
        out_query_result->_is_owned_by_cpp = false;
        if (!query_result->isSuccess()) {
            return GorgonzolaError;
        }
        return GorgonzolaSuccess;
    } catch (...) {
        gorgonzola::c_api::translate_exception();
        return GorgonzolaError;
    }
}
void gorgonzola_connection_interrupt(gorgonzola_connection* connection) {
    static_cast<Connection*>(connection->_connection)->interrupt();
}

gorgonzola_state gorgonzola_connection_set_query_timeout(gorgonzola_connection* connection, uint64_t timeout_in_ms) {
    if (!connection) return GorgonzolaError;
    VALIDATE_HANDLE_RET(connection->_connection, Connection, GorgonzolaError)
        GORGONZOLA_C_API_BEGIN
        static_cast<Connection*>(connection->_connection)->setQueryTimeOut(timeout_in_ms);
    } catch (...) {
        gorgonzola::c_api::translate_exception();
        return GorgonzolaError;
    }
    return GorgonzolaSuccess;
}
