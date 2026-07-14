# GoogleTest comes from vcpkg.
find_package(GTest CONFIG REQUIRED)

add_executable(osiris_core_tests
    arena_test.cpp
)

target_link_libraries(osiris_core_tests PRIVATE
    osiris::core
    GTest::gtest
    GTest::gtest_main
)

target_compile_features(osiris_core_tests PRIVATE cxx_std_23)

include(GoogleTest)
gtest_discover_tests(osiris_core_tests)
