#pragma once

#include "osiris/app/app_api.hpp"

namespace osiris::core {
    struct EngineContext;
}

namespace osiris::app {

// The engine owns the game loop (run()); clients subclass and override the on_* hooks.
class OSIRIS_APP_API Application
{
public:
    Application() = default;
    virtual ~Application(); // out-of-line: anchors the vtable in this DLL

    Application(const Application&) = delete;
    Application& operator=(const Application&) = delete;

    void Run();

    void RequestStop() { m_running = false; }

protected:
    virtual void OnStart(osiris::core::EngineContext&) {}
    virtual void OnUpdate(osiris::core::EngineContext&) {}
    virtual void OnStop(osiris::core::EngineContext&) {}

private:
    bool m_running = false;
};

// Implemented by the client (Sandbox/Editor); called from main() in entry_point.hpp.
Application* CreateApplication();

} // namespace osiris::app
