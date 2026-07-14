#pragma once

#include <cstddef>
#include <memory>
#include <span>

#include "osiris/rhi/rhi_api.hpp"

namespace osiris::rhi {

class Device;

// A root signature + graphics PSO for one shader pair. Concrete D3D12 for now.
class OSIRIS_RHI_API Pipeline
{
public:
    Pipeline();
    ~Pipeline();

    Pipeline(const Pipeline&) = delete;
    Pipeline& operator=(const Pipeline&) = delete;

    bool Initialize(Device& device, std::span<const std::byte> vertex_dxil, std::span<const std::byte> pixel_dxil);

    void* NativeRootSignature() const; // ID3D12RootSignature*
    void* NativePipelineState() const; // ID3D12PipelineState*

private:
    struct Impl;
    std::unique_ptr<Impl> m_impl;
};

} // namespace osiris::rhi
