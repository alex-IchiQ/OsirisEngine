#pragma once

#include <cstddef>
#include <cstdint>

// Fundamental type aliases used across the engine instead of platform types (UINT,
// LONG, SIZE_T, ...). Defined in the root osiris namespace so engine code can use them
// unqualified (e.g. `u32` inside osiris::rhi) or as `osiris::u32` from the outside.
namespace osiris
{

using u8 = std::uint8_t;
using u16 = std::uint16_t;
using u32 = std::uint32_t;
using u64 = std::uint64_t;

using i8 = std::int8_t;
using i16 = std::int16_t;
using i32 = std::int32_t;
using i64 = std::int64_t;

using f32 = float;
using f64 = double;

using usize = std::size_t;
using iptr = std::intptr_t;
using uptr = std::uintptr_t;
using byte = std::byte;

} // namespace osiris
