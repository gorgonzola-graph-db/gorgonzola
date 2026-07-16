#pragma once

#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

#include "common/api.h"
#include "common/cast.h"
#include "common/copy_constructors.h"
#include "common/types/interval_t.h"
#include "common/types/type_aliases.h"
namespace gorgonzola {
namespace main {
class ClientContext;
}
namespace processor {
class ParquetReader;
}
namespace catalog {
class NodeTableCatalogEntry;
}
namespace common {

class Serializer;
class Deserializer;
struct FileInfo;


class ExtraTypeInfo;
class StructField;
class StructTypeInfo;

enum class TypeCategory : uint8_t { INTERNAL = 0, UDT = 1 };

class LogicalType {
    friend struct LogicalTypeUtils;
    friend struct DecimalType;
    friend struct StructType;
    friend struct ListType;
    friend struct ArrayType;

    GORGONZOLA_API LogicalType(const LogicalType& other);

public:
    GORGONZOLA_API LogicalType();
    explicit GORGONZOLA_API LogicalType(LogicalTypeID typeID, TypeCategory info = TypeCategory::INTERNAL);
    DELETE_COPY_ASSN(LogicalType);
    EXPLICIT_COPY_METHOD(LogicalType);
    GORGONZOLA_API ~LogicalType();
    GORGONZOLA_API LogicalType(LogicalType&& other) noexcept;
    GORGONZOLA_API LogicalType& operator=(LogicalType&& other) noexcept;

    GORGONZOLA_API bool operator==(const LogicalType& other) const;
    GORGONZOLA_API bool operator!=(const LogicalType& other) const;

    GORGONZOLA_API std::string toString() const;
    static bool isBuiltInType(const std::string& str);
    static LogicalType convertFromString(const std::string& str, main::ClientContext* context);

    GORGONZOLA_API LogicalTypeID getLogicalTypeID() const { return typeID; }
    bool containsAny() const;
    bool isInternalType() const { return category == TypeCategory::INTERNAL; }

    GORGONZOLA_API PhysicalTypeID getPhysicalType() const { return physicalType; }
    GORGONZOLA_API static PhysicalTypeID getPhysicalType(LogicalTypeID logicalType,
        const std::unique_ptr<ExtraTypeInfo>& extraTypeInfo = nullptr);

    void setExtraTypeInfo(std::unique_ptr<ExtraTypeInfo> typeInfo);

    const ExtraTypeInfo* getExtraTypeInfo() const { return extraTypeInfo.get(); }

    void serialize(Serializer& serializer) const;

    static LogicalType deserialize(Deserializer& deserializer);

    GORGONZOLA_API static std::vector<LogicalType> copy(const std::vector<LogicalType>& types);
    GORGONZOLA_API static std::vector<LogicalType> copy(const std::vector<LogicalType*>& types);

    static LogicalType ANY() { return LogicalType(LogicalTypeID::ANY); }

    // NOTE: avoid using this if possible, this is a temporary hack for passing internal types
    // TODO(Royi) remove this when float compression no longer relies on this or ColumnChunkData
    // takes physical types instead of logical types
    static LogicalType ANY(PhysicalTypeID physicalType) {
        auto ret = LogicalType(LogicalTypeID::ANY);
        ret.physicalType = physicalType;
        return ret;
    }

    static LogicalType BOOL() { return LogicalType(LogicalTypeID::BOOL); }
    static LogicalType HASH() { return LogicalType(LogicalTypeID::UINT64); }
    static LogicalType INT64() { return LogicalType(LogicalTypeID::INT64); }
    static LogicalType INT32() { return LogicalType(LogicalTypeID::INT32); }
    static LogicalType INT16() { return LogicalType(LogicalTypeID::INT16); }
    static LogicalType INT8() { return LogicalType(LogicalTypeID::INT8); }
    static LogicalType UINT64() { return LogicalType(LogicalTypeID::UINT64); }
    static LogicalType UINT32() { return LogicalType(LogicalTypeID::UINT32); }
    static LogicalType UINT16() { return LogicalType(LogicalTypeID::UINT16); }
    static LogicalType UINT8() { return LogicalType(LogicalTypeID::UINT8); }
    static LogicalType INT128() { return LogicalType(LogicalTypeID::INT128); }
    static LogicalType DOUBLE() { return LogicalType(LogicalTypeID::DOUBLE); }
    static LogicalType FLOAT() { return LogicalType(LogicalTypeID::FLOAT); }
    static LogicalType DATE() { return LogicalType(LogicalTypeID::DATE); }
    static LogicalType TIMESTAMP_NS() { return LogicalType(LogicalTypeID::TIMESTAMP_NS); }
    static LogicalType TIMESTAMP_MS() { return LogicalType(LogicalTypeID::TIMESTAMP_MS); }
    static LogicalType TIMESTAMP_SEC() { return LogicalType(LogicalTypeID::TIMESTAMP_SEC); }
    static LogicalType TIMESTAMP_TZ() { return LogicalType(LogicalTypeID::TIMESTAMP_TZ); }
    static LogicalType TIMESTAMP() { return LogicalType(LogicalTypeID::TIMESTAMP); }
    static LogicalType INTERVAL() { return LogicalType(LogicalTypeID::INTERVAL); }
    static GORGONZOLA_API LogicalType DECIMAL(uint32_t precision, uint32_t scale);
    static LogicalType INTERNAL_ID() { return LogicalType(LogicalTypeID::INTERNAL_ID); }
    static LogicalType UINT128() { return LogicalType(LogicalTypeID::UINT128); };
    static LogicalType SERIAL() { return LogicalType(LogicalTypeID::SERIAL); }
    static LogicalType STRING() { return LogicalType(LogicalTypeID::STRING); }
    static LogicalType BLOB() { return LogicalType(LogicalTypeID::BLOB); }
    static LogicalType UUID() { return LogicalType(LogicalTypeID::UUID); }
    static LogicalType POINTER() { return LogicalType(LogicalTypeID::POINTER); }
    static GORGONZOLA_API LogicalType STRUCT(std::vector<StructField>&& fields);

    static GORGONZOLA_API LogicalType RECURSIVE_REL(std::vector<StructField>&& fields);

    static GORGONZOLA_API LogicalType NODE(std::vector<StructField>&& fields);

    static GORGONZOLA_API LogicalType REL(std::vector<StructField>&& fields);

    static GORGONZOLA_API LogicalType UNION(std::vector<StructField>&& fields);

    static GORGONZOLA_API LogicalType LIST(LogicalType childType);
    template<class T>
    static inline LogicalType LIST(T&& childType) {
        return LogicalType::LIST(LogicalType(std::forward<T>(childType)));
    }

    static GORGONZOLA_API LogicalType MAP(LogicalType keyType, LogicalType valueType);
    template<class T>
    static LogicalType MAP(T&& keyType, T&& valueType) {
        return LogicalType::MAP(LogicalType(std::forward<T>(keyType)),
            LogicalType(std::forward<T>(valueType)));
    }

    static GORGONZOLA_API LogicalType ARRAY(LogicalType childType, uint64_t numElements);
    template<class T>
    static LogicalType ARRAY(T&& childType, uint64_t numElements) {
        return LogicalType::ARRAY(LogicalType(std::forward<T>(childType)), numElements);
    }

private:
    friend struct CAPIHelper;
    friend struct JavaAPIHelper;
    friend class gorgonzola::processor::ParquetReader;
    explicit LogicalType(LogicalTypeID typeID, std::unique_ptr<ExtraTypeInfo> extraTypeInfo);

private:
    LogicalTypeID typeID;
    PhysicalTypeID physicalType;
    std::unique_ptr<ExtraTypeInfo> extraTypeInfo;
    TypeCategory category = TypeCategory::INTERNAL;
};

using logical_type_vec_t = std::vector<LogicalType>;

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

enum class FileVersionType : uint8_t { ORIGINAL = 0, WAL_VERSION = 1 };

} // namespace common
} // namespace gorgonzola
