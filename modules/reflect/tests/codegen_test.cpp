#include "demo_component.hpp"

#include "osiris/reflect/registry.hpp"
#include "osiris/reflect/type_info.hpp"

#include <gtest/gtest.h>

#include <cstddef>

// DemoComponent is registered entirely by the generated file — there is no
// OSIRIS_REFLECT_TYPE anywhere for it.

TEST(ReflectCodegen, ExposesFields)
{
    const osiris::reflect::TypeInfo& info = osiris::reflect::TypeOf<DemoComponent>();
    EXPECT_EQ(info.name, "DemoComponent");
    ASSERT_EQ(info.fields.size(), 3u);
    EXPECT_EQ(info.fields[0].name, "position");
    EXPECT_EQ(info.fields[0].type_name, "Vec3");
    EXPECT_EQ(info.fields[1].name, "mass");
    EXPECT_EQ(info.fields[2].name, "id");
    EXPECT_EQ(info.fields[2].offset, offsetof(DemoComponent, id));
}

TEST(ReflectCodegen, RegisteredWithoutManualMacro)
{
    const osiris::reflect::TypeInfo* info = osiris::reflect::Registry::Instance().Find("DemoComponent");
    ASSERT_NE(info, nullptr);
    EXPECT_EQ(info->fields.size(), 3u);
}
