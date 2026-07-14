cmake_minimum_required(VERSION 3.28)

project(OsirisEngine
    VERSION 0.0.1
    LANGUAGES CXX
)

# C++23 for now; bump to C++26 once we switch to native reflection (codegen-based
# reflection today needs no C++26 language features).
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Keep every binary in one directory so DLLs sit next to the exe that loads them.
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

enable_testing()

include(${CMAKE_CURRENT_SOURCE_DIR}/cmake/OsirisReflect.cmake)

add_subdirectory(modules/core)
add_subdirectory(modules/reflect)
add_subdirectory(modules/platform)
add_subdirectory(modules/rhi)
add_subdirectory(modules/app)

add_subdirectory(apps/Sandbox)
