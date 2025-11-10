#include "c_api/gorgonzola.h"
#include "common/types/types.h"

using namespace gorgonzola::common;

namespace gorgonzola::common {
struct CAPIHelper {
    static inline LogicalType* createLogicalType(LogicalTypeID typeID,
        std::unique_ptr<ExtraTypeInfo> extraTypeInfo) {
        return new LogicalType(typeID, std::move(extraTypeInfo));
    }
};
} // namespace gorgonzola::common

void gorgonzola_data_type_create(gorgonzola_data_type_id id, gorgonzola_logical_type* child_type,
    uint64_t num_elements_in_array, gorgonzola_logical_type* out_data_type) {
    uint8_t data_type_id_u8 = id;
    LogicalType* data_type = nullptr;
    auto logicalTypeID = static_cast<LogicalTypeID>(data_type_id_u8);
    if (child_type == nullptr) {
        data_type = new LogicalType(logicalTypeID);
    } else {
        auto child_type_pty = static_cast<LogicalType*>(child_type->_data_type)->copy();
        auto extraTypeInfo =
            num_elements_in_array > 0 ?
                std::make_unique<ArrayTypeInfo>(std::move(child_type_pty), num_elements_in_array) :
                std::make_unique<ListTypeInfo>(std::move(child_type_pty));
        data_type = CAPIHelper::createLogicalType(logicalTypeID, std::move(extraTypeInfo));
    }
    out_data_type->_data_type = data_type;
}

void gorgonzola_data_type_clone(gorgonzola_logical_type* data_type, gorgonzola_logical_type* out_data_type) {
    out_data_type->_data_type =
        new LogicalType(static_cast<LogicalType*>(data_type->_data_type)->copy());
}

void gorgonzola_data_type_destroy(gorgonzola_logical_type* data_type) {
    if (data_type == nullptr) {
        return;
    }
    if (data_type->_data_type != nullptr) {
        delete static_cast<LogicalType*>(data_type->_data_type);
    }
}

bool gorgonzola_data_type_equals(gorgonzola_logical_type* data_type1, gorgonzola_logical_type* data_type2) {
    return *static_cast<LogicalType*>(data_type1->_data_type) ==
           *static_cast<LogicalType*>(data_type2->_data_type);
}

gorgonzola_data_type_id gorgonzola_data_type_get_id(gorgonzola_logical_type* data_type) {
    auto data_type_id_u8 =
        static_cast<uint8_t>(static_cast<LogicalType*>(data_type->_data_type)->getLogicalTypeID());
    return static_cast<gorgonzola_data_type_id>(data_type_id_u8);
}

gorgonzola_state gorgonzola_data_type_get_num_elements_in_array(gorgonzola_logical_type* data_type,
    uint64_t* out_result) {
    auto parent_type = static_cast<LogicalType*>(data_type->_data_type);
    if (parent_type->getLogicalTypeID() != LogicalTypeID::ARRAY) {
        return GorgonzolaError;
    }
    try {
        *out_result = ArrayType::getNumElements(*parent_type);
    } catch (Exception& e) {
        return GorgonzolaError;
    }
    return GorgonzolaSuccess;
}
