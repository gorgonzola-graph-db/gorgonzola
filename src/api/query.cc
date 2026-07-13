#include "gorgonzola/query.h"
#include "api_impl.h"

namespace gorgonzola {

Query::Query(std::string queryText) 
    : queryText_(std::move(queryText)), impl_(std::make_unique<QueryImpl>()) {}

Query::~Query() = default;

Query::Query(Query&&) noexcept = default;
Query& Query::operator=(Query&&) noexcept = default;

const std::string& Query::getQueryText() const {
    return queryText_;
}

void Query::setImpl(std::unique_ptr<QueryImpl> impl) {
    impl_ = std::move(impl);
}

} // namespace gorgonzola
