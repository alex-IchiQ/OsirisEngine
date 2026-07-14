# osiris.app: thin application framework (game loop). Shared DLL, depends on osiris.core.
add_library(osiris_app SHARED
    src/application.cpp
)

add_library(osiris::app ALIAS osiris_app)

set_target_properties(osiris_app PROPERTIES OUTPUT_NAME osiris.app)

target_include_directories(osiris_app PUBLIC include)

target_compile_definitions(osiris_app PRIVATE OSIRIS_APP_EXPORTS)

target_compile_features(osiris_app PUBLIC cxx_std_23)

target_link_libraries(osiris_app PUBLIC osiris::core)

# main() lives in a static lib (not the shared osiris.app, and not a header) so the
# engine owns the entry point while executables just link it and provide create_application().
add_library(osiris_app_entry STATIC
    src/entry_point.cpp
)

add_library(osiris::app_entry ALIAS osiris_app_entry)

target_link_libraries(osiris_app_entry PUBLIC osiris::app)
