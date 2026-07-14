#include "osiris/platform/window.hpp"

#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <windows.h>
#include <string>

#include "osiris/core/log.hpp"

namespace osiris::platform {

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
    switch (msg)
    {
        case WM_DESTROY:
            PostQuitMessage(0);
            return 0;
        default:
            return DefWindowProcA(hwnd, msg, wparam, lparam);
    }
}

Window::Window(const WindowDesc& desc)
{
    m_width = desc.width;
    m_height = desc.height;

    WNDCLASS wc = {};
    wc.lpfnWndProc = &WndProc;
    wc.hInstance = GetModuleHandleA(nullptr);
    wc.hCursor = LoadCursorA(nullptr, IDC_ARROW);
    wc.lpszClassName = "OsirisEngine";
    wc.hbrBackground = reinterpret_cast<HBRUSH>(COLOR_WINDOW + 1);

    RegisterClassA(&wc);

    RECT r{0, 0, static_cast<LONG>(desc.width), static_cast<LONG>(desc.height)};
    AdjustWindowRect(&r, WS_OVERLAPPEDWINDOW, FALSE);

    const std::string title{desc.title};

    m_hwnd = CreateWindowExA(0, "OsirisEngine", title.c_str(), WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT,
        r.right - r.left, r.bottom - r.top, nullptr, nullptr, wc.hInstance, nullptr);

    if (m_hwnd != nullptr)
    {
        ShowWindow(static_cast<HWND>(m_hwnd), SW_SHOW);
    }
    else
    {
        osiris::core::Error("Window::Window() m_hwnd is nullptr");
    }
}

Window::~Window()
{
    if (m_hwnd != nullptr)
    {
        DestroyWindow(static_cast<HWND>(m_hwnd));
    }
}

bool Window::PumpEvents()
{
    MSG msg = {};
    while (PeekMessageA(&msg, nullptr, 0, 0, PM_REMOVE))
    {
        if (msg.message == WM_QUIT)
        {
            return false;
        }

        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return true;
}

} // namespace osiris::platform
