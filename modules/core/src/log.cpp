#include "osiris/core/log.hpp"

#include <iostream>

#if defined(_WIN32)
    #define WIN32_LEAN_AND_MEAN
    #define NOMINMAX
    #include <windows.h>
#endif

namespace osiris::core {

namespace {
    // Windows consoles don't interpret ANSI escapes until VT processing is enabled;
    // do it once on the stdout/stderr handles. No-op elsewhere.
    void EnableAnsiColorsOnce()
    {
#if defined(_WIN32)
        static const bool once = [] {
            for (const DWORD handle_id : {STD_OUTPUT_HANDLE, STD_ERROR_HANDLE}) {
                const HANDLE handle = GetStdHandle(handle_id);
                DWORD mode = 0;
                if (handle != INVALID_HANDLE_VALUE && GetConsoleMode(handle, &mode)) {
                    SetConsoleMode(handle, mode | ENABLE_VIRTUAL_TERMINAL_PROCESSING);
                }
            }
            return true;
        }();
        (void)once;
#endif
    }

    std::string_view LevelTag(LogLevel level)
    {
        switch (level) {
            case LogLevel::Trace: return "TRACE";
            case LogLevel::Info:  return "INFO";
            case LogLevel::Warn:  return "WARN";
            case LogLevel::Error: return "ERROR";
        }
        return "?????";
    }

    // Strip the directory so only the file name is logged.
    std::string_view FileName(std::string_view path)
    {
        const std::size_t slash = path.find_last_of("/\\");
        return slash == std::string_view::npos ? path : path.substr(slash + 1);
    }

    // ANSI colors (understood by the CLion console and Windows Terminal).
    std::string_view LevelColor(LogLevel level)
    {
        switch (level) {
            case LogLevel::Trace: return "\033[90m"; // gray
            case LogLevel::Info:  return "";         // default
            case LogLevel::Warn:  return "\033[33m"; // yellow
            case LogLevel::Error: return "\033[31m"; // red
        }
        return "";
    }

    constexpr std::string_view k_reset = "\033[0m";
} // namespace

void Log(LogLevel level, std::string_view message, const std::source_location& location)
{
    EnableAnsiColorsOnce();

    // Warn/Error carry their origin (file:line + function); Info/Trace stay terse.
    const bool detailed = (level == LogLevel::Warn || level == LogLevel::Error);
    std::ostream& out = detailed ? std::cerr : std::cout;

    out << LevelColor(level) << "[osiris][" << LevelTag(level) << "] ";
    if (detailed)
    {
        out << FileName(location.file_name()) << ':' << location.line() << ' ' << location.function_name() << " | ";
    }
    out << message << k_reset << '\n';
}

} // namespace osiris::core
