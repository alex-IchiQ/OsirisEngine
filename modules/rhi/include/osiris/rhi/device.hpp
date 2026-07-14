#pragma once

#include <memory>

#include "osiris/rhi/rhi_api.hpp"

namespace osiris::rhi {

// Owns the D3D12 device and its graphics command queue. Concrete D3D12 for now;
// an IDevice interface will be extracted once the shape is clear. D3D12 details are
// hidden behind a pimpl so <d3d12.h> does not leak into this header.
class OSIRIS_RHI_API Device
{
public:
    Device();
    ~Device();

    Device(const Device&) = delete;
    Device& operator=(const Device&) = delete;

    bool Initialize();

    // Native D3D12 handles for other RHI objects (e.g. the swapchain), returned as
    // void* so <d3d12.h> stays out of this header; cast back in the implementation.
    void* NativeDevice() const;
    void* NativeCommandQueue() const;
    void* NativeFactory() const;

private:
    struct Impl;
    std::unique_ptr<Impl> m_impl;
};

} // namespace osiris::rhi
