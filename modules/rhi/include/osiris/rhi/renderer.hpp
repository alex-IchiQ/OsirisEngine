#pragma once

#include <memory>

#include "osiris/rhi/rhi_api.hpp"

namespace osiris::rhi
{

class Device;
class Swapchain;
class Pipeline;

// Owns per-frame command recording and CPU/GPU sync: records a clear + draw into the
// swapchain's current back buffer, submits, presents, and waits. Concrete D3D12 for now.
class OSIRIS_RHI_API Renderer
{
public:
    Renderer();
    ~Renderer();

    Renderer(const Renderer&) = delete;
    Renderer& operator=(const Renderer&) = delete;

    bool Initialize(Device& device);
    void RenderFrame(Swapchain& swapchain, Pipeline& pipeline, float r, float g, float b);

private:
    struct Impl;
    std::unique_ptr<Impl> m_impl;
};

} // namespace osiris::rhi
