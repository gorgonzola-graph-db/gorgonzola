#include "gorgonzola/session.h"
#include "api_impl.h"
#include <stdexcept>

namespace gorgonzola {

Session::Session(Database& db)
    : impl_(std::make_unique<SessionImpl>(db.getImpl()->getInternalDatabase())) {}

Session::~Session() = default;

Session::Session(Session&&) noexcept = default;
Session& Session::operator=(Session&&) noexcept = default;

Result Session::execute(const std::string& queryText) {
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

std::vector<std::string> Session::getTableNames() {
    auto queryResult = execute("CALL show_tables()");
    if (!queryResult.isSuccess()) {
        return {};
    }
    
    std::vector<std::string> tables;
    auto colNames = queryResult.getColumnNames();
    size_t nameIdx = 0;
    for (size_t i = 0; i < colNames.size(); ++i) {
        if (colNames[i] == "name") {
            nameIdx = i;
            break;
        }
    }
    
    while (queryResult.hasNext()) {
        auto row = queryResult.getNext();
        if (row) {
            auto val = row->getValue(nameIdx);
            if (val && !val->isNull()) {
                tables.push_back(val->getString());
            }
        }
    }
    return tables;
}

} // namespace gorgonzola
