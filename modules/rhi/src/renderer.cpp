#include "osiris/rhi/renderer.hpp"

// Phase 1 — YOUR TASK. Move the frame recording out of Swapchain into here:
//   Impl: command_allocator, command_list, fence, fence_value, fence_event, queue.
//   Initialize(device): cache the queue, create the command objects (Close the list).
//   RenderFrame(swapchain, pipeline, r, g, b): the body of the old Swapchain::Render,
//       using the new Swapchain accessors instead of Swapchain's private members.

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
