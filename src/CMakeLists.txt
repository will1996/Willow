cmake_minimum_required(VERSION 3.16)

include_directories(../include)
include_directories(../include/willow)
include_directories(../include/willow/dev-utils)
include_directories(../include/willow/dev-utils/Vulkan)
include_directories(../include/willow/input)
include_directories(../include/willow/messaging)
include_directories(../include/willow/rendering)
include_directories(../include/willow/Vulkan)
include_directories(../include/willow/root)
include_directories(../include/willow/scripting)
include_directories(../include/willow/window)
include_directories(../include/willow/ECS)



add_library(WillowRoot )
target_link_libraries(WillowRoot PUBLIC spdlog)
target_include_directories(WillowRoot PUBLIC ../external/spdlog/include)
target_sources(WillowRoot
        PUBLIC ../include/willow/root/logr.hpp
        PUBLIC ../include/willow/root/wilo_APICodes.hpp
        PUBLIC ../include/willow/root/wilo_engine_element.hpp
        PUBLIC ../include/willow/root/wilo_object_counter.hpp
        PUBLIC ../include/willow/root/WiloConfig.h
        PUBLIC ../include/willow/root/WiloConfig.h.in
        PUBLIC willow/root/wilo_engine_element.cpp
        PUBLIC willow/root/wilo_object_counter.cpp
        PUBLIC willow/root/logr.cpp
        )

add_library(WillowMessaging )
target_link_libraries(WillowMessaging PUBLIC WillowRoot)
target_include_directories(WillowMessaging PRIVATE willow/messaging)
target_sources(WillowMessaging
        PUBLIC ../include/willow/messaging/Messages.hpp
        PUBLIC ../include/willow/messaging/MessageSystem.hpp
        PUBLIC ../include/willow/messaging/Xylem.h
        PRIVATE willow/messaging/Xylem.cpp
        PRIVATE willow/messaging/messages.cpp
        PRIVATE willow/messaging/MessageSystem.cpp

        )

find_package(Vulkan REQUIRED)
add_library(WillowDevUtils ../include/willow/dev-utils/Helpers.h)
target_link_libraries(WillowDevUtils PUBLIC Vulkan::Vulkan)
target_link_libraries(WillowDevUtils PUBLIC WillowRoot)
target_sources(WillowDevUtils
     PUBLIC willow/Vulkan/vkutil.cpp
     PUBLIC willow/Vulkan/vkdevice.cpp
     PUBLIC willow/Vulkan/vkswapchain.cpp
     PUBLIC ../include/willow/dev-utils/Vulkan/vkdevice.hpp
     PUBLIC ../include/willow/dev-utils/Vulkan/vkswapchain.hpp
     PUBLIC ../include/willow/dev-utils/Vulkan/vkutil.hpp
     PUBLIC ../include/willow/dev-utils/Vulkan/vktypes.hpp
     PUBLIC ../include/willow/dev-utils/Vulkan/wilovk.hpp
     PUBLIC ../include/willow/root/wilo_dev_core.hpp
        )
add_library(WillowRendering )
target_link_libraries(WillowRendering PUBLIC Vulkan::Vulkan)
target_link_libraries(WillowRendering PUBLIC glm)
target_link_libraries(WillowRendering PUBLIC WillowMessaging)
target_link_libraries(WillowRendering PUBLIC WillowDevUtils)
target_link_libraries(WillowRendering PUBLIC glfw)
target_link_libraries(WillowRendering PUBLIC WillowRoot)
target_sources(WillowRendering
        PUBLIC ../include/willow/rendering/Tag.hpp
        PUBLIC ../include/willow/rendering/Renderer.hpp
        PUBLIC ../include/willow/rendering/DataLayout.hpp
        PUBLIC ../include/willow/rendering/RenderDataTypes.hpp
        PUBLIC ../include/willow/rendering/RenderPath.hpp
        PUBLIC ../include/willow/rendering/Frame.hpp
        PUBLIC ../include/willow/rendering/Buffer.hpp
        PUBLIC ../include/willow/rendering/Attachment.hpp
        PUBLIC ../include/willow/Vulkan/VulkanSetup.hpp
        PUBLIC ../include/willow/Vulkan/VulkanSwapchain.hpp
        PUBLIC ../include/willow/Vulkan/VulkanRoot.hpp
        PUBLIC ../include/willow/Vulkan/VulkanGraphicsPipelineFactory.hpp
        PUBLIC ../include/willow/Vulkan/VulkanMemoryManager.hpp
        PUBLIC ../include/willow/Vulkan/VulkanBuffer.hpp
        PUBLIC willow/rendering/Tag.cpp
        PUBLIC willow/rendering/Renderer.cpp
        PUBLIC willow/rendering/DataLayout.cpp
        PUBLIC willow/rendering/RenderPath.cpp
        PUBLIC willow/rendering/Shader.cpp
        PUBLIC willow/rendering/Attachment.cpp
        PUBLIC willow/dev-utils/Vulkan/VulkanSetup.cpp
        PUBLIC willow/Vulkan/VulkanSwapchain.cpp
        PUBLIC willow/Vulkan/VulkanRoot.cpp
        PUBLIC willow/Vulkan/VulkanBuffer.cpp
        PUBLIC willow/Vulkan/VulkanGraphicsPipelineFactory.cpp
        PUBLIC willow/rendering/OrthographicCamera3D.cpp
       #TO DELETE
        #PUBLIC willow/rendering/Vulkan/vkdevice.cpp
        #PUBLIC willow/rendering/Vulkan/vkutil.cpp
        #PUBLIC willow/rendering/Vulkan/vulkan_context.cpp
        #PUBLIC willow/rendering/Vulkan/vulkan_frame_buffer_array.cpp
        #PUBLIC willow/rendering/Vulkan/vulkan_graphics_pipeline.cpp
        #PUBLIC willow/rendering/Vulkan/vulkan_index_buffer.cpp
        #PUBLIC willow/rendering/Vulkan/vulkan_render_core.cpp
        #PUBLIC willow/rendering/Vulkan/vulkan_render_pass.cpp
        #PUBLIC willow/rendering/Vulkan/vulkan_shader.cpp
        #PUBLIC willow/rendering/Vulkan/vulkan_uniform_buffer_array.cpp
        #PUBLIC willow/rendering/Vulkan/vulkan_vertex_buffer.cpp
        #PUBLIC willow/rendering/wilo_renderer.cpp


        )

add_library(WillowWindow )
target_link_libraries(WillowWindow PUBLIC WillowRoot)
target_link_libraries(WillowWindow PUBLIC glfw)
target_link_libraries(WillowWindow PUBLIC WillowMessaging)
target_link_libraries(WillowWindow PUBLIC Vulkan::Vulkan)
target_sources(WillowWindow
        PUBLIC ../include/willow/window/mac_window.hpp
        PUBLIC ../include/willow/window/wilo_window.hpp
        PUBLIC ../include/willow/window/window.hpp
        PUBLIC willow/window/mac_window.cpp
        )

add_library(WillowScripting ../include/willow/ECS/ECS.h)
target_link_libraries(WillowScripting PUBLIC WillowRoot)
target_link_libraries(WillowScripting PUBLIC Lua)
target_link_libraries(WillowScripting PUBLIC WillowMessaging)
target_include_directories(WillowScripting PUBLIC ../include)
target_sources(WillowScripting
        PUBLIC ../include/willow/scripting/LuaBinding.hpp
        PUBLIC willow/scripting/LuaBinding.cpp
        PUBLIC ../include/willow/scripting/LuaEnvironment.h
        willow/scripting/LuaEnvironment.cpp
        )


add_library(Willow
        ../include/willow/application.hpp
        ../include/willow/console/wilo_console.hpp
        ../include/willow/console/ConsoleCore.hpp
        ../include/willow/console/FontMap.hpp
        ../include/willow.hpp
        willow/application.cpp
        willow/console/FontMap.cpp
        willow/console/wilo_console.cpp
        willow/console/ConsoleCore.cpp
        )
target_link_libraries(Willow PUBLIC WillowWindow)
target_link_libraries(Willow PUBLIC WillowScripting)
target_link_libraries(Willow PUBLIC WillowRendering)
target_link_libraries(Willow PUBLIC WillowRoot)
target_link_libraries(Willow PUBLIC WillowDevUtils)
target_include_directories(Willow PUBLIC willow/include)
