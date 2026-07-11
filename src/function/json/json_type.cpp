#include "function/json/json_type.h"

#include "common/exception/binder.h"

namespace gorgonzola {
namespace json_extension {

constexpr char JSON_TYPE_NAME[] = "JSON";

common::LogicalType JsonType::getJsonType() {
    auto type = common::LogicalType(common::LogicalTypeID::STRING, common::TypeCategory::UDT);
    type.setExtraTypeInfo(std::make_unique<common::UDTTypeInfo>(JSON_TYPE_NAME));
    return type;
}

bool JsonType::isJson(const common::LogicalType& type) {
    if (!type.isInternalType() &&
        type.getExtraTypeInfo()->constPtrCast<common::UDTTypeInfo>()->getTypeName() ==
            JSON_TYPE_NAME) {
        return true;
    }
    return false;
}

} // namespace json_extension
} // namespace gorgonzola
