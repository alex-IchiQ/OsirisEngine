#include "osiris/rhi/pipeline.hpp"

#include "osiris/core/log.hpp"
#include "osiris/rhi/device.hpp"

#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <d3d12.h>
#include <wrl/client.h>

using Microsoft::WRL::ComPtr;

namespace osiris::rhi {

struct Pipeline::Impl {
    ComPtr<ID3D12RootSignature> root_signature;
    ComPtr<ID3D12PipelineState> pipeline_state;
};

Pipeline::Pipeline()
    : m_impl(std::make_unique<Impl>())
{
}

Pipeline::~Pipeline() = default;

bool Pipeline::Initialize(Device& device, std::span<const std::byte> vertex_dxil,
                          std::span<const std::byte> pixel_dxil)
{
    auto* d3d_device = static_cast<ID3D12Device*>(device.NativeDevice());

    D3D12_ROOT_SIGNATURE_DESC root = {};
    root.NumParameters = 0;
    root.NumStaticSamplers = 0;
    root.Flags = D3D12_ROOT_SIGNATURE_FLAG_NONE;

    ComPtr<ID3DBlob> serialized;
    ComPtr<ID3DBlob> error;

    if (FAILED(D3D12SerializeRootSignature(&root, D3D_ROOT_SIGNATURE_VERSION_1, &serialized, &error)))
    {
        if (error)
        {
            osiris::core::Error(static_cast<const char*>(error->GetBufferPointer()));
        }

        return false;
    }

    if (FAILED(d3d_device->CreateRootSignature(0, serialized->GetBufferPointer(), serialized->GetBufferSize(),IID_PPV_ARGS(&m_impl->root_signature))))
    {
        osiris::core::Error("Failed to create root signature.");
        return false;
    }

    D3D12_RASTERIZER_DESC rasterizer = {};
    rasterizer.FillMode = D3D12_FILL_MODE_SOLID;
    rasterizer.CullMode = D3D12_CULL_MODE_NONE;
    rasterizer.DepthClipEnable = TRUE;

    D3D12_BLEND_DESC blend = {};
    blend.RenderTarget[0].RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;

    D3D12_GRAPHICS_PIPELINE_STATE_DESC pso = {};
    pso.pRootSignature = m_impl->root_signature.Get();
    pso.VS = { vertex_dxil.data(), vertex_dxil.size() };
    pso.PS = { pixel_dxil.data(), pixel_dxil.size() };
    pso.RasterizerState = rasterizer;
    pso.BlendState = blend;
    pso.DepthStencilState.DepthEnable = FALSE;
    pso.DepthStencilState.StencilEnable = FALSE;
    pso.SampleMask = UINT_MAX;
    pso.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
    pso.NumRenderTargets = 1;
    pso.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;   // как у swapchain
    pso.SampleDesc.Count = 1;

    if (FAILED(d3d_device->CreateGraphicsPipelineState(&pso, IID_PPV_ARGS(&m_impl->pipeline_state))))
    {
        osiris::core::Error("Failed to create pipeline state.");
        return false;
    }

    osiris::core::Info("Pipeline created.");
    return true;
}

void* Pipeline::NativeRootSignature() const
{
    return m_impl->root_signature.Get();
}

void* Pipeline::NativePipelineState() const
{
    return m_impl->pipeline_state.Get();
}

} // namespace osiris::rhi
