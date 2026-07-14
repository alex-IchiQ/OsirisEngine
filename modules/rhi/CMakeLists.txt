# osiris.rhi: render hardware interface. D3D12 backend (concrete for now). Shared DLL.
find_package(slang CONFIG REQUIRED)

add_library(osiris_rhi SHARED
    src/device.cpp
    src/swapchain.cpp
    src/shader_compiler.cpp
    src/pipeline.cpp
    src/renderer.cpp
)

add_library(osiris::rhi ALIAS osiris_rhi)

set_target_properties(osiris_rhi PROPERTIES OUTPUT_NAME osiris.rhi)

target_include_directories(osiris_rhi PUBLIC include)

target_compile_definitions(osiris_rhi PRIVATE OSIRIS_RHI_EXPORTS)

target_compile_features(osiris_rhi PUBLIC cxx_std_23)

target_link_libraries(osiris_rhi PUBLIC osiris::core PRIVATE slang::slang)

if(WIN32)
    target_link_libraries(osiris_rhi PRIVATE d3d12 dxgi dxguid)
endif()
