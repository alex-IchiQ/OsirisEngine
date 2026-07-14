#pragma once

// Symbol visibility for the osiris.app shared library. See core_api.hpp.

#if defined(_WIN32)
    #if defined(OSIRIS_APP_EXPORTS)
        #define OSIRIS_APP_API __declspec(dllexport)
    #else
        #define OSIRIS_APP_API __declspec(dllimport)
    #endif
#else
    #define OSIRIS_APP_API __attribute__((visibility("default")))
#endif
