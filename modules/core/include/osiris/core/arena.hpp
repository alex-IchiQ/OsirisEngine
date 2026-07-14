#pragma once

#include <cstddef>
#include <cstdint>
#include <memory>

namespace osiris::core {

// Bump allocator: hands out memory from one fixed buffer by advancing an offset.
// No per-object free; reset() reclaims everything at once. Suited for per-frame or
// per-task scratch memory.
class Arena
{
public:
    explicit Arena(std::size_t capacity)
        : m_buffer(std::make_unique<std::byte[]>(capacity)), m_capacity(capacity)
    {
    }

    // alignment must be a power of two. Returns nullptr if the request does not fit.
    void* Allocate(std::size_t size, std::size_t alignment = alignof(std::max_align_t))
    {
        const auto base = reinterpret_cast<std::uintptr_t>(m_buffer.get());
        const std::uintptr_t current = base + m_offset;
        const std::uintptr_t aligned = (current + (alignment - 1)) & ~(static_cast<std::uintptr_t>(alignment) - 1);
        const std::size_t new_offset = static_cast<std::size_t>(aligned - base) + size;

        if (new_offset > m_capacity) {
            return nullptr;
        }
        m_offset = new_offset;
        return reinterpret_cast<void*>(aligned);
    }

    void Reset() { m_offset = 0; }

    std::size_t Used() const { return m_offset; }
    std::size_t Capacity() const { return m_capacity; }

private:
    std::unique_ptr<std::byte[]> m_buffer;
    std::size_t m_capacity;
    std::size_t m_offset = 0;
};

} // namespace osiris::core
