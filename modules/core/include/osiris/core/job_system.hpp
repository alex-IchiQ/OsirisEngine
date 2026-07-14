#pragma once

#include <functional>

#include "osiris/core/core_api.hpp"

namespace osiris::core {

// Stub: jobs run synchronously on submit. The threaded implementation will land
// later behind this same interface.
class OSIRIS_CORE_API JobSystem
{
public:
    using Job = std::function<void()>;

    void Submit(Job job);
    void WaitIdle();
};

} // namespace osiris::core
