#pragma once

#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

#include "common/types/struct_field.h"
#include "common/types/types.h"

namespace gorgonzola {
namespace common {

class GORGONZOLA_API ExtraTypeInfo {
public:
    virtual ~ExtraTypeInfo() = default;

    void serialize(Serializer& serializer) const { serializeInternal(serializer); }

    virtual bool containsAny() const = 0;

    virtual bool operator==(const ExtraTypeInfo& other) const = 0;

    virtual std::unique_ptr<ExtraTypeInfo> copy() const = 0;

    template<class TARGET>
    const TARGET* constPtrCast() const {
        return common::ku_dynamic_cast<const TARGET*>(this);
    }

protected:
    virtual void serializeInternal(Serializer& serializer) const = 0;
};

class GORGONZOLA_API UDTTypeInfo : public ExtraTypeInfo {
public:
    explicit UDTTypeInfo(std::string typeName) : typeName{std::move(typeName)} {}

    std::string getTypeName() const { return typeName; }

    bool containsAny() const override { return false; }

    bool operator==(const ExtraTypeInfo& other) const override;

    std::unique_ptr<ExtraTypeInfo> copy() const override;

    static std::unique_ptr<ExtraTypeInfo> deserialize(Deserializer& deserializer);

private:
    void serializeInternal(Serializer& serializer) const override;

private:
    std::string typeName;
};

class DecimalTypeInfo final : public ExtraTypeInfo {
public:
    explicit DecimalTypeInfo(uint32_t precision = 18, uint32_t scale = 3)
        : precision(precision), scale(scale) {}

    uint32_t getPrecision() const { return precision; }
    uint32_t getScale() const { return scale; }

    bool containsAny() const override { return false; }

    bool operator==(const ExtraTypeInfo& other) const override;

    std::unique_ptr<ExtraTypeInfo> copy() const override;

    static std::unique_ptr<ExtraTypeInfo> deserialize(Deserializer& deserializer);

protected:
    void serializeInternal(Serializer& serializer) const override;

    uint32_t precision, scale;
};

class GORGONZOLA_API ListTypeInfo : public ExtraTypeInfo {
public:
    ListTypeInfo() = default;
    explicit ListTypeInfo(LogicalType childType) : childType{std::move(childType)} {}

    const LogicalType& getChildType() const { return childType; }

    bool containsAny() const override;

    bool operator==(const ExtraTypeInfo& other) const override;

    std::unique_ptr<ExtraTypeInfo> copy() const override;

    static std::unique_ptr<ExtraTypeInfo> deserialize(Deserializer& deserializer);

protected:
    void serializeInternal(Serializer& serializer) const override;

protected:
    LogicalType childType;
};

class GORGONZOLA_API ArrayTypeInfo final : public ListTypeInfo {
public:
    ArrayTypeInfo() : numElements{0} {};
    explicit ArrayTypeInfo(LogicalType childType, uint64_t numElements)
        : ListTypeInfo{std::move(childType)}, numElements{numElements} {}

    uint64_t getNumElements() const { return numElements; }

    bool operator==(const ExtraTypeInfo& other) const override;

    static std::unique_ptr<ExtraTypeInfo> deserialize(Deserializer& deserializer);

    std::unique_ptr<ExtraTypeInfo> copy() const override;

private:
    void serializeInternal(Serializer& serializer) const override;

private:
    uint64_t numElements;
};

class StructTypeInfo final : public ExtraTypeInfo {
public:
    StructTypeInfo() = default;
    explicit StructTypeInfo(std::vector<StructField>&& fields);
    StructTypeInfo(const std::vector<std::string>& fieldNames,
        const std::vector<LogicalType>& fieldTypes);

    bool hasField(const std::string& fieldName) const;
    struct_field_idx_t getStructFieldIdx(std::string fieldName) const;
    const StructField& getStructField(struct_field_idx_t idx) const;
    const StructField& getStructField(const std::string& fieldName) const;
    const std::vector<StructField>& getStructFields() const;

    const LogicalType& getChildType(struct_field_idx_t idx) const;
    std::vector<const LogicalType*> getChildrenTypes() const;
    // can't be a vector of refs since that can't be for-each looped through
    std::vector<std::string> getChildrenNames() const;

    bool containsAny() const override;

    bool operator==(const ExtraTypeInfo& other) const override;

    static std::unique_ptr<ExtraTypeInfo> deserialize(Deserializer& deserializer);
    std::unique_ptr<ExtraTypeInfo> copy() const override;

private:
    void serializeInternal(Serializer& serializer) const override;

private:
    std::vector<StructField> fields;
    std::unordered_map<std::string, struct_field_idx_t> fieldNameToIdxMap;
};

} // namespace common
} // namespace gorgonzola
