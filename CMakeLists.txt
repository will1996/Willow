cmake_minimum_required(VERSION 3.16)
project(Willow LANGUAGES CXX)
enable_testing()


set(CMAKE_CXX_STANDARD 20)

if(MSVC)
    set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS TRUE)
    set(BUILD_SHARED_LIBS TRUE)
endif()
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}")

option(BUILD_SHARED_LIBS "using shared librares" ON)


include(external/CMakeLists.txt)


add_library(WillowRoot EXCLUDE_FROM_ALL)
target_link_libraries(WillowRoot PUBLIC lua)
target_link_libraries(WillowRoot PUBLIC spdlog)
target_include_directories(WillowRoot PUBLIC include)

target_sources(WillowRoot
        PUBLIC
        include/willow/root/Root.hpp
        include/willow/root/Logger.hpp
        include/willow/root/FileSystem.hpp
        include/willow/root/PlatformCodes.hpp
        include/willow/root/Tag.hpp
        include/willow/root/WiloConfig.h
        include/willow/root/WiloConfig.h.in
        include/willow/messaging/Messages.hpp
        include/willow/messaging/MessageSystem.hpp
        include/willow/messaging/Xylem.h
        include/willow/scripting/LuaBinding.hpp
        include/willow/scripting/LuaEnvironment.h
        PRIVATE
        src/willow/root/Logger.cpp
        src/willow/root/Tag.cpp
        src/willow/messaging/Xylem.cpp
        src/willow/messaging/messages.cpp
        src/willow/scripting/LuaEnvironment.cpp
)


add_library(WillowRendering EXCLUDE_FROM_ALL include/willow/rendering/Scene.hpp src/willow/rendering/Scene.cpp)
target_link_libraries(WillowRendering PUBLIC WillowRoot)
target_link_libraries(WillowRendering PUBLIC glfw)
target_link_libraries(WillowRendering PUBLIC Vulkan::Vulkan)
target_link_libraries(WillowRendering PUBLIC glm)
target_link_libraries(WillowRendering PUBLIC spirv-cross-cpp)
target_include_directories(WillowRendering PUBLIC Vulkan::Vulkan)
target_include_directories(WillowRendering PUBLIC include)
target_include_directories(WillowRendering PRIVATE .)
target_compile_options(WillowRendering PRIVATE -Wno-deprecated-volatile)
target_sources(WillowRendering
        #for now, since we're using GLFW windows and rendering are basically the same. For this reason, let's keep them in the same target
        PUBLIC include/willow/window/Window.hpp
        PRIVATE src/willow/window/GLFWWindow.cpp
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
        PUBLIC  src/willow/rendering/Model.cpp
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
)


add_library(Willow EXCLUDE_FROM_ALL)
target_link_libraries(Willow PUBLIC WillowRendering)
target_include_directories(Willow PUBLIC include)
target_sources(Willow
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

add_subdirectory(test)