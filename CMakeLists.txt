cmake_minimum_required(VERSION 3.16)
project(Willow) 
set(CMAKE_CXX_STANDARD 20)
#add_definitions(-Werror=return-type)

add_subdirectory(external)
add_subdirectory(test)
add_subdirectory(src)

add_executable(CubeExample examples/Cube.cpp src/willow/console/ConsoleCore.cpp include/willow/console/ConsoleCore.hpp src/willow/rendering/OrthographicCamera3D.cpp include/willow/rendering/OrthographicCamera3D.hpp)
target_include_directories(CubeExample PRIVATE include)
target_link_libraries(CubeExample PRIVATE Willow)

add_executable(FontRenderer examples/FontRenderer.cpp)
target_include_directories(FontRenderer PRIVATE include)
target_link_libraries(FontRenderer PRIVATE Willow)

#target_compile_options(CubeExample PRIVATE -Werror)

set(WILO_ENGINE_SCRIPTS_PATH "\"${PROJECT_SOURCE_DIR}/scripts/\"" )
set(WILO_TESTS_SCRIPTS_PATH "" )
set(WILO_ENGINE_VERSION "0")
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/include/willow/root/WiloConfig.h.in "${CMAKE_CURRENT_SOURCE_DIR}/include/willow/root/WiloConfig.h")