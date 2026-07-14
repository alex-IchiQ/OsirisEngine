#pragma once

namespace osiris::core {

class JobSystem;

// Service bus passed explicitly through the engine instead of reached via singletons
// (see docs/architecture/01). Plain aggregate: public fields, no m_ prefix.
struct EngineContext {
    JobSystem& jobs;
};

} // namespace osiris::core
