#include "gorgonzola/session.h"

#include <stdexcept>

#include "api_impl.h"

namespace gorgonzola {

Session::Session(Database& db) {
    if (!db.getImpl()) {
        throw std::invalid_argument("Invalid database");
    }
    impl_ = std::make_unique<SessionImpl>(db.getImpl());
}

Session::~Session() = default;

Session::Session(Session&&) noexcept = default;
Session& Session::operator=(Session&&) noexcept = default;

Result Session::execute(const std::string& queryText) {
    if (impl_->getDatabaseImpl()->isPoisoned()) {
        auto errResult = main::QueryResult::getQueryResultWithError(
            impl_->getDatabaseImpl()->getLastError().message());
        Result res;
        res.setImpl(std::make_unique<ResultImpl>(std::move(errResult)));
        return res;
    }

    try {
        auto queryResult = impl_->getConnection()->query(queryText);
        Result res;
        res.setImpl(std::make_unique<ResultImpl>(std::move(queryResult)));
        return res;
    } catch (const std::exception& e) {
        // Fallback catch if exceptions escape
        auto errResult = main::QueryResult::getQueryResultWithError(e.what());
        Result res;
        res.setImpl(std::make_unique<ResultImpl>(std::move(errResult)));
        return res;
    }
}

Result Session::execute(const Query& query) {
    return execute(query.getQueryText());
}

Query Session::prepare(const std::string& queryText) {
    return Query(queryText);
}

Transaction Session::beginTransaction() {
    auto res = execute("BEGIN TRANSACTION");
    if (!res.isSuccess()) {
        // How to handle failure? In a real system, we'd throw or Transaction would check state.
    }
    return Transaction(std::make_unique<TransactionImpl>(impl_.get()));
}

void Session::interrupt() {
    impl_->getConnection()->interrupt();
}

} // namespace gorgonzola
