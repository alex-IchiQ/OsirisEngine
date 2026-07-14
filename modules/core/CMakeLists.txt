# Shared DLL: osiris.core must exist as a single instance per process (shared log,
# allocator, reflection registry across modules — see docs/architecture/08).
add_library(osiris_core SHARED
    src/log.cpp
    src/job_system.cpp
)

add_library(osiris::core ALIAS osiris_core)

set_target_properties(osiris_core PROPERTIES OUTPUT_NAME osiris.core)

target_include_directories(osiris_core PUBLIC include)

# OSIRIS_CORE_EXPORTS marks symbols for export only while building the DLL.
target_compile_definitions(osiris_core PRIVATE OSIRIS_CORE_EXPORTS)

target_compile_features(osiris_core PUBLIC cxx_std_23)

add_subdirectory(tests)
