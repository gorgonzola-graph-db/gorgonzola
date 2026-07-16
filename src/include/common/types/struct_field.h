#pragma once

#include <string>

#include "common/copy_constructors.h"
#include "common/types/types.h"

namespace gorgonzola {
namespace common {

class StructField {
public:
    StructField() : type{LogicalType()} {}
    StructField(std::string name, LogicalType type)
        : name{std::move(name)}, type{std::move(type)} {};

    DELETE_COPY_DEFAULT_MOVE(StructField);

    std::string getName() const { return name; }

    const LogicalType& getType() const { return type; }

    bool containsAny() const;

    bool operator==(const StructField& other) const;
    bool operator!=(const StructField& other) const { return !(*this == other); }

    void serialize(Serializer& serializer) const;

    static StructField deserialize(Deserializer& deserializer);

    StructField copy() const;

private:
    std::string name;
    LogicalType type;
};

} // namespace common
} // namespace gorgonzola
