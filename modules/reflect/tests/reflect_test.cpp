#include "osiris/reflect/reflect.hpp"

#include <gtest/gtest.h>

#include <cstddef>


struct Demo {
    float x;
    int y;
};

OSIRIS_REFLECT_TYPE(Demo,
    OSIRIS_REFLECT_FIELD(Demo, float, x),
    OSIRIS_REFLECT_FIELD(Demo, int, y))

TEST(Reflect, ExposesFields)
{
    const osiris::reflect::TypeInfo& info = osiris::reflect::TypeOf<Demo>();
    EXPECT_EQ(info.name, "Demo");
    ASSERT_EQ(info.fields.size(), 2u);
    EXPECT_EQ(info.fields[0].name, "x");
    EXPECT_EQ(info.fields[0].type_name, "float");
    EXPECT_EQ(info.fields[0].offset, offsetof(Demo, x));
    EXPECT_EQ(info.fields[1].name, "y");
    EXPECT_EQ(info.fields[1].offset, offsetof(Demo, y));
}

TEST(Reflect, RegisteredAtStartup)
{
    const osiris::reflect::TypeInfo* info = osiris::reflect::Registry::Instance().Find("Demo");
    ASSERT_NE(info, nullptr);
    EXPECT_EQ(info->fields.size(), 2u);
}
