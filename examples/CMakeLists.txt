cmake_minimum_required(VERSION 3.16)

add_custom_target(Shaders
        COMMAND ${CMAKE_COMMAND} -DPROJECT_ROOT=${PROJECT_SOURCE_DIR} -P ${PROJECT_SOURCE_DIR}/CMake/BuildWillowShaders.cmake
        )

function(WillowAddExample ExampleName)
        message("CREATING WILLOW EXAMPLE: ${ExampleName} ")
        set_target_properties( ${ExampleName}
                PROPERTIES
                BINARY_DIR "${CMAKE_BINARY_DIR}/${ExampleName}"
                ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/${ExampleName}"
                LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/${ExampleName}"
                RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/${ExampleName}"
                )
        get_target_property(EXAMPLE_BINARY_DIR ${ExampleName} RUNTIME_OUTPUT_DIRECTORY)
        message("${ExampleName} created at " ${EXAMPLE_BINARY_DIR})
        target_include_directories(${ExampleName} PRIVATE include)
        target_link_libraries(${ExampleName} PUBLIC Willow)

        make_directory(${EXAMPLE_BINARY_DIR}/Assets/Shaders)
        make_directory(${EXAMPLE_BINARY_DIR}/Assets/Textures)
        make_directory(${EXAMPLE_BINARY_DIR}/Assets/Scripts)

        set(AssetSource ${CMAKE_CURRENT_LIST_DIR}/${ExampleName}/Assets)
        set(AssetDestination ${EXAMPLE_BINARY_DIR}/Assets)


        file(GLOB_RECURSE WillowScripts "../scripts/*")
        file(COPY ${WillowScripts} DESTINATION ${AssetDestination}/Scripts/)

        file(GLOB_RECURSE ExampleTextures "${AssetSource}/Textures/*")
        file(COPY ${ExampleTextures} DESTINATION ${AssetDestination}/Textures/)


        file(GLOB_RECURSE GLSL_SOURCE_FILES
                "${AssetSource}/Shaders/*.frag"
                "${AssetSource}/Shaders/*.vert"
                "${AssetSource}/Shaders/*.comp"
                )

        find_program(GLSL_VALIDATOR glslangValidator HINTS /usr/bin /usr/local/bin $ENV{VULKAN_SDK}/Bin/ $ENV{VULKAN_SDK}/Bin32/ REQUIRED)
        foreach(GLSL ${GLSL_SOURCE_FILES})
                get_filename_component(FILE_NAME ${GLSL} NAME)
                message(STATUS "BUILDING SHADER ${FILE_NAME}")
                set(SPIRV "${AssetDestination}/Shaders/${FILE_NAME}.spv")
                message(STATUS ${GLSL})
                execute_process( COMMAND ${GLSL_VALIDATOR} -V ${GLSL} -o ${SPIRV}
                        RESULT_VARIABLE CompileResult
                        ERROR_VARIABLE CompileOutput
                        OUTPUT_VARIABLE CompileOutput )
                if(CompileResult EQUAL 0)
                        message("Compile Succeeded")
                else()
                        message("Shader Compilation Failed for ${SPIRV}")
                        message(FATAL_ERROR "${CompileOutput}")
                endif()
        endforeach(GLSL)
endfunction()
add_executable(CubeExample "CubeExample/CubeExample.cpp")
add_dependencies(CubeExample Shaders)
WillowAddExample(CubeExample)

add_executable(FontRendering "FontRendering/FontRenderer.cpp")
WillowAddExample(FontRendering )


add_executable(AsteroidsExample "AsteroidsExample/AsteroidsExample.cpp")
WillowAddExample(AsteroidsExample)

add_executable(TriangleTest "TriangleTest/TriangleTest.cpp")
add_dependencies(TriangleTest Shaders)
WillowAddExample(TriangleTest)


