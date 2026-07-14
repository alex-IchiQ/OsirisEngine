#pragma once

#include <span>
#include <string_view>

#include "osiris/reflect/reflect_api.hpp"

namespace osiris::reflect {

struct TypeInfo;

// One registry per process; the instance lives in the osiris.reflect DLL so every
// module observes the same reflected types (see docs/architecture/08).
class OSIRIS_REFLECT_API Registry
{
public:
    static Registry& Instance();

    void Add(const TypeInfo& type);
    const TypeInfo* Find(std::string_view name) const;
    std::span<const TypeInfo* const> Types() const;

private:
    Registry() = default;
};

// Specialized per reflected type by OSIRIS_REFLECT_TYPE (or the codegen tool).
template <class T>
const TypeInfo& TypeOf();

namespace detail {

// Registers a type during static init so it is present without a prior type_of<T>() call.
struct OSIRIS_REFLECT_API AutoRegister {
    explicit AutoRegister(const TypeInfo& type);
};

} // namespace detail

} // namespace osiris::reflect
