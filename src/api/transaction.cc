#include "gorgonzola/transaction.h"
#include "api_impl.h"

namespace gorgonzola {

Transaction::Transaction(std::unique_ptr<TransactionImpl> impl)
    : impl_(std::move(impl)), is_finished_(false) {}

Transaction::~Transaction() {
    if (!is_finished_ && impl_) {
        // Automatically rollback if not explicitly committed or rolled back
        impl_->rollback();
    }
}

Transaction::Transaction(Transaction&& other) noexcept 
    : impl_(std::move(other.impl_)), is_finished_(other.is_finished_) {
    other.is_finished_ = true; // prevent rollback from other
}

Transaction& Transaction::operator=(Transaction&& other) noexcept {
    if (this != &other) {
        if (!is_finished_ && impl_) {
            impl_->rollback();
        }
        impl_ = std::move(other.impl_);
        is_finished_ = other.is_finished_;
        other.is_finished_ = true;
    }
    return *this;
}

Status Transaction::commit() {
    if (is_finished_) {
        return Status::Error("Transaction already finished");
    }
    is_finished_ = true;
    return impl_->commit();
}

Status Transaction::rollback() {
    if (is_finished_) {
        return Status::Error("Transaction already finished");
    }
    is_finished_ = true;
    return impl_->rollback();
}

} // namespace gorgonzola
