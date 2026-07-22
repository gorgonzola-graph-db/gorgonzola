#pragma once

#include <memory>

#include "gorgonzola/export.h"
#include "gorgonzola/status.h"

namespace gorgonzola {

class TransactionImpl;

/**
 * @brief Represents an active transaction.
 * If it goes out of scope without commit() being called, it is automatically rolled back.
 */
class GORGONZOLA_API Transaction {
public:
    // Designed to be constructed by Session, not directly by user.
    explicit Transaction(std::unique_ptr<TransactionImpl> impl);
    ~Transaction();

    Transaction(const Transaction&) = delete;
    Transaction& operator=(const Transaction&) = delete;

    Transaction(Transaction&&) noexcept;
    Transaction& operator=(Transaction&&) noexcept;

    /**
     * @brief Commits the transaction.
     * @return Status indicating success or failure.
     */
    Status commit();

    /**
     * @brief Rolls back the transaction.
     * @return Status indicating success or failure.
     */
    Status rollback();

    // Internal use only
    TransactionImpl* getImpl() const { return impl_.get(); }

private:
    std::unique_ptr<TransactionImpl> impl_;
    bool is_finished_;
};

} // namespace gorgonzola
