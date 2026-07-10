#pragma once

#include <cstdint>
#include <concepts>
#include <string>
#include <string_view>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include "common/api.h"
#include "common/cast.h"
#include "common/copy_constructors.h"
#include "common/types/interval_t.h"

namespace gorgonzola {
namespace common {

class Serializer;
class Deserializer;
struct FileInfo;

using sel_t = uint64_t;
constexpr sel_t INVALID_SEL = UINT64_MAX;
using hash_t = uint64_t;
using page_idx_t = uint32_t;
using frame_idx_t = page_idx_t;
using page_offset_t = uint32_t;
constexpr page_idx_t INVALID_PAGE_IDX = UINT32_MAX;
using file_idx_t = uint32_t;
constexpr file_idx_t INVALID_FILE_IDX = UINT32_MAX;
using page_group_idx_t = uint32_t;
using frame_group_idx_t = page_group_idx_t;
using column_id_t = uint32_t;
using property_id_t = uint32_t;
constexpr column_id_t INVALID_COLUMN_ID = UINT32_MAX;
constexpr column_id_t ROW_IDX_COLUMN_ID = INVALID_COLUMN_ID - 1;
using idx_t = uint32_t;
constexpr idx_t INVALID_IDX = UINT32_MAX;
using block_idx_t = uint64_t;
constexpr block_idx_t INVALID_BLOCK_IDX = UINT64_MAX;
using struct_field_idx_t = uint16_t;
using union_field_idx_t = struct_field_idx_t;
constexpr struct_field_idx_t INVALID_STRUCT_FIELD_IDX = UINT16_MAX;
using row_idx_t = uint64_t;
constexpr row_idx_t INVALID_ROW_IDX = UINT64_MAX;
constexpr uint32_t UNDEFINED_CAST_COST = UINT32_MAX;
using node_group_idx_t = uint64_t;
constexpr node_group_idx_t INVALID_NODE_GROUP_IDX = UINT64_MAX;
using partition_idx_t = uint64_t;
constexpr partition_idx_t INVALID_PARTITION_IDX = UINT64_MAX;
using length_t = uint64_t;
constexpr length_t INVALID_LENGTH = UINT64_MAX;
using list_size_t = uint32_t;
using sequence_id_t = uint64_t;
using oid_t = uint64_t;
constexpr oid_t INVALID_OID = UINT64_MAX;

using transaction_t = uint64_t;
constexpr transaction_t INVALID_TRANSACTION = UINT64_MAX;
using executor_id_t = uint64_t;
using executor_info = std::unordered_map<executor_id_t, uint64_t>;

// table id type alias
using table_id_t = oid_t;
using table_id_vector_t = std::vector<table_id_t>;
using table_id_set_t = std::unordered_set<table_id_t>;
template<typename T>
using table_id_map_t = std::unordered_map<table_id_t, T>;
constexpr table_id_t INVALID_TABLE_ID = INVALID_OID;
// offset type alias
using offset_t = uint64_t;
constexpr offset_t INVALID_OFFSET = UINT64_MAX;
// internal id type alias
struct internalID_t;
using nodeID_t = internalID_t;
using relID_t = internalID_t;

using cardinality_t = uint64_t;
constexpr offset_t INVALID_LIMIT = UINT64_MAX;
constexpr offset_t INVALID_RELBOUND = UINT32_MAX;
using offset_vec_t = std::vector<offset_t>;

// System representation for internalID.
struct GORGONZOLA_API internalID_t {
    offset_t offset;
    table_id_t tableID;

    internalID_t();
    internalID_t(offset_t offset, table_id_t tableID);

    // comparison operators
    bool operator==(const internalID_t& rhs) const;
    bool operator!=(const internalID_t& rhs) const;
    bool operator>(const internalID_t& rhs) const;
    bool operator>=(const internalID_t& rhs) const;
    bool operator<(const internalID_t& rhs) const;
    bool operator<=(const internalID_t& rhs) const;
};

// System representation for a variable-sized overflow value.
struct overflow_value_t {
    // the size of the overflow buffer can be calculated as:
    // numElements * sizeof(Element) + nullMap(4 bytes alignment)
    uint64_t numElements = 0;
    uint8_t* value = nullptr;
};

struct list_entry_t {
    offset_t offset;
    list_size_t size;

    constexpr list_entry_t() : offset{INVALID_OFFSET}, size{UINT32_MAX} {}
    constexpr list_entry_t(offset_t offset, list_size_t size) : offset{offset}, size{size} {}
};

struct struct_entry_t {
    int64_t pos;
};

struct map_entry_t {
    list_entry_t entry;
};

struct union_entry_t {
    struct_entry_t entry;
};

struct int128_t;
struct uint128_t;
struct ku_string_t;

template<typename T>
concept SignedIntegerTypes =
    std::is_same_v<T, int8_t> || std::is_same_v<T, int16_t> || std::is_same_v<T, int32_t> ||
    std::is_same_v<T, int64_t> || std::is_same_v<T, int128_t>;

template<typename T>
concept UnsignedIntegerTypes =
    std::is_same_v<T, uint8_t> || std::is_same_v<T, uint16_t> || std::is_same_v<T, uint32_t> ||
    std::is_same_v<T, uint64_t> || std::is_same_v<T, uint128_t>;

template<typename T>
concept IntegerTypes = SignedIntegerTypes<T> || UnsignedIntegerTypes<T>;

template<typename T>
concept FloatingPointTypes = std::is_same_v<T, float> || std::is_same_v<T, double>;

template<typename T>
concept NumericTypes = IntegerTypes<T> || std::floating_point<T>;

template<typename T>
concept ComparableTypes = NumericTypes<T> || std::is_same_v<T, ku_string_t> ||
                          std::is_same_v<T, interval_t> || std::is_same_v<T, bool>;

template<typename T>
concept HashablePrimitive =
    ((std::integral<T> && !std::is_same_v<T, bool>) || std::floating_point<T> ||
        std::is_same_v<T, int128_t> || std::is_same_v<T, uint128_t>);

template<typename T>
concept IndexHashable = ((std::integral<T> && !std::is_same_v<T, bool>) || std::floating_point<T> ||
                         std::is_same_v<T, int128_t> || std::is_same_v<T, uint128_t> ||
                         std::is_same_v<T, ku_string_t> || std::is_same_v<T, std::string_view> ||
                         std::same_as<T, std::string>);

template<typename T>
concept HashableNonNestedTypes =
    (std::integral<T> || std::floating_point<T> || std::is_same_v<T, int128_t> ||
        std::is_same_v<T, uint128_t> || std::is_same_v<T, internalID_t> ||
        std::is_same_v<T, interval_t> || std::is_same_v<T, ku_string_t>);

template<typename T>
concept HashableNestedTypes =
    (std::is_same_v<T, list_entry_t> || std::is_same_v<T, struct_entry_t>);

template<typename T>
concept HashableTypes = (HashableNestedTypes<T> || HashableNonNestedTypes<T>);

enum class LogicalTypeID : uint8_t {
    ANY = 0,
    NODE = 10,
    REL = 11,
    RECURSIVE_REL = 12,
    SERIAL = 13,

    BOOL = 22,
    INT64 = 23,
    INT32 = 24,
    INT16 = 25,
    INT8 = 26,
    UINT64 = 27,
    UINT32 = 28,
    UINT16 = 29,
    UINT8 = 30,
    INT128 = 31,
    DOUBLE = 32,
    FLOAT = 33,
    DATE = 34,
    TIMESTAMP = 35,
    TIMESTAMP_SEC = 36,
    TIMESTAMP_MS = 37,
    TIMESTAMP_NS = 38,
    TIMESTAMP_TZ = 39,
    INTERVAL = 40,
    DECIMAL = 41,
    INTERNAL_ID = 42,
    UINT128 = 43,

    STRING = 50,
    BLOB = 51,

    LIST = 52,
    ARRAY = 53,
    STRUCT = 54,
    MAP = 55,
    UNION = 56,
    POINTER = 58,

    UUID = 59,
};

enum class PhysicalTypeID : uint8_t {
    ANY = 0,
    BOOL = 1,
    INT64 = 2,
    INT32 = 3,
    INT16 = 4,
    INT8 = 5,
    UINT64 = 6,
    UINT32 = 7,
    UINT16 = 8,
    UINT8 = 9,
    INT128 = 10,
    DOUBLE = 11,
    FLOAT = 12,
    INTERVAL = 13,
    INTERNAL_ID = 14,
    ALP_EXCEPTION_FLOAT = 15,
    ALP_EXCEPTION_DOUBLE = 16,
    UINT128 = 17,

    STRING = 20,
    LIST = 22,
    ARRAY = 23,
    STRUCT = 24,
    POINTER = 25,
};

} // namespace common
} // namespace gorgonzola
