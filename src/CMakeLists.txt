cmake_minimum_required(VERSION 3.16)

include_directories(../include)
include_directories(../include/willow)
include_directories(../include/willow/dev-utils)
include_directories(../include/willow/dev-utils/Vulkan)
include_directories(../include/willow/input)
include_directories(../include/willow/messaging)
include_directories(../include/willow/rendering)
include_directories(../include/willow/rendering/Vulkan)
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
target_sources(WillowMessaging
        PUBLIC ../include/willow/messaging/keyboard_message.hpp
        PUBLIC ../include/willow/messaging/mouse_message.hpp
        PUBLIC ../include/willow/messaging/wilo_observer.hpp
        PUBLIC ../include/willow/messaging/wilo_subject.hpp
        PUBLIC ../include/willow/messaging/window_message.hpp
        PUBLIC ../include/willow/messaging/wilo_message.hpp
        PUBLIC ../include/willow/messaging/wilo_observer_system.hpp
        PUBLIC ../include/willow/messaging/Xylem.h
        PRIVATE willow/messaging/Xylem.cpp
        PRIVATE willow/messaging/keyboard_message.cpp
        PRIVATE willow/messaging/mouse_message.cpp
        PRIVATE willow/messaging/wilo_observer.cpp
        PRIVATE willow/messaging/wilo_subject.cpp
        PRIVATE willow/messaging/window_message.cpp

        )

find_package(Vulkan REQUIRED)
add_library(WillowDevUtils SHARED ../include/willow/dev-utils/Helpers.h)
target_link_libraries(WillowDevUtils PUBLIC Vulkan::Vulkan)
target_link_libraries(WillowDevUtils PUBLIC WillowRoot)
target_sources(WillowDevUtils
     PUBLIC ../include/willow/dev-utils/Vulkan/vkcore.hpp
     PUBLIC willow/rendering/Vulkan/vkcore.cpp
     PUBLIC willow/rendering/Vulkan/vkutil.cpp
     PUBLIC willow/rendering/Vulkan/vkdevice.cpp
     PUBLIC willow/rendering/Vulkan/vkinit.cpp
     PUBLIC willow/rendering/Vulkan/vkswapchain.cpp
     PUBLIC ../include/willow/dev-utils/Vulkan/vkdevice.hpp
     PUBLIC ../include/willow/dev-utils/Vulkan/vkinit.hpp
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
        PUBLIC ../include/willow/rendering/Vulkan/vulkan_context.hpp
        PUBLIC ../include/willow/rendering/Vulkan/vulkan_frame_buffer_array.hpp
        PUBLIC ../include/willow/rendering/Vulkan/vulkan_graphics_pipeline.hpp
        PUBLIC ../include/willow/rendering/Vulkan/vulkan_index_buffer.hpp
        PUBLIC ../include/willow/rendering/Vulkan/vulkan_render_buffer.hpp
        PUBLIC ../include/willow/rendering/Vulkan/vulkan_render_core.hpp
        PUBLIC ../include/willow/rendering/Vulkan/vulkan_render_pass.hpp
        #PUBLIC ../include/willow/rendering/Vulkan/vulkan_render_task_manager.hpp
        PUBLIC ../include/willow/rendering/Vulkan/vulkan_shader.hpp
        PUBLIC ../include/willow/rendering/Vulkan/vulkan_swapchain.hpp
        PUBLIC ../include/willow/rendering/Vulkan/vulkan_uniform_buffer_array.hpp
        PUBLIC ../include/willow/rendering/Vulkan/vulkan_vertex_buffer.hpp
        PUBLIC ../include/willow/rendering/wilo_render_core.hpp
        PUBLIC ../include/willow/rendering/wilo_render_data_layout.hpp
        PUBLIC ../include/willow/rendering/wilo_renderer.hpp
        PUBLIC ../include/willow/rendering/wilo_rendering_API.hpp
        PUBLIC ../include/willow/rendering/wilo_shader.hpp
        PUBLIC ../include/willow/rendering/wilo_vertex_types.hpp
        PUBLIC willow/rendering/Vulkan/vulkan_context.cpp
        PUBLIC willow/rendering/Vulkan/vulkan_frame_buffer_array.cpp
        PUBLIC willow/rendering/Vulkan/vulkan_graphics_pipeline.cpp
        PUBLIC willow/rendering/Vulkan/vulkan_index_buffer.cpp
        PUBLIC willow/rendering/Vulkan/vulkan_render_core.cpp
        PUBLIC willow/rendering/Vulkan/vulkan_render_pass.cpp
        #PUBLICwillow /rendering/Vulkan/vulkan_render_task_manager.cpp
        PUBLIC willow/rendering/Vulkan/vulkan_shader.cpp
        PUBLIC willow/rendering/Vulkan/vulkan_swapchain.cpp
        PUBLIC willow/rendering/Vulkan/vulkan_uniform_buffer_array.cpp
        PUBLIC willow/rendering/Vulkan/vulkan_vertex_buffer.cpp
        PUBLIC willow/rendering/wilo_renderer.cpp
        PUBLIC willow/rendering/wilo_shader.cpp
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
target_sources(WillowScripting
        PUBLIC ../include/willow/scripting/Scriptable.hpp
        PUBLIC willow/scripting/Scriptable.cpp
        PUBLIC ../include/willow/scripting/LuaBasics.h
        willow/scripting/LuaBasics.cpp
        )


add_library(Willow
        ../include/willow/application.hpp
        ../include/willow/wilo_console.hpp
        ../include/willow.hpp
        willow/application.cpp
        willow/wilo_console.cpp
        )
target_link_libraries(Willow PUBLIC WillowWindow)
target_link_libraries(Willow PUBLIC WillowScripting)
target_link_libraries(Willow PUBLIC WillowRendering)
target_link_libraries(Willow PUBLIC WillowRoot)
target_link_libraries(Willow PUBLIC WillowDevUtils)
