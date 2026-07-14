#include "osiris/core/job_system.hpp"

namespace osiris::core {

void JobSystem::Submit(Job job)
{
    if (job) {
        job();
    }
}

void JobSystem::WaitIdle() {}

} // namespace osiris::core
