# GoogleTest comes from vcpkg.
find_package(GTest CONFIG REQUIRED)

add_executable(osiris_reflect_tests
    reflect_test.cpp
)

target_link_libraries(osiris_reflect_tests PRIVATE
    osiris::reflect
    GTest::gtest
    GTest::gtest_main
)

target_compile_features(osiris_reflect_tests PRIVATE cxx_std_23)

include(GoogleTest)
gtest_discover_tests(osiris_reflect_tests)

# End-to-end: the codegen tool produces the registration for demo_component.hpp;
# nothing here uses OSIRIS_REFLECT_TYPE for DemoComponent.
osiris_reflect_generate(demo_component_gen "${CMAKE_CURRENT_SOURCE_DIR}/demo_component.hpp")

add_executable(osiris_reflect_codegen_tests
    codegen_test.cpp
    ${demo_component_gen}
)

target_include_directories(osiris_reflect_codegen_tests PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(osiris_reflect_codegen_tests PRIVATE
    osiris::reflect
    GTest::gtest
    GTest::gtest_main
)

target_compile_features(osiris_reflect_codegen_tests PRIVATE cxx_std_23)

gtest_discover_tests(osiris_reflect_codegen_tests)
