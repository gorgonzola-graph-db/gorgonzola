#pragma once

#include <memory>
#include <string>
#include "gorgonzola/export.h"
#include "gorgonzola/database.h"
#include "gorgonzola/query.h"
#include "gorgonzola/result.h"
#include "gorgonzola/transaction.h"

namespace gorgonzola {

class SessionImpl;

/**
 * @brief Represents a single execution context for executing queries.
 * 
 * IMPORTANT: Not thread-safe. Each thread must create its own session, 
 * or access to a session must be protected by a mutex.
 */
class GORGONZOLA_API Session {
public:
    /**
     * @brief Creates a session attached to the given Database.
     */
    explicit Session(Database& db);
    ~Session();

    Session(const Session&) = delete;
    Session& operator=(const Session&) = delete;
    
    Session(Session&&) noexcept;
    Session& operator=(Session&&) noexcept;

    /**
     * @brief Executes a plain cypher query string.
     * @return Result containing data or error state.
     */
    Result execute(const std::string& queryText);

    /**
     * @brief Executes a parameterized/prepared query.
     * @return Result containing data or error state.
     */
    Result execute(const Query& query);

    /**
     * @brief Prepares a query without executing it.
     */
    Query prepare(const std::string& queryText);

    /**
     * @brief Begins a new explicit transaction.
     */
    Transaction beginTransaction();

    /**
     * @brief Safely interrupts any currently executing query on this session.
     * This is the ONLY method that is thread-safe on a Session.
     */
    void interrupt();

    /**
     * @brief Retrieves the names of all tables currently defined in the database.
     * @return A list of table names.
     */
    std::vector<std::string> getTableNames();

    // Internal use only
    SessionImpl* getImpl() const { return impl_.get(); }

private:
    std::unique_ptr<SessionImpl> impl_;
};

} // namespace gorgonzola
