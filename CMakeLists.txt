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



include(external/CMakeLists.txt)


add_library(Wua STATIC EXCLUDE_FROM_ALL)
target_include_directories(Wua PUBLIC include/willow/Lua)
target_sources(Wua
    PUBLIC
         include/willow/Lua/lua.hpp 
         include/willow/Lua/lapi.h
         include/willow/Lua/lauxlib.h
         include/willow/Lua/lcode.h
         include/willow/Lua/lctype.h
         include/willow/Lua/ldebug.h
         include/willow/Lua/ldo.h
         include/willow/Lua/lfunc.h
         include/willow/Lua/lgc.h
         include/willow/Lua/ljumptab.h
         include/willow/Lua/llex.h
         include/willow/Lua/llimits.h
         include/willow/Lua/lmem.h
         include/willow/Lua/lobject.h
         include/willow/Lua/lopcodes.h
         include/willow/Lua/lopnames.h
         include/willow/Lua/lparser.h
         include/willow/Lua/lprefix.h
         include/willow/Lua/lstate.h
         include/willow/Lua/lstring.h
         include/willow/Lua/ltable.h
         include/willow/Lua/ltm.h
         include/willow/Lua/lua.h
         include/willow/Lua/luaconf.h
         include/willow/Lua/lualib.h
         include/willow/Lua/lundump.h
         include/willow/Lua/lvm.h
         include/willow/Lua/lzio.h
    PRIVATE
	 src/willow/Lua/lapi.cpp
	 src/willow/Lua/lauxlib.cpp
	 src/willow/Lua/lbaselib.cpp
	 src/willow/Lua/lcode.cpp
	 src/willow/Lua/lcorolib.cpp
	 src/willow/Lua/lctype.cpp
	 src/willow/Lua/ldblib.cpp
	 src/willow/Lua/ldebug.cpp
	 src/willow/Lua/ldo.cpp
	 src/willow/Lua/ldump.cpp
	 src/willow/Lua/lgc.cpp
	 src/willow/Lua/lfunc.cpp
	 src/willow/Lua/linit.cpp
	 src/willow/Lua/liolib.cpp
	 src/willow/Lua/llex.cpp
	 src/willow/Lua/lmathlib.cpp
	 src/willow/Lua/lmem.cpp
	 src/willow/Lua/loadlib.cpp
	 src/willow/Lua/lobject.cpp
	 src/willow/Lua/lopcodes.cpp
	 src/willow/Lua/loslib.cpp
	 src/willow/Lua/lparser.cpp
	 src/willow/Lua/lstate.cpp
	 src/willow/Lua/lstring.cpp
	 src/willow/Lua/lstrlib.cpp
	 src/willow/Lua/ltable.cpp
	 src/willow/Lua/ltablib.cpp
	 src/willow/Lua/ltm.cpp
	 src/willow/Lua/lundump.cpp
	 src/willow/Lua/lutf8lib.cpp
	 src/willow/Lua/lvm.cpp
	 src/willow/Lua/lzio.cpp
        
        
)

add_library(WillowRoot STATIC EXCLUDE_FROM_ALL)
target_link_libraries(WillowRoot PUBLIC Wua)
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
 "include/willow/Lua/lua.hpp")


add_library(WillowRendering STATIC EXCLUDE_FROM_ALL include/willow/rendering/Scene.hpp src/willow/rendering/Scene.cpp "include/willow/Lua/lua.hpp")
target_link_libraries(WillowRendering PUBLIC WillowRoot)
target_link_libraries(WillowRendering PUBLIC glfw)
target_link_libraries(WillowRendering PUBLIC Vulkan::Vulkan)
target_link_libraries(WillowRendering PUBLIC glm)
target_link_libraries(WillowRendering PUBLIC spirv-cross-cpp)
target_include_directories(WillowRendering PUBLIC Vulkan::Vulkan)
target_include_directories(WillowRendering PUBLIC include)
target_include_directories(WillowRendering PRIVATE .)
#target_compile_options(WillowRendering PRIVATE -Wno-deprecated-volatile)
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


add_library(Willow STATIC EXCLUDE_FROM_ALL)
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
         "include/willow/Lua/lua.hpp")

add_executable(CubeExample "examples/CubeExample/CubeExample.cpp"  "include/willow/Lua/lua.hpp")
set_target_properties( CubeExample
  PROPERTIES
  BINARY_DIR "${CMAKE_BINARY_DIR}/CubeExample"
  ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/CubeExample"
  LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/CubeExample"
  RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/CubeExample"
)
get_target_property(CubeExample_BINARY_DIR CubeExample RUNTIME_OUTPUT_DIRECTORY)
message("cube example created at " ${CubeExample_BINARY_DIR})
target_include_directories(CubeExample PRIVATE include)
target_link_libraries(CubeExample PUBLIC Willow)

file(COPY "examples/CubeExample/Assets" DESTINATION ${CubeExample_BINARY_DIR})    

add_executable(FontRenderer "examples/FontRendering/FontRenderer.cpp"  "include/willow/Lua/lua.hpp")
target_include_directories(FontRenderer PRIVATE include)
#target_link_libraries(FontRenderer PRIVATE Willow)

#target_compile_options(CubeExample PRIVATE -Werror)

set(WLO_ROOT ${PROJECT_SOURCE_DIR} )
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/include/willow/root/WiloConfig.h.in "${CMAKE_CURRENT_SOURCE_DIR}/include/willow/root/WiloConfig.h")

file(COPY shaders DESTINATION ${CMAKE_BINARY_DIR}/Willow)
file(COPY scripts DESTINATION ${CMAKE_BINARY_DIR}/Willow)

add_subdirectory(test)