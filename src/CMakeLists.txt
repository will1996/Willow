cmake_minimum_required(VERSION 3.16)
include(GNUInstallDirs)
message(INCLUDED GNUINSTALLDIRS)
message(${CMAKE_INSTALL_LIBDIR})
message(${CMAKE_INSTALL_INCLUDEDIR})
message(CURRENT LIST DIR)
message(${CMAKE_CURRENT_LIST_DIR})

add_library(Wua STATIC EXCLUDE_FROM_ALL)
target_include_directories(Wua PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/../include/willow/Lua>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
        )


set(WuaHeaders
       $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include/willow/Lua/lua.hpp>
       $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include/willow/Lua/lapi.h>
       $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include/willow/Lua/lauxlib.h>
       $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include/willow/Lua/lcode.h>
       $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include/willow/Lua/lctype.h>
       $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include/willow/Lua/ldebug.h>
       $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include/willow/Lua/ldo.h>
       $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include/willow/Lua/lfunc.h>
       $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include/willow/Lua/lgc.h>
       $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include/willow/Lua/ljumptab.h>
       $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include/willow/Lua/llex.h>
       $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include/willow/Lua/llimits.h>
       $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include/willow/Lua/lmem.h>
       $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include/willow/Lua/lobject.h>
       $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include/willow/Lua/lopcodes.h>
       $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include/willow/Lua/lopnames.h>
       $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include/willow/Lua/lparser.h>
       $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include/willow/Lua/lprefix.h>
       $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include/willow/Lua/lstate.h>
       $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include/willow/Lua/lstring.h>
       $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include/willow/Lua/ltable.h>
       $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include/willow/Lua/ltm.h>
       $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include/willow/Lua/lua.h>
       $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include/willow/Lua/luaconf.h>
       $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include/willow/Lua/lualib.h>
       $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include/willow/Lua/lundump.h>
       $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include/willow/Lua/lvm.h>
       $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include/willow/Lua/lzio.h>

        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/willow/Lua/lua.hpp>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/willow/Lua/lapi.h>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/willow/Lua/lauxlib.h>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/willow/Lua/lcode.h>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/willow/Lua/lctype.h>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/willow/Lua/ldebug.h>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/willow/Lua/ldo.h>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/willow/Lua/lfunc.h>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/willow/Lua/lgc.h>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/willow/Lua/ljumptab.h>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/willow/Lua/llex.h>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/willow/Lua/llimits.h>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/willow/Lua/lmem.h>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/willow/Lua/lobject.h>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/willow/Lua/lopcodes.h>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/willow/Lua/lopnames.h>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/willow/Lua/lparser.h>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/willow/Lua/lprefix.h>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/willow/Lua/lstate.h>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/willow/Lua/lstring.h>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/willow/Lua/ltable.h>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/willow/Lua/ltm.h>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/willow/Lua/lua.h>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/willow/Lua/luaconf.h>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/willow/Lua/lualib.h>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/willow/Lua/lundump.h>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/willow/Lua/lvm.h>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/willow/Lua/lzio.h>
        )
target_sources(Wua
        PRIVATE
           ${WuaHeaders}
        willow/Lua/lapi.cpp
        willow/Lua/lauxlib.cpp
        willow/Lua/lbaselib.cpp
        willow/Lua/lcode.cpp
        willow/Lua/lcorolib.cpp
        willow/Lua/lctype.cpp
        willow/Lua/ldblib.cpp
        willow/Lua/ldebug.cpp
        willow/Lua/ldo.cpp
        willow/Lua/ldump.cpp
        willow/Lua/lgc.cpp
        willow/Lua/lfunc.cpp
        willow/Lua/linit.cpp
        willow/Lua/liolib.cpp
        willow/Lua/llex.cpp
        willow/Lua/lmathlib.cpp
        willow/Lua/lmem.cpp
        willow/Lua/loadlib.cpp
        willow/Lua/lobject.cpp
        willow/Lua/lopcodes.cpp
        willow/Lua/loslib.cpp
        willow/Lua/lparser.cpp
        willow/Lua/lstate.cpp
        willow/Lua/lstring.cpp
        willow/Lua/lstrlib.cpp
        willow/Lua/ltable.cpp
        willow/Lua/ltablib.cpp
        willow/Lua/ltm.cpp
        willow/Lua/lundump.cpp
        willow/Lua/lutf8lib.cpp
        willow/Lua/lvm.cpp
        willow/Lua/lzio.cpp
        )

install(TARGETS Wua EXPORT Wua DESTINATION "${CMAKE_INSTALL_LIBDIR}")
install(FILES ${WuaHeaders} DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}")
install(FILES ../CMake/WuaConfig.cmake DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/Wua)
install(EXPORT Wua
        DESTINATION
            ${CMAKE_INSTALL_LIBDIR}/cmake/Wua
        )


add_library(WillowRoot STATIC EXCLUDE_FROM_ALL)
set_property(TARGET WillowRoot PROPERTY CXX_STANDARD_REQUIRED 20)
target_link_libraries(WillowRoot PUBLIC Wua)
target_link_libraries(WillowRoot PUBLIC spdlog)

find_package(glm REQUIRED)
if(MSVC)

set_target_properties(glm::glm PROPERTIES INTERFACE_INCLUDE_DIRECTORIES "C:\\Program Files\\include")
else()
set_target_properties(glm::glm PROPERTIES INTERFACE_INCLUDE_DIRECTORIES /usr/local/include)
endif()

target_link_libraries(WillowRoot PUBLIC glm::glm)

target_include_directories(WillowRoot PUBLIC
        $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
        )


target_sources(WillowRoot
        PUBLIC
        $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include/willow/input/InputManager.hpp>

       $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include/willow/root/Root.hpp>
       $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include/willow/root/Logger.hpp>
       $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include/willow/root/FileSystem.hpp>
       $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include/willow/root/PlatformCodes.hpp>
       $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include/willow/root/Tag.hpp>
       $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include/willow/root/WiloConfig.h>
       $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include/willow/root/WiloConfig.h.in>
       $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include/willow/messaging/Messages.hpp>
       $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include/willow/messaging/MessageSystem.hpp>
       $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include/willow/messaging/Xylem.h>
       $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include/willow/scripting/LuaBinding.hpp>
        $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include/willow/scripting/Scripting.hpp>
       $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include/willow/scripting/LuaEnvironment.hpp>
        $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include/willow/scripting/LuaCommunication.hpp>

        $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include/willow/data/Data.hpp>
        $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include/willow/data/Type.hpp>
        $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include/willow/data/View.hpp>
        $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include/willow/data/Value.hpp>

        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/willow/root/Root.hpp>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/willow/input/InputManager.hpp>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/willow/root/Logger.hpp>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/willow/root/FileSystem.hpp>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/willow/root/PlatformCodes.hpp>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/willow/root/Tag.hpp>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/willow/root/WiloConfig.h>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/willow/root/WiloConfig.h.in>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/willow/messaging/Messages.hpp>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/willow/messaging/MessageSystem.hpp>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/willow/messaging/Xylem.h>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/willow/scripting/Scripting.hpp>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/willow/scripting/LuaBinding.hpp>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/willow/scripting/LuaEnvironment.hpp>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/willow/scripting/LuaCommunication.hpp>

        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/willow/data/Data.hpp>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/willow/data/Type.hpp>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/willow/data/View.hpp>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/willow/data/Value.hpp>
        PRIVATE
        willow/root/Type.cpp
        willow/root/Value.cpp
        willow/input/InputManager.cpp
        willow/root/FileSystem.cpp
        willow/root/Logger.cpp
        willow/root/Tag.cpp
        willow/messaging/Xylem.cpp
        willow/messaging/messages.cpp
        willow/scripting/LuaEnvironment.cpp
        willow/scripting/LuaCommunication.cpp
        )


install(TARGETS spdlog EXPORT Willow DESTINATION ${CMAKE_INSTALL_LIBDIR})
install(TARGETS WillowRoot EXPORT Willow DESTINATION ${CMAKE_INSTALL_LIBDIR})

add_library(WillowRendering STATIC EXCLUDE_FROM_ALL ../include/willow/rendering/Mesh.hpp)
target_link_libraries(WillowRendering PUBLIC WillowRoot)
target_link_libraries(WillowRendering PUBLIC glfw)
target_link_libraries(WillowRendering PUBLIC Vulkan::Vulkan)
target_link_libraries(WillowRendering PUBLIC spirv-cross-cpp)

target_include_directories(WillowRendering
        PRIVATE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/..>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
        )
#target_compile_options(WillowRendering PRIVATE -Wno-deprecated-volatile)
target_sources(WillowRendering
        #for now, since we're using GLFW windows and rendering are basically the same. For this reason, let's keep them in the same target
        PUBLIC
         $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include/willow/window/Window.hpp>
         $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include/willow/rendering/Renderer.hpp>
         $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include/willow/rendering/RenderDataTypes.hpp>
         $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include/willow/rendering/Frame.hpp>
         $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include/willow/rendering/Geometry.hpp>
         $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include/willow/rendering/Buffer.hpp>
         $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include/willow/rendering/Model.hpp>
         $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include/willow/rendering/Material.hpp>
         $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include/willow/Vulkan/VulkanSetup.hpp>
         $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include/willow/Vulkan/VulkanSwapchain.hpp>
         $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include/willow/Vulkan/VulkanRoot.hpp>
         $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include/willow/Vulkan/VulkanGraphicsPipelineFactory.hpp>
         $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include/willow/Vulkan/VulkanMemoryManager.hpp>
         $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include/willow/Vulkan/VulkanShaderCompiler.hpp>
         $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include/willow/Vulkan/VulkanTextureFactory.hpp>
         $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include/willow/Vulkan/VulkanCommandInterface.hpp>
         $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include/willow/rendering/PerspectiveCamera3D.hpp>
         $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include/willow/rendering/Scene.hpp>
        $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include/willow/rendering/Texture.hpp>

        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/willow/window/Window.hpp>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/willow/rendering/Renderer.hpp>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/willow/rendering/RenderDataTypes.hpp>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/willow/rendering/Frame.hpp>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/willow/rendering/Geometry.hpp>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/willow/rendering/Buffer.hpp>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/willow/rendering/Model.hpp>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/willow/rendering/Material.hpp>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/willow/Vulkan/VulkanSetup.hpp>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/willow/Vulkan/VulkanSwapchain.hpp>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/willow/Vulkan/VulkanRoot.hpp>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/willow/Vulkan/VulkanGraphicsPipelineFactory.hpp>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/willow/Vulkan/VulkanMemoryManager.hpp>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/willow/Vulkan/VulkanShaderCompiler.hpp>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/willow/Vulkan/VulkanTextureFactory.hpp>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/willow/Vulkan/VulkanCommandInterface.hpp>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/willow/rendering/PerspectiveCamera3D.hpp>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/willow/rendering/Scene.hpp>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/willow/rendering/Texture.hpp>
        PRIVATE
        ../include/willow/external/stb_image.h
            willow/rendering/Scene.cpp
            willow/rendering/Texture.cpp
            willow/window/GLFWWindow.cpp
            willow/rendering/Model.cpp
            willow/rendering/Renderer.cpp
            willow/rendering/PerspectiveCamera3D.cpp
            willow/dev-utils/Vulkan/VulkanSetup.cpp
            willow/Vulkan/VulkanSwapchain.cpp
            willow/Vulkan/VulkanRoot.cpp
            willow/Vulkan/VulkanShaderCompiler.cpp
            willow/Vulkan/VulkanGraphicsPipelineFactory.cpp
            willow/Vulkan/VulkanTextureFactory.cpp
            willow/Vulkan/VulkanMemoryManager.cpp
            willow/Vulkan/VulkanCommandInterface.cpp
        )
install(TARGETS WillowRendering EXPORT Willow DESTINATION ${CMAKE_INSTALL_LIBDIR})

add_library(Willow STATIC EXCLUDE_FROM_ALL ../include/willow/DefaultAssets.hpp willow/DefaultAssets.cpp)
set_property(TARGET Willow PROPERTY CXX_STANDARD_REQUIRED 20)
target_link_libraries(Willow PUBLIC WillowRendering)
target_include_directories(Willow PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/..>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
        )
set(WillowHeaders
       $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include/willow/Application.hpp>
       $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include/willow.hpp>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/willow/Application.hpp>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/willow.hpp>
        )
target_sources(Willow
        PUBLIC
        ${WillowHeaders}
        PRIVATE
             willow/application.cpp
        willow/console/Font.cpp
             willow/console/wilo_console.cpp
             willow/console/ConsoleCore.cpp
        )

install(FILES ../CMake/WillowConfig.cmake DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/Willow)
install(FILES ../CMake/BuildWillowShaders.cmake DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/Willow)
install(FILES ../CMake/MoveWillowAssets.cmake DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/Willow)
install(TARGETS Willow EXPORT Willow DESTINATION ${CMAKE_INSTALL_LIBDIR}/Willow)

install(EXPORT Willow
        DESTINATION
        ${CMAKE_INSTALL_LIBDIR}/cmake/Willow
        )



install(DIRECTORY ../include/willow DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}")
install(FILES ../include/willow.hpp DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}")
install(FILES ../include/willow/external/sol.hpp DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}")


install(DIRECTORY ../scripts DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/Willow/Willow")
