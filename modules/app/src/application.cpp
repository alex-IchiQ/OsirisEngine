#include "osiris/app/application.hpp"

#include "osiris/core/engine_context.hpp"
#include "osiris/core/job_system.hpp"
#include "osiris/core/log.hpp"

namespace osiris::app {

Application::~Application() = default;

void Application::Run()
{
    osiris::core::Info("application starting");

    osiris::core::JobSystem jobs;
    osiris::core::EngineContext ctx{jobs};

    OnStart(ctx);

    m_running = true;
    while (m_running) {
        OnUpdate(ctx);
    }

    OnStop(ctx);
    osiris::core::Info("application stopped");
}

} // namespace osiris::app
