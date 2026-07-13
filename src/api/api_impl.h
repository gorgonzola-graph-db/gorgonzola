#pragma once

#include "gorgonzola/database.h"
#include "gorgonzola/session.h"
#include "gorgonzola/transaction.h"
#include "gorgonzola/query.h"
#include "gorgonzola/result.h"

#include "main/database.h"
#include "main/connection.h"
#include "main/query_result.h"

namespace gorgonzola {

class DatabaseImpl {
public:
    explicit DatabaseImpl(std::unique_ptr<main::Database> db) 
        : database_(std::move(db)) {}

    main::Database* getInternalDatabase() const { return database_.get(); }
private:
    std::unique_ptr<main::Database> database_;
};

class SessionImpl {
public:
    explicit SessionImpl(main::Database* db) 
        : connection_(std::make_unique<main::Connection>(db)) {}

    main::Connection* getConnection() const { return connection_.get(); }
private:
    std::unique_ptr<main::Connection> connection_;
};

class TransactionImpl {
public:
    explicit TransactionImpl(SessionImpl* session) : session_(session) {}

    Status commit() {
        if (!session_) return Status::Error("Invalid session in transaction");
        auto result = session_->getConnection()->query("COMMIT");
        if (!result->isSuccess()) return Status::Error(result->getErrorMessage());
        return Status::Success();
    }

    Status rollback() {
        if (!session_) return Status::Error("Invalid session in transaction");
        auto result = session_->getConnection()->query("ROLLBACK");
        if (!result->isSuccess()) return Status::Error(result->getErrorMessage());
        return Status::Success();
    }
private:
    SessionImpl* session_;
};

class QueryImpl {
public:
    // Future-proof for parameters, etc.
};

class ValueImpl {
    // TODO: Wrap common::Value
};

class RowImpl {
public:
    // TODO: We need a way to store the current FlatTuple and expose its fields as Value.
};

class ResultImpl {
public:
    explicit ResultImpl(std::unique_ptr<main::QueryResult> result) 
        : result_(std::move(result)) {}

    main::QueryResult* getInternalResult() const { return result_.get(); }
private:
    std::unique_ptr<main::QueryResult> result_;
};

} // namespace gorgonzola
