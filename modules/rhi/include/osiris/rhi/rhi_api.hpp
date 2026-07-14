#pragma once

// Symbol visibility for the osiris.rhi shared library. See core_api.hpp.

#if defined(_WIN32)
    #if defined(OSIRIS_RHI_EXPORTS)
        #define OSIRIS_RHI_API __declspec(dllexport)
    #else
        #define OSIRIS_RHI_API __declspec(dllimport)
    #endif
#else
    #define OSIRIS_RHI_API __attribute__((visibility("default")))
#endif
