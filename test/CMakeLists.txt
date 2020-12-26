 cmake_minimum_required(VERSION 3.16)


 add_executable(XylemMessageTest XylemMessageTest.cpp catch.hpp)

 target_link_libraries(XylemMessageTest PRIVATE Willow)
 target_include_directories(XylemMessageTest PRIVATE ../include)

 add_executable(ObserverMessageTest ObserverMessageTest.cpp catch.hpp)
 target_link_libraries(ObserverMessageTest PRIVATE Willow)
 target_include_directories(ObserverMessageTest PRIVATE ../include)

 add_executable(ConsoleCoreTest ConsoleCoreTest.cpp catch.hpp)
 target_link_libraries(ConsoleCoreTest PRIVATE Willow)
 target_include_directories(ConsoleCoreTest PRIVATE ../include)



 add_executable(GLFWWindowTest GLFWWindowTest.cpp)
 find_package(Vulkan REQUIRED )
 target_link_libraries(GLFWWindowTest PRIVATE glfw)
 target_link_libraries(GLFWWindowTest PUBLIC Vulkan::Vulkan)

 add_executable(RenderStressTest RenderStressTest.cpp)
 target_link_libraries(RenderStressTest PRIVATE Willow)

 add_executable(NewRendererBootStrapTest NewRendererBootStrap.cpp)
 target_link_libraries(NewRendererBootStrapTest PRIVATE Willow)

 add_executable(RenderDataTest RenderDataTest.cpp)
 target_link_libraries(RenderDataTest PRIVATE Willow)
