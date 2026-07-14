# Reflection codegen integration.
#
# osiris_reflect_generate(<out_var> <header>) runs the libclang-based tool at build
# time and returns the generated .reflect.gen.cpp path in <out_var>. The caller adds
# that file to its target's sources. The command re-runs only when the header (or the
# tool) changes.

find_package(Python3 COMPONENTS Interpreter REQUIRED)

set(OSIRIS_REFLECT_CODEGEN "${CMAKE_SOURCE_DIR}/tools/reflect-codegen/reflect_codegen.py")
set(OSIRIS_REFLECT_INCLUDE_DIR "${CMAKE_SOURCE_DIR}/modules/reflect/include")

function(osiris_reflect_generate out_var header)
    get_filename_component(header_abs "${header}" ABSOLUTE)
    get_filename_component(header_dir "${header_abs}" DIRECTORY)
    get_filename_component(header_name "${header_abs}" NAME)
    get_filename_component(name_we "${header_abs}" NAME_WE)
    set(gen "${CMAKE_CURRENT_BINARY_DIR}/${name_we}.reflect.gen.cpp")

    add_custom_command(
        OUTPUT "${gen}"
        COMMAND "${Python3_EXECUTABLE}" "${OSIRIS_REFLECT_CODEGEN}"
                --input "${header_abs}"
                --output "${gen}"
                --include "${header_name}"
                -I "${header_dir}"
                -I "${OSIRIS_REFLECT_INCLUDE_DIR}"
        DEPENDS "${header_abs}" "${OSIRIS_REFLECT_CODEGEN}"
        COMMENT "reflect-codegen: ${header_name}"
        VERBATIM
    )

    set(${out_var} "${gen}" PARENT_SCOPE)
endfunction()
