#pragma once

// Symbol visibility for the osiris.reflect shared library. See core_api.hpp.

#if defined(_WIN32)
    #if defined(OSIRIS_REFLECT_EXPORTS)
        #define OSIRIS_REFLECT_API __declspec(dllexport)
    #else
        #define OSIRIS_REFLECT_API __declspec(dllimport)
    #endif
#else
    #define OSIRIS_REFLECT_API __attribute__((visibility("default")))
#endif
