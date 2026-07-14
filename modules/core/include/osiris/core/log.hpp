#pragma once

#include <source_location>
#include <string_view>

#include "osiris/core/core_api.hpp"

namespace osiris::core {

enum class LogLevel {
    Trace,
    Info,
    Warn,
    Error,
};

// The default source_location is evaluated at the *call site*, so the helpers below
// capture the caller's function/file/line and forward it here. Only Log crosses the
// DLL boundary; the helpers are inline and compile into the caller.
OSIRIS_CORE_API void Log(LogLevel level, std::string_view message, const std::source_location& location = std::source_location::current());

inline void Trace(std::string_view m, const std::source_location& loc = std::source_location::current())
{
    Log(LogLevel::Trace, m, loc);
}

inline void Info(std::string_view m, const std::source_location& loc = std::source_location::current())
{
    Log(LogLevel::Info, m, loc);
}

inline void Warn(std::string_view m, const std::source_location& loc = std::source_location::current())
{
    Log(LogLevel::Warn, m, loc);
}

inline void Error(std::string_view m, const std::source_location& loc = std::source_location::current())
{
    Log(LogLevel::Error, m, loc);
}

} // namespace osiris::core
