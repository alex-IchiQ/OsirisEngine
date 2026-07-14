# Shared DLL: the reflection registry must be a single instance per process.
add_library(osiris_reflect SHARED
    src/registry.cpp
)

add_library(osiris::reflect ALIAS osiris_reflect)

set_target_properties(osiris_reflect PROPERTIES OUTPUT_NAME osiris.reflect)

target_include_directories(osiris_reflect PUBLIC include)

target_compile_definitions(osiris_reflect PRIVATE OSIRIS_REFLECT_EXPORTS)

target_compile_features(osiris_reflect PUBLIC cxx_std_23)

add_subdirectory(tests)
