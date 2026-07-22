#pragma once

#include <memory>
#include <string>

#include "gorgonzola/export.h"

namespace gorgonzola {

class QueryImpl;

/**
 * @brief Represents a query to be executed. Future-proofed for prepared statements.
 */
class GORGONZOLA_API Query {
public:
    explicit Query(std::string queryText);
    ~Query();

    Query(const Query&) = delete;
    Query& operator=(const Query&) = delete;

    Query(Query&&) noexcept;
    Query& operator=(Query&&) noexcept;

    const std::string& getQueryText() const;

    // Bind parameters can be added here in the future
    // e.g. void bind(const std::string& name, const Value& val);

    // Internal use only
    QueryImpl* getImpl() const { return impl_.get(); }
    void setImpl(std::unique_ptr<QueryImpl> impl);

private:
    std::string queryText_;
    std::unique_ptr<QueryImpl> impl_;
};

} // namespace gorgonzola
