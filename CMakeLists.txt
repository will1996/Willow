cmake_minimum_required(VERSION 3.16)
project(Willow LANGUAGES CXX)
enable_testing()


set(CMAKE_CXX_STANDARD 20)

option(BUILD_SHARED_LIBS "using shared librares" FALSE)

if(MSVC)
    set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS TRUE)
    set(BUILD_SHARED_LIBS FALSE )
endif()
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}")

include(include/willow/external/CMakeLists.txt)

add_subdirectory(src)
add_subdirectory(examples)
add_subdirectory(test)

#target_compile_options(CubeExample PRIVATE -Werror)

set(WLO_ROOT ${CMAKE_BINARY_DIR} )
message(WILLOW ROOT ${WLO_ROOT})
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/include/willow/root/WiloConfig.h.in "${CMAKE_CURRENT_SOURCE_DIR}/include/willow/root/WiloConfig.h")

file(COPY shaders DESTINATION ${CMAKE_BINARY_DIR}/Willow)
file(COPY scripts DESTINATION ${CMAKE_BINARY_DIR}/Willow)
