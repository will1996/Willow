cmake_minimum_required(VERSION 3.16)

message("BUILDING WILLOW SHADERS")
message("PASSED IN PROJECT ROOT  ${PROJECT_ROOT}")

find_program(GLSL_VALIDATOR glslangValidator HINTS /usr/bin /usr/local/bin $ENV{VULKAN_SDK}/Bin/ $ENV{VULKAN_SDK}/Bin32/ REQUIRED)
set(SHADERS_DIR ${PROJECT_ROOT}/Assets/Shaders)
set(SHADERS_OUTPUT_DIR ${CMAKE_BINARY_DIR}/Assets/Shaders)
message("LOOKING FOR SHADER SOURCE IN ${SHADERS_DIR}")
message("WRITING SHADER ARTIFACTS TO ${SHADERS_OUTPUT_DIR}")
file(GLOB_RECURSE GLSL_SOURCE_FILES
        "${SHADERS_DIR}/*.frag"
        "${SHADERS_DIR}/*.vert"
        "${SHADERS_DIR}/*.comp"
        )
foreach(GLSL ${GLSL_SOURCE_FILES})
    get_filename_component(FILE_NAME ${GLSL} NAME)
    message(STATUS "BUILDING SHADER ${FILE_NAME}")
    set(SPIRV "${SHADERS_OUTPUT_DIR}/${FILE_NAME}.spv")
    message(STATUS ${GLSL})
    execute_process( COMMAND ${GLSL_VALIDATOR} -V ${GLSL} -o ${SPIRV}
                    RESULT_VARIABLE CompileResult
                    ERROR_VARIABLE CompileOutput
                    OUTPUT_VARIABLE CompileOutput )
    if(CompileResult EQUAL 0)
        message("Compile Succeeded")
        else()
        message(FATAL_ERROR "${CompileOutput}")
        endif()
endforeach(GLSL)
