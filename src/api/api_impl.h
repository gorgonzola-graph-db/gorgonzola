#pragma once

#include "gorgonzola/database.h"
#include "gorgonzola/query.h"
#include "gorgonzola/result.h"
#include "gorgonzola/session.h"
#include "gorgonzola/transaction.h"
#include "main/connection.h"
#include "main/database.h"
#include "main/query_result.h"
#include "processor/result/flat_tuple.h"

namespace gorgonzola {

class DatabaseImpl {
public:
    explicit DatabaseImpl(std::unique_ptr<main::Database> db) : db_(std::move(db)) {}

    main::Database* getInternalDatabase() const { return db_.get(); }
    void setInternalDatabase(std::unique_ptr<main::Database> db) { db_ = std::move(db); }

    void poison(Status s) {
        poisoned_ = true;
        lastError_ = std::move(s);
    }

    bool isPoisoned() const { return poisoned_; }
    Status getLastError() const { return lastError_; }

private:
    std::unique_ptr<main::Database> db_;
    std::atomic<bool> poisoned_{false};
    Status lastError_;
};

class SessionImpl {
public:
    explicit SessionImpl(DatabaseImpl* dbImpl)
        : dbImpl_(dbImpl),
          connection_(std::make_unique<main::Connection>(dbImpl->getInternalDatabase())) {}

    main::Connection* getConnection() const { return connection_.get(); }
    DatabaseImpl* getDatabaseImpl() const { return dbImpl_; }

private:
    DatabaseImpl* dbImpl_;
    std::unique_ptr<main::Connection> connection_;
};

class TransactionImpl {
public:
    explicit TransactionImpl(SessionImpl* session) : session_(session) {}

    Status commit() {
        if (!session_)
            return Status::Error("Invalid session in transaction");
        auto result = session_->getConnection()->query("COMMIT");
        if (!result->isSuccess())
            return Status::Error(result->getErrorMessage());
        return Status::Success();
    }

    Status rollback() {
        if (!session_)
            return Status::Error("Invalid session in transaction");
        auto result = session_->getConnection()->query("ROLLBACK");
        if (!result->isSuccess())
            return Status::Error(result->getErrorMessage());
        return Status::Success();
    }

private:
    SessionImpl* session_;
};

class QueryImpl {
public:
    // Future-proof for parameters, etc.
};

#include "common/types/value/value.h"

class ValueImpl {
public:
    explicit ValueImpl(const common::Value* val) : val_(val) {}
    const common::Value* getInternalValue() const { return val_; }

private:
    const common::Value* val_;
};

class RowImpl {
public:
    explicit RowImpl(std::shared_ptr<processor::FlatTuple> tuple) : tuple_(std::move(tuple)) {
        if (tuple_) {
            values_.reserve(tuple_->len());
            for (common::idx_t i = 0; i < tuple_->len(); ++i) {
                auto valObj = std::make_unique<Value>();
                valObj->setImpl(std::make_unique<ValueImpl>(tuple_->getValue(i)));
                values_.push_back(std::move(valObj));
            }
        }
    }

    processor::FlatTuple* getInternalTuple() const { return tuple_.get(); }
    const Value* getValue(size_t index) const {
        if (index < values_.size()) {
            return values_[index].get();
        }
        return nullptr;
    }

private:
    std::shared_ptr<processor::FlatTuple> tuple_;
    std::vector<std::unique_ptr<Value>> values_;
};

class ResultImpl {
public:
    explicit ResultImpl(std::unique_ptr<main::QueryResult> result) : result_(std::move(result)) {}

    main::QueryResult* getInternalResult() const { return result_.get(); }

    const Row* getNextRow() {
        if (!result_ || !result_->hasNext())
            return nullptr;
        currentRow_ = std::make_unique<Row>();
        currentRow_->setImpl(std::make_unique<RowImpl>(result_->getNext()));
        return currentRow_.get();
    }

private:
    std::unique_ptr<main::QueryResult> result_;
    std::unique_ptr<Row> currentRow_;
};

} // namespace gorgonzola
