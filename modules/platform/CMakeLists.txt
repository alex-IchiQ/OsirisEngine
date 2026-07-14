# osiris.platform: OS window and input. Shared DLL.
add_library(osiris_platform SHARED
    src/window.cpp
)

add_library(osiris::platform ALIAS osiris_platform)

set_target_properties(osiris_platform PROPERTIES OUTPUT_NAME osiris.platform)

target_include_directories(osiris_platform PUBLIC include)

target_compile_definitions(osiris_platform PRIVATE OSIRIS_PLATFORM_EXPORTS)

target_compile_features(osiris_platform PUBLIC cxx_std_23)

target_link_libraries(osiris_platform PRIVATE osiris::core)

if(WIN32)
    target_link_libraries(osiris_platform PRIVATE user32)
endif()
