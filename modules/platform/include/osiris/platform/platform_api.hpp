#pragma once

// Symbol visibility for the osiris.platform shared library. See core_api.hpp.

#if defined(_WIN32)
    #if defined(OSIRIS_PLATFORM_EXPORTS)
        #define OSIRIS_PLATFORM_API __declspec(dllexport)
    #else
        #define OSIRIS_PLATFORM_API __declspec(dllimport)
    #endif
#else
    #define OSIRIS_PLATFORM_API __attribute__((visibility("default")))
#endif
