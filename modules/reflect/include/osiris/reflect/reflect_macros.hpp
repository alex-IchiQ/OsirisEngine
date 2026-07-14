#pragma once

#include <cstddef>

#include "osiris/reflect/registry.hpp"
#include "osiris/reflect/type_info.hpp"

// Hand-written reflection registration. The codegen tool will emit the same
// specialization automatically; this macro is the manual fallback.

#define OSIRIS_REFLECT_DETAIL_CONCAT(a, b) a##b
#define OSIRIS_REFLECT_DETAIL_UNIQUE(prefix) OSIRIS_REFLECT_DETAIL_CONCAT(prefix, __COUNTER__)

#define OSIRIS_REFLECT_FIELD(Type, FieldType, Member) \
    ::osiris::reflect::FieldInfo{#Member, #FieldType, offsetof(Type, Member), sizeof(FieldType)}

#define OSIRIS_REFLECT_TYPE(Type, ...)                                                                      \
    template <>                                                                                             \
    const ::osiris::reflect::TypeInfo& ::osiris::reflect::TypeOf<Type>()                                    \
    {                                                                                                       \
        static constexpr ::osiris::reflect::FieldInfo k_fields[] = {__VA_ARGS__};                           \
        static const ::osiris::reflect::TypeInfo k_info{#Type, sizeof(Type), alignof(Type), k_fields};      \
        return k_info;                                                                                      \
    }                                                                                                       \
    static const ::osiris::reflect::detail::AutoRegister                                                    \
        OSIRIS_REFLECT_DETAIL_UNIQUE(k_osiris_autoreg_){::osiris::reflect::TypeOf<Type>()};
