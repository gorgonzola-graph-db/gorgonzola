#pragma once

#include <cstdint>
#include <memory>
#include <string>
#include <vector>

#include "gorgonzola/export.h"

namespace gorgonzola {

class ValueImpl;
class RowImpl;
class ResultImpl;

/**
 * @brief Represents a single typed value from the database.
 */
class GORGONZOLA_API Value {
public:
    Value();
    ~Value();
    Value(const Value&) = delete;
    Value& operator=(const Value&) = delete;
    Value(Value&&) noexcept;
    Value& operator=(Value&&) noexcept;

    bool isNull() const;
    int64_t getInt64() const;
    double getDouble() const;
    bool getBool() const;
    std::string getString() const;

    // Internal use only
    ValueImpl* getImpl() const { return impl_.get(); }
    void setImpl(std::unique_ptr<ValueImpl> impl);

private:
    std::unique_ptr<ValueImpl> impl_;
};

/**
 * @brief Represents a single row of result values.
 */
class GORGONZOLA_API Row {
public:
    Row();
    ~Row();
    Row(const Row&) = delete;
    Row& operator=(const Row&) = delete;
    Row(Row&&) noexcept;
    Row& operator=(Row&&) noexcept;

    const Value* getValue(size_t index) const;

    // Internal use only
    RowImpl* getImpl() const { return impl_.get(); }
    void setImpl(std::unique_ptr<RowImpl> impl);

private:
    std::unique_ptr<RowImpl> impl_;
};

/**
 * @brief An iterable result returned from query execution.
 */
class GORGONZOLA_API Result {
public:
    Result();
    ~Result();
    Result(const Result&) = delete;
    Result& operator=(const Result&) = delete;
    Result(Result&&) noexcept;
    Result& operator=(Result&&) noexcept;

    bool isSuccess() const;
    std::string getErrorMessage() const;

    size_t getNumColumns() const;
    std::vector<std::string> getColumnNames() const;

    // Total number of tuples (may require full evaluation)
    size_t getNumTuples() const;

    // Iteration
    bool hasNext() const;
    const Row* getNext();

    // Internal use only
    ResultImpl* getImpl() const { return impl_.get(); }
    void setImpl(std::unique_ptr<ResultImpl> impl);

private:
    std::unique_ptr<ResultImpl> impl_;
};

} // namespace gorgonzola
