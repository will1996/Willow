 cmake_minimum_required(VERSION 3.16)


macro(addTest testName sourceFile engineComponent)
 add_executable(${testName} ${sourceFile})
 target_link_libraries(${testName} PRIVATE ${engineComponent})
 add_test(${testName} ${CMAKE_BINARY_DIR}/${testName} )
endmacro()

 add_executable(XylemMessageTest XylemMessageTest.cpp catch.hpp)

 target_link_libraries(XylemMessageTest PRIVATE Willow)
 target_include_directories(XylemMessageTest PRIVATE ../include)

 add_executable(ObserverMessageTest ObserverMessageTest.cpp catch.hpp)
 target_link_libraries(ObserverMessageTest PRIVATE Willow)
 target_include_directories(ObserverMessageTest PRIVATE ../include)

 add_executable(ConsoleCoreTest ConsoleCoreTest.cpp catch.hpp ../include/willow/DefaultAssets.hpp)
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
 add_test(RenderData ${CMAKE_BINARY_DIR}/RenderDataTest)

 add_executable(FileSystemTest FileSystemTest.cpp)
 target_link_libraries(FileSystemTest PRIVATE Willow)

 add_executable(Tag TagTest.cpp)
 target_link_libraries(Tag PRIVATE WillowRoot)
 add_test(TestTag ${CMAKE_BINARY_DIR}/Tag )

 add_executable(Scene SceneTest.cpp)
 target_link_libraries(Scene PRIVATE Willow)
 add_test(TestScene ${CMAKE_BINARY_DIR}/Scene )

 add_executable(Messenger MessengerTest.cpp)
 target_link_libraries(Messenger PRIVATE WillowRendering)
 add_test(Messenger ${CMAKE_BINARY_DIR}/Messenger )

 add_executable(InputManager InputManagerTest.cpp)
 target_link_libraries(InputManager PRIVATE WillowRendering)
 add_test(InputManager ${CMAKE_BINARY_DIR}/InputManager )


 addTest(LuaDataTest LuaDataTest.cpp WillowRoot)
 addTest(DataVaueTest DataValueTest.cpp WillowRoot)
 addTest(DataTypeTest DataTypeTest.cpp WillowRoot)
