cmake_minimum_required(VERSION 3.16)

add_library(Wua STATIC EXCLUDE_FROM_ALL)
target_include_directories(Wua PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/../include/willow/Lua>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
        )


set(WuaHeaders
        )
target_sources(Wua
        PRIVATE
       ${PROJECT_SOURCE_DIR}/include/willow/Lua/lua.hpp
       ${PROJECT_SOURCE_DIR}/include/willow/Lua/lapi.h
       ${PROJECT_SOURCE_DIR}/include/willow/Lua/lauxlib.h
       ${PROJECT_SOURCE_DIR}/include/willow/Lua/lcode.h
       ${PROJECT_SOURCE_DIR}/include/willow/Lua/lctype.h
       ${PROJECT_SOURCE_DIR}/include/willow/Lua/ldebug.h
       ${PROJECT_SOURCE_DIR}/include/willow/Lua/ldo.h
       ${PROJECT_SOURCE_DIR}/include/willow/Lua/lfunc.h
       ${PROJECT_SOURCE_DIR}/include/willow/Lua/lgc.h
       ${PROJECT_SOURCE_DIR}/include/willow/Lua/ljumptab.h
       ${PROJECT_SOURCE_DIR}/include/willow/Lua/llex.h
       ${PROJECT_SOURCE_DIR}/include/willow/Lua/llimits.h
       ${PROJECT_SOURCE_DIR}/include/willow/Lua/lmem.h
       ${PROJECT_SOURCE_DIR}/include/willow/Lua/lobject.h
       ${PROJECT_SOURCE_DIR}/include/willow/Lua/lopcodes.h
       ${PROJECT_SOURCE_DIR}/include/willow/Lua/lopnames.h
       ${PROJECT_SOURCE_DIR}/include/willow/Lua/lparser.h
       ${PROJECT_SOURCE_DIR}/include/willow/Lua/lprefix.h
       ${PROJECT_SOURCE_DIR}/include/willow/Lua/lstate.h
       ${PROJECT_SOURCE_DIR}/include/willow/Lua/lstring.h
       ${PROJECT_SOURCE_DIR}/include/willow/Lua/ltable.h
       ${PROJECT_SOURCE_DIR}/include/willow/Lua/ltm.h
       ${PROJECT_SOURCE_DIR}/include/willow/Lua/lua.h
       ${PROJECT_SOURCE_DIR}/include/willow/Lua/luaconf.h
       ${PROJECT_SOURCE_DIR}/include/willow/Lua/lualib.h
       ${PROJECT_SOURCE_DIR}/include/willow/Lua/lundump.h
       ${PROJECT_SOURCE_DIR}/include/willow/Lua/lvm.h
       ${PROJECT_SOURCE_DIR}/include/willow/Lua/lzio.h
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



add_library(WillowRoot STATIC EXCLUDE_FROM_ALL)
set_property(TARGET WillowRoot PROPERTY CXX_STANDARD_REQUIRED 20)
target_link_libraries(WillowRoot PUBLIC Wua)
target_link_libraries(WillowRoot PUBLIC spdlog)
target_link_libraries(WillowRoot PUBLIC glm::glm)

target_include_directories(WillowRoot PUBLIC
       ${PROJECT_SOURCE_DIR}/include
        )

        message("WillowRoot include dirs ${PROJECT_SOURCE_DIR}/include")


target_sources(WillowRoot
        PUBLIC
        ${PROJECT_SOURCE_DIR}/include/willow/input/InputManager.hpp

       ${PROJECT_SOURCE_DIR}/include/willow/root/Root.hpp
       ${PROJECT_SOURCE_DIR}/include/willow/root/Logger.hpp
       ${PROJECT_SOURCE_DIR}/include/willow/root/FileSystem.hpp
       ${PROJECT_SOURCE_DIR}/include/willow/root/PlatformCodes.hpp
       ${PROJECT_SOURCE_DIR}/include/willow/root/Tag.hpp
       ${PROJECT_SOURCE_DIR}/include/willow/root/WiloConfig.h
       ${PROJECT_SOURCE_DIR}/include/willow/root/WiloConfig.h.in
       ${PROJECT_SOURCE_DIR}/include/willow/root/EngineComponent.hpp
        ${PROJECT_SOURCE_DIR}/include/willow/ecs/EntityComponentSystem.hpp
        ${PROJECT_SOURCE_DIR}/include/willow/ecs/Systems.hpp
        ../include/willow/ecs/EntityComponentSystem.hpp
        ${PROJECT_SOURCE_DIR}/include/willow/root/Core.hpp
       ${PROJECT_SOURCE_DIR}/include/willow/Assets.hpp

        ${PROJECT_SOURCE_DIR}/include/willow/Warden.hpp


       ${PROJECT_SOURCE_DIR}/include/willow/messaging/Messages.hpp
       ${PROJECT_SOURCE_DIR}/include/willow/messaging/MessageSystem.hpp
       ${PROJECT_SOURCE_DIR}/include/willow/messaging/Xylem.h
       ${PROJECT_SOURCE_DIR}/include/willow/scripting/LuaBinding.hpp
       ${PROJECT_SOURCE_DIR}/include/willow/scripting/Scripting.hpp
       ${PROJECT_SOURCE_DIR}/include/willow/scripting/LuaEnvironment.hpp
       ${PROJECT_SOURCE_DIR}/include/willow/scripting/LuaCommunication.hpp
        ${PROJECT_SOURCE_DIR}/include/willow/rendering/Texture.hpp

       ${PROJECT_SOURCE_DIR}/include/willow/data/Data.hpp
       ${PROJECT_SOURCE_DIR}/include/willow/data/Type.hpp
       ${PROJECT_SOURCE_DIR}/include/willow/data/View.hpp
       ${PROJECT_SOURCE_DIR}/include/willow/data/Value.hpp

        willow/Assets.cpp
        willow/rendering/Texture.cpp
        willow/root/Data.cpp
        willow/root/Value.cpp
        willow/root/Type.cpp
        PRIVATE
        willow/input/InputManager.cpp
        willow/root/FileSystem.cpp
        willow/root/Logger.cpp
        willow/root/EntityComponentSystem.cpp
        willow/root/Systems.cpp
        willow/root/Tag.cpp
        willow/root/Core.cpp
        willow/messaging/Xylem.cpp
        willow/messaging/messages.cpp
        willow/scripting/LuaEnvironment.cpp
        willow/scripting/LuaCommunication.cpp
         )


install(TARGETS spdlog EXPORT Willow DESTINATION ${CMAKE_INSTALL_LIBDIR})
install(TARGETS WillowRoot EXPORT Willow DESTINATION ${CMAKE_INSTALL_LIBDIR})

add_library(WillowRendering STATIC EXCLUDE_FROM_ALL ../include/willow/rendering/Mesh.hpp )
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
         ${PROJECT_SOURCE_DIR}/include/willow/window/Window.hpp
         ${PROJECT_SOURCE_DIR}/include/willow/rendering/Renderer.hpp
         ${PROJECT_SOURCE_DIR}/include/willow/rendering/Geometry.hpp
         ${PROJECT_SOURCE_DIR}/include/willow/rendering/Buffer.hpp
         ${PROJECT_SOURCE_DIR}/include/willow/rendering/Material.hpp
         ${PROJECT_SOURCE_DIR}/include/willow/Vulkan/VulkanSetup.hpp
         ${PROJECT_SOURCE_DIR}/include/willow/Vulkan/VulkanSwapchain.hpp
         ${PROJECT_SOURCE_DIR}/include/willow/Vulkan/VulkanRoot.hpp
         ${PROJECT_SOURCE_DIR}/include/willow/Vulkan/VulkanGraphicsPipelineFactory.hpp
         ${PROJECT_SOURCE_DIR}/include/willow/Vulkan/VulkanMemoryManager.hpp
         ${PROJECT_SOURCE_DIR}/include/willow/Vulkan/VulkanShaderCompiler.hpp
         ${PROJECT_SOURCE_DIR}/include/willow/Vulkan/VulkanTextureFactory.hpp
         ${PROJECT_SOURCE_DIR}/include/willow/Vulkan/VulkanCommandInterface.hpp
         ${PROJECT_SOURCE_DIR}/include/willow/rendering/PerspectiveCamera3D.hpp
         ${PROJECT_SOURCE_DIR}/include/willow/rendering/Scene.hpp
        PRIVATE
        ../include/willow/external/stb_image.h
            willow/rendering/Scene.cpp
            willow/window/GLFWWindow.cpp
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

add_library(Willow STATIC EXCLUDE_FROM_ALL ../include/willow/DefaultAssets.hpp willow/DefaultAssets.cpp )

set_property(TARGET Willow PROPERTY CXX_STANDARD_REQUIRED 20)
target_link_libraries(Willow PUBLIC WillowRendering)
target_include_directories(Willow PUBLIC
        ${CMAKE_CURRENT_LIST_DIR}/../include
        )
set(WillowHeaders
        ${PROJECT_SOURCE_DIR}/include/willow/Application.hpp
       ${PROJECT_SOURCE_DIR}/include/willow.hpp
        )
target_sources(Willow
        PUBLIC
        ${WillowHeaders}
        PRIVATE
        willow/Application.cpp
             willow/console/Font.cpp
             willow/console/wilo_console.cpp
             willow/console/ConsoleCore.cpp
        )
