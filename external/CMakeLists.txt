
cmake_minimum_required(VERSION 3.16)

add_subdirectory(spdlog)

if(NOT TARGET spdlog)
    # Stand-alone build
    find_package(spdlog REQUIRED)
endif()

add_subdirectory(glm-0.9.9.7)

add_subdirectory(glfw-3.3.2)


add_subdirectory(lua-5.3.5)

add_subdirectory(glslang)

add_subdirectory(VulkanMemoryAllocator)
