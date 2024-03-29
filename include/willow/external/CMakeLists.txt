cmake_minimum_required(VERSION 3.16)

include(ExternalProject)
include(FetchContent)

include(GNUInstallDirs)

find_package(Vulkan REQUIRED)
Set(FETCHCONTENT_QUIET FALSE)
#Declare all external dependnecies, pull them from github if they aren't already 



FetchContent_Declare(spdlog_remote
        GIT_REPOSITORY https://github.com/gabime/spdlog.git 
        GIT_TAG de0dbfa3596a18cd70a4619b6a9766847a941276
        GIT_PROGRESS TRUE
        #SOURCE_DIR ${CMAKE_CURRENT_LIST_DIR}/spdlog
        )
FetchContent_Declare(glfw_remote
        GIT_REPOSITORY  https://github.com/glfw/glfw.git
#        GIT_TAG 0a49ef0a00baa3ab520ddc452f0e3b1e099c5589
        GIT_PROGRESS TRUE
#        SOURCE_DIR ${CMAKE_CURRENT_LIST_DIR}/glfw
        )
FetchContent_Declare(spirvCross_remote
        GIT_REPOSITORY https://github.com/KhronosGroup/SPIRV-Cross.git 
        GIT_TAG 9acb9ec31f5a8ef80ea6b994bb77be787b08d3d1
        GIT_PROGRESS TRUE
#        SOURCE_DIR ${CMAKE_CURRENT_LIST_DIR}/spirv-Cross
        )

FetchContent_MakeAvailable(spdlog_remote glfw_remote spirvCross_remote)
#get lua 5.4, build 

#ExternalProject_Add(lua_build
#      SOURCE_DIR ${CMAKE_CURRENT_LIST_DIR}/lua-5.4.2
#   CONFIGURE_COMMAND ""
#   BUILD_COMMAND nmake
#   BUILD_ALWAYS true
#   BINARY_DIR ${CMAKE_CURRENT_LIST_DIR}/lua-5.4.2/src
#   INSTALL_COMMAND "" 
#)

add_library(lua STATIC IMPORTED) 
add_dependencies(lua lua_build)
set_property(TARGET lua PROPERTY IMPORTED_LOCATION  ${CMAKE_CURRENT_LIST_DIR}/lua-5.4.2/src/liblua.a)
target_include_directories(lua INTERFACE ${CMAKE_CURRENT_LIST_DIR}/lua-5.4.2/src)

add_library(glm::glm INTERFACE IMPORTED)
target_include_directories(glm::glm INTERFACE 
        $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/willow/external>
)




