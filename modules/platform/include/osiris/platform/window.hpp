#pragma once

#include <cstdint>
#include <string_view>

#include "osiris/platform/platform_api.hpp"

namespace osiris::platform {

struct WindowDesc
{
    std::string_view title = "Osiris";
    std::uint32_t width = 1280;
    std::uint32_t height = 720;
};

// A single OS window. NativeHandle() returns the HWND (as void*) so the RHI can
// bind a swapchain to it later without <windows.h> leaking into this header.
class OSIRIS_PLATFORM_API Window
{
public:
    explicit Window(const WindowDesc& desc);
    ~Window();

    Window(const Window&) = delete;
    Window& operator=(const Window&) = delete;

    // Drains the OS message queue. Returns false once the window has been closed.
    bool PumpEvents();

    void* NativeHandle() const { return m_hwnd; }
    std::uint32_t Width() const { return m_width; }
    std::uint32_t Height() const { return m_height; }

private:
    void* m_hwnd = nullptr; // HWND
    std::uint32_t m_width = 0;
    std::uint32_t m_height = 0;
};

} // namespace osiris::platform
