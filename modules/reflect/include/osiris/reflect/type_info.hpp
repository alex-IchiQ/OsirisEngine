#pragma once

#include <cstddef>
#include <span>
#include <string_view>

namespace osiris::reflect {

// Populated by OSIRIS_REFLECT_TYPE today, by the codegen tool later. Consumers
// (inspector, serialization, ECS) only ever see these two structs.
struct FieldInfo
{
    std::string_view name;
    std::string_view type_name;
    std::size_t offset;
    std::size_t size;
};

struct TypeInfo
{
    std::string_view name;
    std::size_t size;
    std::size_t align;
    std::span<const FieldInfo> fields;
};

} // namespace osiris::reflect
