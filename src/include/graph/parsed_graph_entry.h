#pragma once

#include <cstdint>
#include <string>
#include <vector>

#include "common/cast.h"

namespace gorgonzola {
namespace graph {

enum class GraphEntryType : uint8_t {
    NATIVE = 0,
    CYPHER = 1,
};

struct GraphEntryTypeUtils {
    static std::string toString(GraphEntryType type);
};

struct GORGONZOLA_API ParsedGraphEntry {
    GraphEntryType type;

    explicit ParsedGraphEntry(GraphEntryType type) : type{type} {}
    virtual ~ParsedGraphEntry() = default;

    template<class TARGET>
    TARGET& cast() {
        return common::ku_dynamic_cast<TARGET&>(*this);
    }
};

struct ParsedNativeGraphTableInfo {
    std::string tableName;
    std::string predicate;

    ParsedNativeGraphTableInfo(std::string tableName, std::string predicate)
        : tableName{std::move(tableName)}, predicate{std::move(predicate)} {}
};

struct GORGONZOLA_API ParsedNativeGraphEntry : ParsedGraphEntry {
    std::vector<ParsedNativeGraphTableInfo> nodeInfos;
    std::vector<ParsedNativeGraphTableInfo> relInfos;

    ParsedNativeGraphEntry(std::vector<ParsedNativeGraphTableInfo> nodeInfos,
        std::vector<ParsedNativeGraphTableInfo> relInfos)
        : ParsedGraphEntry{GraphEntryType::NATIVE}, nodeInfos{std::move(nodeInfos)},
          relInfos{std::move(relInfos)} {}
};

struct GORGONZOLA_API ParsedCypherGraphEntry : ParsedGraphEntry {
    std::string cypherQuery;

    explicit ParsedCypherGraphEntry(std::string cypherQuery)
        : ParsedGraphEntry{GraphEntryType::CYPHER}, cypherQuery{std::move(cypherQuery)} {}
};

} // namespace graph
} // namespace gorgonzola
