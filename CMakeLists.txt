cmake_minimum_required(VERSION 3.19)
project(Willow LANGUAGES CXX) 
set(CMAKE_CXX_STANDARD 20)
option(BUILD_SHARED_LIBS "using shared librares" ON)

add_library(Lua STATIC IMPORTED)

add_subdirectory(external)

cmake_minimum_required(VERSION 3.16)
find_package(Vulkan REQUIRED)

add_library(Willow EXCLUDE_FROM_ALL )
add_dependencies(Willow Lua)
target_link_libraries(Willow PUBLIC Lua)
target_link_libraries(Willow PUBLIC glfw)
target_link_libraries(Willow PUBLIC glm)
target_link_libraries(Willow PUBLIC spirv-cross-cpp)
target_link_libraries(Willow PUBLIC Vulkan::Vulkan)
target_link_libraries(Willow PUBLIC spdlog)
target_include_directories(Willow PUBLIC ../external/spdlog/include)
target_include_directories(Willow PUBLIC Vulkan::Vulkan)
target_include_directories(Willow PUBLIC willow/include)
target_sources(Willow 
        PUBLIC include/willow/root/Root.hpp
        PUBLIC include/willow/root/Logger.hpp
        PUBLIC include/willow/root/FileSystem.hpp
        PUBLIC include/willow/root/PlatformCodes.hpp
        PUBLIC include/willow/root/Tag.hpp
        PUBLIC include/willow/root/WiloConfig.h
        PUBLIC include/willow/root/WiloConfig.h.in
        PUBLIC src/willow/root/Logger.cpp
        PRIVATE src/willow/root/Tag.cpp

        PUBLIC include/willow/messaging/Messages.hpp
        PUBLIC include/willow/messaging/MessageSystem.hpp
        PUBLIC include/willow/messaging/Xylem.h
        PRIVATE src/willow/messaging/Xylem.cpp
        PRIVATE src/willow/messaging/messages.cpp
        PUBLIC include/willow/rendering/Renderer.hpp
        PUBLIC include/willow/rendering/DataLayout.hpp
        PUBLIC include/willow/rendering/RenderDataTypes.hpp
        PUBLIC include/willow/rendering/Frame.hpp
        PUBLIC include/willow/rendering/Geometry.hpp
        PUBLIC include/willow/rendering/Buffer.hpp
        PUBLIC include/willow/rendering/DataView.hpp
        PUBLIC include/willow/rendering/Model.hpp
        PUBLIC include/willow/rendering/Material.hpp
        PUBLIC include/willow/Vulkan/VulkanSetup.hpp
        PUBLIC include/willow/Vulkan/VulkanSwapchain.hpp
        PUBLIC include/willow/Vulkan/VulkanRoot.hpp
        PUBLIC include/willow/Vulkan/VulkanGraphicsPipelineFactory.hpp
        PUBLIC include/willow/Vulkan/VulkanMemoryManager.hpp
        PUBLIC include/willow/Vulkan/VulkanShaderCompiler.hpp
        PUBLIC include/willow/Vulkan/VulkanTextureFactory.hpp
        PUBLIC include/willow/Vulkan/VulkanCommandInterface.hpp
        PUBLIC include/willow/rendering/PerspectiveCamera3D.hpp
        PRIVATE external/stb_image.h
#        PUBLIC willow/rendering/Model.cpp
        PRIVATE src/willow/rendering/Renderer.cpp
        PRIVATE src/willow/rendering/DataLayout.cpp
#        PUBLIC willow/rendering/Shader.cpp
#        PUBLIC willow/rendering/DataView.cpp
        PRIVATE src/willow/rendering/PerspectiveCamera3D.cpp
        PRIVATE src/willow/dev-utils/Vulkan/VulkanSetup.cpp
        PRIVATE src/willow/Vulkan/VulkanSwapchain.cpp
        PRIVATE src/willow/Vulkan/VulkanRoot.cpp
        PRIVATE src/willow/Vulkan/VulkanShaderCompiler.cpp
        PRIVATE src/willow/Vulkan/VulkanGraphicsPipelineFactory.cpp
        PRIVATE src/willow/Vulkan/VulkanTextureFactory.cpp
        PRIVATE src/willow/Vulkan/VulkanMemoryManager.cpp
        PRIVATE src/willow/Vulkan/VulkanCommandInterface.cpp

        PUBLIC include/willow/window/Window.hpp
        PRIVATE src/willow/window/GLFWWindow.cpp

        PUBLIC include/willow/scripting/LuaBinding.hpp
        PUBLIC include/willow/scripting/LuaEnvironment.h
        PRIVATE src/willow/scripting/LuaEnvironment.cpp

        PUBLIC include/willow/Application.hpp
        PUBLIC include/willow/console/wilo_console.hpp
        PUBLIC include/willow/console/ConsoleCore.hpp
        PUBLIC include/willow/console/FontMap.hpp
        PUBLIC include/willow.hpp
        PRIVATE src/willow/application.cpp
        PRIVATE src/willow/console/FontMap.cpp
        PRIVATE src/willow/console/wilo_console.cpp
        PRIVATE src/willow/console/ConsoleCore.cpp
        )
target_compile_options(Willow PRIVATE -Wno-deprecated-volatile)#make glm happy, I don't use volatile floats, so we're all good

target_include_directories(Willow PUBLIC .)#get to external dependencies
target_include_directories(Willow PUBLIC include)

add_executable(CubeExample examples/Cube.cpp )
target_include_directories(CubeExample PRIVATE include)
target_link_libraries(CubeExample PUBLIC Willow)

add_executable(FontRenderer examples/FontRenderer.cpp )
target_include_directories(FontRenderer PRIVATE include)
#target_link_libraries(FontRenderer PRIVATE Willow)

#target_compile_options(CubeExample PRIVATE -Werror)

set(WILO_ENGINE_SCRIPTS_PATH "\"${PROJECT_SOURCE_DIR}/scripts/\"" )
set(WILO_TESTS_SCRIPTS_PATH "" )
set(WILO_ENGINE_VERSION "0")
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/include/willow/root/WiloConfig.h.in "${CMAKE_CURRENT_SOURCE_DIR}/include/willow/root/WiloConfig.h")

file(COPY shaders DESTINATION ${CMAKE_BINARY_DIR}/Willow)
