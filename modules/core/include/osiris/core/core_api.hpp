#pragma once

// Symbol visibility for the osiris.core shared library.
// OSIRIS_CORE_EXPORTS is defined only while building the DLL itself (see CMakeLists).

#if defined(_WIN32)
    #if defined(OSIRIS_CORE_EXPORTS)
        #define OSIRIS_CORE_API __declspec(dllexport)
    #else
        #define OSIRIS_CORE_API __declspec(dllimport)
    #endif
#else
    #define OSIRIS_CORE_API __attribute__((visibility("default")))
#endif
