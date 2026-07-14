#include "osiris/reflect/registry.hpp"

#include "osiris/reflect/type_info.hpp"

#include <vector>

namespace osiris::reflect {

namespace {
std::vector<const TypeInfo*>& Storage()
{
    static std::vector<const TypeInfo*> types;
    return types;
}
} // namespace

Registry& Registry::Instance()
{
    static Registry registry;
    return registry;
}

void Registry::Add(const TypeInfo& type)
{
    if (Find(type.name) == nullptr) {
        Storage().push_back(&type);
    }
}

const TypeInfo* Registry::Find(std::string_view name) const
{
    for (const TypeInfo* type : Storage()) {
        if (type->name == name) {
            return type;
        }
    }
    return nullptr;
}

std::span<const TypeInfo* const> Registry::Types() const
{
    return Storage();
}

detail::AutoRegister::AutoRegister(const TypeInfo& type)
{
    Registry::Instance().Add(type);
}

} // namespace osiris::reflect
