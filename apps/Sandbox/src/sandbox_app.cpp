#include <memory>
#include <string>

#include "osiris/app/application.hpp"

#include "osiris/core/engine_context.hpp"
#include "osiris/core/job_system.hpp"
#include "osiris/core/log.hpp"
#include "osiris/platform/window.hpp"
#include "osiris/rhi/device.hpp"
#include "osiris/rhi/pipeline.hpp"
#include "osiris/rhi/swapchain.hpp"
#include "osiris/rhi/shader_compiler.hpp"

static constexpr const char* k_triangle_shader = R"(
struct VSOutput { float4 position : SV_Position; float3 color : COLOR0; };

[shader("vertex")]
VSOutput vertexMain(uint id : SV_VertexID)
{
    float2 verts[3] = { float2(0, 0.5), float2(0.5, -0.5), float2(-0.5, -0.5) };
    float3 cols[3]  = { float3(1,0,0), float3(0,1,0), float3(0,0,1) };
    VSOutput output;
    output.position = float4(verts[id], 0, 1);
    output.color = cols[id];
    return output;
}

[shader("fragment")]
float4 fragmentMain(VSOutput input) : SV_Target
{
    return float4(input.color, 1);
}
)";

// main() is provided by osiris::app_entry; the client only supplies the Application subclass.
class SandboxApp : public osiris::app::Application
{
protected:
    void OnStart(osiris::core::EngineContext& ctx) override
    {
        osiris::core::Info("Osiris Engine - bootstrap OK");
        ctx.jobs.Submit([] { osiris::core::Trace("job ran on the job system"); });

        osiris::platform::WindowDesc window_desc;

        m_window = std::make_unique<osiris::platform::Window>(window_desc);

        m_device = std::make_unique<osiris::rhi::Device>();
        m_device->Initialize();

        m_swapchain = std::make_unique<osiris::rhi::Swapchain>();
        m_swapchain->Initialize(*m_device, m_window->NativeHandle(), m_window->Width(), m_window->Height());

        osiris::rhi::ShaderCompiler compiler;
        const std::vector<std::byte> vs = compiler.Compile(k_triangle_shader, "vertexMain");
        const std::vector<std::byte> ps = compiler.Compile(k_triangle_shader, "fragmentMain");
        osiris::core::Info(("VS DXIL bytes: " + std::to_string(vs.size())).c_str());
        osiris::core::Info(("PS DXIL bytes: " + std::to_string(ps.size())).c_str());

        m_pipeline = std::make_unique<osiris::rhi::Pipeline>();
        m_pipeline->Initialize(*m_device, vs, ps);
    }

    void OnUpdate(osiris::core::EngineContext&) override
    {
        if (!m_window || !m_window->PumpEvents())
        {
            RequestStop();
        }

        m_swapchain->Render(*m_pipeline, 0.2f, 0.4f, 0.8f);
    }

    void OnStop(osiris::core::EngineContext&) override
    {
        osiris::core::Trace("SandboxApp stopping");
    }

private:
    std::unique_ptr<osiris::platform::Window> m_window = nullptr;
    std::unique_ptr<osiris::rhi::Device> m_device = nullptr;
    std::unique_ptr<osiris::rhi::Swapchain> m_swapchain = nullptr;
    std::unique_ptr<osiris::rhi::Pipeline> m_pipeline = nullptr;
};

osiris::app::Application* osiris::app::CreateApplication()
{
    return new SandboxApp();
}
