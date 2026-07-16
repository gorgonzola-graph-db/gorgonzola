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



enum class FileVersionType : uint8_t { ORIGINAL = 0, WAL_VERSION = 1 };

} // namespace common
} // namespace gorgonzola
