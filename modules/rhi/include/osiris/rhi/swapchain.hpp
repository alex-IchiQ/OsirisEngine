#pragma once

#include <cstdint>
#include <memory>

#include "osiris/rhi/rhi_api.hpp"

namespace osiris::rhi {

class Device;
class Pipeline;

// The chain of back buffers rendered into and presented to a window. Creates a
// render target view (RTV) for each buffer. D3D12 details hidden behind a pimpl.
class OSIRIS_RHI_API Swapchain
{
public:
    Swapchain();
    ~Swapchain();

    Swapchain(const Swapchain&) = delete;
    Swapchain& operator=(const Swapchain&) = delete;

    bool Initialize(Device& device, void* window_handle, std::uint32_t width, std::uint32_t height);

    // Records and submits one frame: clears the current back buffer to {r, g, b} and
    // draws the pipeline's triangle into it.
    void Render(Pipeline& pipeline, float r, float g, float b);

private:
    struct Impl;
    std::unique_ptr<Impl> m_impl;
};

} // namespace osiris::rhi
