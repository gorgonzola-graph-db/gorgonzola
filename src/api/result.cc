#include "gorgonzola/result.h"

#include "api_impl.h"
#include "processor/result/flat_tuple.h"

namespace gorgonzola {

// Value methods
Value::Value() = default;
Value::~Value() = default;
Value::Value(Value&&) noexcept = default;
Value& Value::operator=(Value&&) noexcept = default;
void Value::setImpl(std::unique_ptr<ValueImpl> impl) {
    impl_ = std::move(impl);
}

bool Value::isNull() const {
    return impl_ ? impl_->getInternalValue()->isNull() : true;
}
int64_t Value::getInt64() const {
    return impl_ ? impl_->getInternalValue()->getValue<int64_t>() : 0;
}
double Value::getDouble() const {
    return impl_ ? impl_->getInternalValue()->getValue<double>() : 0.0;
}
bool Value::getBool() const {
    return impl_ ? impl_->getInternalValue()->getValue<bool>() : false;
}
std::string Value::getString() const {
    return impl_ ? impl_->getInternalValue()->toString() : "";
}

// Row methods
Row::Row() = default;
Row::~Row() = default;
Row::Row(Row&&) noexcept = default;
Row& Row::operator=(Row&&) noexcept = default;
void Row::setImpl(std::unique_ptr<RowImpl> impl) {
    impl_ = std::move(impl);
}

const Value* Row::getValue(size_t index) const {
    return impl_ ? impl_->getValue(index) : nullptr;
}

// Result methods
Result::Result() = default;
Result::~Result() = default;
Result::Result(Result&&) noexcept = default;
Result& Result::operator=(Result&&) noexcept = default;
void Result::setImpl(std::unique_ptr<ResultImpl> impl) {
    impl_ = std::move(impl);
}

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
    return impl_ ? impl_->getNextRow() : nullptr;
}

} // namespace gorgonzola
