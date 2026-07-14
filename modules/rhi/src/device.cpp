#include "osiris/rhi/device.hpp"

#include "osiris/core/log.hpp"

#include <d3d12.h>
#include <dxgi1_6.h>
#include <wrl/client.h>

using Microsoft::WRL::ComPtr;

namespace osiris::rhi {

struct Device::Impl {
    ComPtr<IDXGIFactory4> factory;
    ComPtr<ID3D12Device> device;
    ComPtr<ID3D12CommandQueue> queue;
};

Device::Device()
    : m_impl(std::make_unique<Impl>())
{
}

Device::~Device() = default;

bool Device::Initialize()
{
    UINT flags = 0;

#if defined(_DEBUG)
    flags |= DXGI_CREATE_FACTORY_DEBUG;
    ComPtr<ID3D12Debug> debug_controller;

    if (!SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(&debug_controller))))
    {
        osiris::core::Error("Failed to get D3D12 debug interface.");
        return false;
    }

    debug_controller->EnableDebugLayer();
#endif

    //TODO: move it to string utils or some lib
    auto wchar_to_string = [](const wchar_t* wstr) -> std::string {
        const size_t size = std::wcstombs(nullptr, wstr, 0);
        if (size == static_cast<size_t>(-1))
        {
            return std::string();
        }

        std::string result(size, '\0');
        wcstombs(&result[0], wstr, size);

        return result;
    };

    if (!SUCCEEDED(CreateDXGIFactory2(flags, IID_PPV_ARGS(&m_impl->factory))))
    {
        osiris::core::Error("Failed to create DXGIFactory2");
        return false;
    }

    ComPtr<IDXGIAdapter1> adapter;
    for (UINT i = 0; m_impl->factory->EnumAdapters1(i, adapter.ReleaseAndGetAddressOf()) != DXGI_ERROR_NOT_FOUND; ++i)
    {
        DXGI_ADAPTER_DESC1 description = {};
        adapter->GetDesc1(&description);

        if (description.Flags & DXGI_ADAPTER_FLAG_SOFTWARE)
        {
            continue;
        }

        if (!SUCCEEDED(D3D12CreateDevice(adapter.Get(), D3D_FEATURE_LEVEL_12_0, IID_PPV_ARGS(&m_impl->device))))
        {
            osiris::core::Error("Failed to create D3D12 device.");
            return false;
        }

        osiris::core::Info(("Using GPU: " + wchar_to_string(description.Description)).c_str());
        break;
    }

    D3D12_COMMAND_QUEUE_DESC queue_description = {};
    queue_description.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
    queue_description.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;

    if (!SUCCEEDED(m_impl->device->CreateCommandQueue(&queue_description, IID_PPV_ARGS(&m_impl->queue))))
    {
        osiris::core::Error("Failed to create D3D12 command queue.");
        return false;
    }

    osiris::core::Info("D3D12 Command Queue created.");

    return true;
}

void* Device::NativeDevice() const
{
    return m_impl->device.Get();
}

void* Device::NativeCommandQueue() const
{
    return m_impl->queue.Get();
}

void* Device::NativeFactory() const
{
    return m_impl->factory.Get();
}

} // namespace osiris::rhi
