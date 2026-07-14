#include "osiris/rhi/renderer.hpp"

namespace osiris::rhi
{

struct Renderer::Impl
{
};

Renderer::Renderer()
    : m_impl(std::make_unique<Impl>())
{
}

Renderer::~Renderer() = default;

bool Renderer::Initialize(Device& device)
{
    (void)device;
    return false;
}

void Renderer::RenderFrame(Swapchain& swapchain, Pipeline& pipeline, float r, float g, float b)
{
    (void)swapchain;
    (void)pipeline;
    (void)r;
    (void)g;
    (void)b;
}

} // namespace osiris::rhi
