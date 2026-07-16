#pragma once

#include "common/types/types.h"

namespace gorgonzola {
namespace common {

struct GORGONZOLA_API DecimalType {
    static uint32_t getPrecision(const LogicalType& type);
    static uint32_t getScale(const LogicalType& type);
    static std::string insertDecimalPoint(const std::string& value, uint32_t posFromEnd);
};

struct GORGONZOLA_API ListType {
    static const LogicalType& getChildType(const LogicalType& type);
};

struct GORGONZOLA_API ArrayType {
    static const LogicalType& getChildType(const LogicalType& type);
    static uint64_t getNumElements(const LogicalType& type);
};

struct GORGONZOLA_API StructType {
    static std::vector<const LogicalType*> getFieldTypes(const LogicalType& type);
    // since the field types isn't stored as a vector of LogicalTypes, we can't return vector<>&

    static const LogicalType& getFieldType(const LogicalType& type, struct_field_idx_t idx);

    static const LogicalType& getFieldType(const LogicalType& type, const std::string& key);

    static std::vector<std::string> getFieldNames(const LogicalType& type);

    static uint64_t getNumFields(const LogicalType& type);

    static const std::vector<StructField>& getFields(const LogicalType& type);

    static bool hasField(const LogicalType& type, const std::string& key);

    static const StructField& getField(const LogicalType& type, struct_field_idx_t idx);

    static const StructField& getField(const LogicalType& type, const std::string& key);

    static struct_field_idx_t getFieldIdx(const LogicalType& type, const std::string& key);
};

struct GORGONZOLA_API MapType {
    static const LogicalType& getKeyType(const LogicalType& type);

    static const LogicalType& getValueType(const LogicalType& type);
};

struct GORGONZOLA_API UnionType {
    static constexpr union_field_idx_t TAG_FIELD_IDX = 0;

    static constexpr auto TAG_FIELD_TYPE = LogicalTypeID::UINT16;

    static constexpr char TAG_FIELD_NAME[] = "tag";

    static union_field_idx_t getInternalFieldIdx(union_field_idx_t idx);

    static std::string getFieldName(const LogicalType& type, union_field_idx_t idx);

    static const LogicalType& getFieldType(const LogicalType& type, union_field_idx_t idx);

    static const LogicalType& getFieldType(const LogicalType& type, const std::string& key);

    static uint64_t getNumFields(const LogicalType& type);

    static bool hasField(const LogicalType& type, const std::string& key);

    static union_field_idx_t getFieldIdx(const LogicalType& type, const std::string& key);
};

struct PhysicalTypeUtils {
    static std::string toString(PhysicalTypeID physicalType);
    static uint32_t getFixedTypeSize(PhysicalTypeID physicalType);
};

struct GORGONZOLA_API LogicalTypeUtils {
    static std::string toString(LogicalTypeID dataTypeID);
    static std::string toString(const std::vector<LogicalType>& dataTypes);
    static std::string toString(const std::vector<LogicalTypeID>& dataTypeIDs);
    static uint32_t getRowLayoutSize(const LogicalType& logicalType);
    static bool isDate(const LogicalType& dataType);
    static bool isDate(const LogicalTypeID& dataType);
    static bool isTimestamp(const LogicalType& dataType);
    static bool isTimestamp(const LogicalTypeID& dataType);
    static bool isUnsigned(const LogicalType& dataType);
    static bool isUnsigned(const LogicalTypeID& dataType);
    static bool isIntegral(const LogicalType& dataType);
    static bool isIntegral(const LogicalTypeID& dataType);
    static bool isNumerical(const LogicalType& dataType);
    static bool isNumerical(const LogicalTypeID& dataType);
    static bool isFloatingPoint(const LogicalTypeID& dataType);
    static bool isNested(const LogicalType& dataType);
    static bool isNested(LogicalTypeID logicalTypeID);
    static std::vector<LogicalTypeID> getAllValidComparableLogicalTypes();
    static std::vector<LogicalTypeID> getNumericalLogicalTypeIDs();
    static std::vector<LogicalTypeID> getIntegerTypeIDs();
    static std::vector<LogicalTypeID> getFloatingPointTypeIDs();
    static std::vector<LogicalTypeID> getAllValidLogicTypeIDs();
    static std::vector<LogicalType> getAllValidLogicTypes();
    static bool tryGetMaxLogicalType(const LogicalType& left, const LogicalType& right,
        LogicalType& result);
    static bool tryGetMaxLogicalType(const std::vector<LogicalType>& types, LogicalType& result);

    // Differs from tryGetMaxLogicalType because it treats string as a maximal type, instead of a
    // minimal type. as such, it will always succeed.
    // Also combines structs by the union of their fields. As such, currently, it is not guaranteed
    // for casting to work from input types to resulting types. Ideally this changes
    static LogicalType combineTypes(const LogicalType& left, const LogicalType& right);
    static LogicalType combineTypes(const std::vector<LogicalType>& types);

    // makes a copy of the type with any occurences of ANY replaced with replacement
    static LogicalType purgeAny(const LogicalType& type, const LogicalType& replacement);

private:
    static bool tryGetMaxLogicalTypeID(const LogicalTypeID& left, const LogicalTypeID& right,
        LogicalTypeID& result);
};

} // namespace common
} // namespace gorgonzola
