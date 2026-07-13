#include "gorgonzola/result.h"
#include "api_impl.h"
#include "processor/result/flat_tuple.h"

namespace gorgonzola {

// Value methods
Value::Value() = default;
Value::~Value() = default;
Value::Value(Value&&) noexcept = default;
Value& Value::operator=(Value&&) noexcept = default;
void Value::setImpl(std::unique_ptr<ValueImpl> impl) { impl_ = std::move(impl); }

bool Value::isNull() const { return false; } // TODO
int64_t Value::getInt64() const { return 0; } // TODO
double Value::getDouble() const { return 0.0; } // TODO
bool Value::getBool() const { return false; } // TODO
std::string Value::getString() const { return ""; } // TODO

// Row methods
Row::Row() = default;
Row::~Row() = default;
Row::Row(Row&&) noexcept = default;
Row& Row::operator=(Row&&) noexcept = default;
void Row::setImpl(std::unique_ptr<RowImpl> impl) { impl_ = std::move(impl); }

const Value* Row::getValue(size_t index) const { return nullptr; } // TODO

// Result methods
Result::Result() = default;
Result::~Result() = default;
Result::Result(Result&&) noexcept = default;
Result& Result::operator=(Result&&) noexcept = default;
void Result::setImpl(std::unique_ptr<ResultImpl> impl) { impl_ = std::move(impl); }

bool Result::isSuccess() const {
    return impl_ ? impl_->getInternalResult()->isSuccess() : false;
}

std::string Result::getErrorMessage() const {
    return impl_ ? impl_->getInternalResult()->getErrorMessage() : "No result";
}

size_t Result::getNumColumns() const {
    return impl_ ? impl_->getInternalResult()->getNumColumns() : 0;
}

std::vector<std::string> Result::getColumnNames() const {
    return impl_ ? impl_->getInternalResult()->getColumnNames() : std::vector<std::string>();
}

size_t Result::getNumTuples() const {
    return impl_ ? impl_->getInternalResult()->getNumTuples() : 0;
}

bool Result::hasNext() const {
    return impl_ ? impl_->getInternalResult()->hasNext() : false;
}

const Row* Result::getNext() {
    // TODO: fetch getNext() from QueryResult, populate a Row, and return it.
    if (impl_) {
        impl_->getInternalResult()->getNext();
    }
    return nullptr;
}

} // namespace gorgonzola
