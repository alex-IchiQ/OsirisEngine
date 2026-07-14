#include "osiris/rhi/swapchain.hpp"

#include "osiris/core/log.hpp"
#include "osiris/rhi/device.hpp"
#include "osiris/rhi/pipeline.hpp"

#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <d3d12.h>
#include <dxgi1_6.h>
#include <ostream>
#include <wrl/client.h>

using Microsoft::WRL::ComPtr;

namespace osiris::rhi {

struct Swapchain::Impl {
    static constexpr UINT k_buffer_count = 2;

    ComPtr<IDXGISwapChain3> swap_chain;
    ComPtr<ID3D12DescriptorHeap> rtv_heap;
    ComPtr<ID3D12Resource> back_buffers[k_buffer_count];
    UINT rtv_descriptor_size = 0;
    UINT width = 0;
    UINT height = 0;

    // Frame recording + CPU/GPU sync (M1.3). device/queue are cached from Initialize.
    ComPtr<ID3D12Device> device;
    ComPtr<ID3D12CommandQueue> queue;
    ComPtr<ID3D12CommandAllocator> command_allocator;
    ComPtr<ID3D12GraphicsCommandList> command_list;
    ComPtr<ID3D12Fence> fence;
    UINT64 fence_value = 0;
    HANDLE fence_event = nullptr;
};

Swapchain::Swapchain()
    : m_impl(std::make_unique<Impl>())
{
}

Swapchain::~Swapchain() = default;

bool Swapchain::Initialize(Device& device, void* window_handle, std::uint32_t width, std::uint32_t height)
{
    auto* d3d_device = static_cast<ID3D12Device*>(device.NativeDevice());
    auto* queue = static_cast<ID3D12CommandQueue*>(device.NativeCommandQueue());
    auto* factory = static_cast<IDXGIFactory4*>(device.NativeFactory());
    auto* hwnd = static_cast<HWND>(window_handle);

    DXGI_SWAP_CHAIN_DESC1 sc_descriptor = {};
    sc_descriptor.Width = width;
    sc_descriptor.Height = height;
    sc_descriptor.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    sc_descriptor.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    sc_descriptor.BufferCount = Impl::k_buffer_count;
    sc_descriptor.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
    sc_descriptor.SampleDesc.Count = 1;

    ComPtr<IDXGISwapChain1> swap_chain_1;
    if (!SUCCEEDED(factory->CreateSwapChainForHwnd(queue, hwnd, &sc_descriptor, nullptr, nullptr, &swap_chain_1)))
    {
        osiris::core::Error("Failed to create swap chain.");
        return false;
    }

    if (!SUCCEEDED(swap_chain_1.As(&m_impl->swap_chain)))
    {
        osiris::core::Error("Failed to cast swap chain - 1 to 3.");
        return false;
    }

    D3D12_DESCRIPTOR_HEAP_DESC rtv_heap_description = {};
    rtv_heap_description.NumDescriptors = Impl::k_buffer_count;
    rtv_heap_description.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;

    if (!SUCCEEDED(d3d_device->CreateDescriptorHeap(&rtv_heap_description, IID_PPV_ARGS(&m_impl->rtv_heap))))
    {
        osiris::core::Error("Failed to create RTV Heap");
        return false;
    }

    m_impl->rtv_descriptor_size = d3d_device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);

    D3D12_CPU_DESCRIPTOR_HANDLE rtv_handle = m_impl->rtv_heap->GetCPUDescriptorHandleForHeapStart();
    for (UINT i = 0; i < Impl::k_buffer_count; ++i)
    {
        if (!SUCCEEDED(m_impl->swap_chain->GetBuffer(i, IID_PPV_ARGS(&m_impl->back_buffers[i]))))
        {
            osiris::core::Error("Failed to get back buffer.");
            return false;
        }

        d3d_device->CreateRenderTargetView(m_impl->back_buffers[i].Get(), nullptr, rtv_handle);
        rtv_handle.ptr += m_impl->rtv_descriptor_size;
    }

    m_impl->device = d3d_device;
    m_impl->queue = queue;

    if (!SUCCEEDED(d3d_device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&m_impl->command_allocator))))
    {
        osiris::core::Error("Failed to create command allocator.");
        return false;
    }

    if (!SUCCEEDED(d3d_device->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, m_impl->command_allocator.Get(), nullptr, IID_PPV_ARGS(&m_impl->command_list))))
    {
        osiris::core::Error("Failed to create command list.");
        return false;
    }

    if (!SUCCEEDED(m_impl->command_list->Close()))
    {
        osiris::core::Error("Failed to close command list.");
        return false;
    }

    if (!SUCCEEDED(d3d_device->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&m_impl->fence))))
    {
        osiris::core::Error("Failed to create fence.");
        return false;
    }

    m_impl->fence_event = CreateEventA(nullptr, FALSE, FALSE, nullptr);

    m_impl->width = width;
    m_impl->height = height;

    return true;
}

void Swapchain::Render(Pipeline& pipeline, float r, float g, float b)
{
    if (!SUCCEEDED(m_impl->command_allocator->Reset()))
    {
        osiris::core::Error("Failed to reset Command Allocator");
        return;
    }

    if (!SUCCEEDED(m_impl->command_list->Reset(m_impl->command_allocator.Get(), nullptr)))
    {
        osiris::core::Error("Failed to reset Command List.");
        return;
    }

    const UINT index = m_impl->swap_chain->GetCurrentBackBufferIndex();

    D3D12_RESOURCE_BARRIER barrier = {};
    barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
    barrier.Transition.pResource = m_impl->back_buffers[index].Get();
    barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_PRESENT;
    barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_RENDER_TARGET;
    barrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
    m_impl->command_list->ResourceBarrier(1, &barrier);

    D3D12_CPU_DESCRIPTOR_HANDLE rtv_handle = m_impl->rtv_heap->GetCPUDescriptorHandleForHeapStart();
    rtv_handle.ptr += static_cast<SIZE_T>(index) * m_impl->rtv_descriptor_size;

    const float color[4] = {r, g, b, 1.0f};
    m_impl->command_list->ClearRenderTargetView(rtv_handle, color, 0, nullptr);

    m_impl->command_list->OMSetRenderTargets(1, &rtv_handle, FALSE, nullptr);

    D3D12_VIEWPORT viewport = {};
    viewport.Width = static_cast<float>(m_impl->width);
    viewport.Height = static_cast<float>(m_impl->height);
    viewport.MaxDepth = 1.0f;
    m_impl->command_list->RSSetViewports(1, &viewport);

    D3D12_RECT scissor = {};
    scissor.right = static_cast<LONG>(m_impl->width);
    scissor.bottom = static_cast<LONG>(m_impl->height);
    m_impl->command_list->RSSetScissorRects(1, &scissor);

    m_impl->command_list->SetGraphicsRootSignature(static_cast<ID3D12RootSignature*>(pipeline.NativeRootSignature()));
    m_impl->command_list->SetPipelineState(static_cast<ID3D12PipelineState*>(pipeline.NativePipelineState()));
    m_impl->command_list->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    m_impl->command_list->DrawInstanced(3, 1, 0, 0);

    barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_RENDER_TARGET;
    barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_PRESENT;
    m_impl->command_list->ResourceBarrier(1, &barrier);

    if (!SUCCEEDED(m_impl->command_list->Close()))
    {
        osiris::core::Error("Failed to close command list.");
        return;
    }

    ID3D12CommandList* command_list_array[] = { m_impl->command_list.Get() };
    m_impl->queue->ExecuteCommandLists(1, command_list_array);

    if (!SUCCEEDED(m_impl->swap_chain->Present(1, 0)))
    {
        osiris::core::Error("Failed to present swap chain.");
        return;
    }

    m_impl->fence_value += 1;
    if (!SUCCEEDED(m_impl->queue->Signal(m_impl->fence.Get(), m_impl->fence_value)))
    {
        osiris::core::Error("Failed to signal fence.");
        return;
    }

    if (m_impl->fence->GetCompletedValue() < m_impl->fence_value)
    {
        if (!SUCCEEDED(m_impl->fence->SetEventOnCompletion(m_impl->fence_value, m_impl->fence_event)))
        {
            osiris::core::Error("Failed to set fence event.");
        }

        WaitForSingleObject(m_impl->fence_event, INFINITE);
    }
}

} // namespace osiris::rhi
