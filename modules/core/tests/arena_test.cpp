#include "osiris/core/arena.hpp"

#include <gtest/gtest.h>

#include <cstdint>

using osiris::core::Arena;

TEST(Arena, AllocatesWithinCapacity)
{
    Arena a(1024);
    void* p = a.Allocate(128);
    ASSERT_NE(p, nullptr);
    EXPECT_GE(a.Used(), 128u);
    EXPECT_EQ(a.Capacity(), 1024u);
}

TEST(Arena, RespectsAlignment)
{
    Arena a(1024);
    a.Allocate(1); // misalign the next address so padding is required
    void* p = a.Allocate(16, 64);
    ASSERT_NE(p, nullptr);
    EXPECT_EQ(reinterpret_cast<std::uintptr_t>(p) % 64, 0u);
}

TEST(Arena, ReturnsNullOnOverflow)
{
    Arena a(64);
    EXPECT_EQ(a.Allocate(128), nullptr);
    EXPECT_EQ(a.Used(), 0u); // a failed allocation must not advance the offset
}

TEST(Arena, ResetReclaimsSpace)
{
    Arena a(256);
    a.Allocate(200);
    EXPECT_GT(a.Used(), 0u);
    a.Reset();
    EXPECT_EQ(a.Used(), 0u);
    EXPECT_NE(a.Allocate(200), nullptr);
}
