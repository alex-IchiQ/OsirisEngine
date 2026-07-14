add_executable(Sandbox
    src/sandbox_app.cpp
)

target_compile_features(Sandbox PRIVATE cxx_std_23)

# osiris::app_entry provides main() and pulls in osiris::app transitively.
# osiris::core is listed explicitly because sandbox_app.cpp uses the log directly.
target_link_libraries(Sandbox PRIVATE osiris::app_entry osiris::core osiris::platform osiris::rhi)
