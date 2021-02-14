cmake_minimum_required(VERSION 3.19)

include(ExternalProject)
include(FetchContent)

find_package(Vulkan REQUIRED)

FetchContent_Declare(glm_remote
        GIT_REPOSITORY https://github.com/g-truc/glm.git
        GIT_TAG bf71a834948186f4097caa076cd2663c69a10e1e
        GIT_PROGRESS true
        )

FetchContent_Declare(spdlog_remote
        GIT_REPOSITORY https://github.com/gabime/spdlog.git 
        GIT_TAG de0dbfa3596a18cd70a4619b6a9766847a941276
        GIT_PROGRESS true
        )
FetchContent_Declare(glfw_remote
        GIT_REPOSITORY  https://github.com/glfw/glfw.git
        GIT_TAG 0a49ef0a00baa3ab520ddc452f0e3b1e099c5589
        GIT_PROGRESS true
        )
FetchContent_Declare(spirvCross_remote
        GIT_REPOSITORY https://github.com/KhronosGroup/SPIRV-Cross.git 
        GIT_TAG 9acb9ec31f5a8ef80ea6b994bb77be787b08d3d1
        GIT_PROGRESS true
        )

FetchContent_MakeAvailable(glm_remote spdlog_remote glfw_remote spirvCross_remote)

ExternalProject_Add(lua_remote
   URL "https://www.lua.org/ftp/lua-5.4.2.tar.gz"
   CONFIGURE_COMMAND ""
   BUILD_COMMAND make generic
   BUILD_ALWAYS true
   BUILD_IN_SOURCE true
   INSTALL_COMMAND ""
)
add_dependencies(Lua lua_remote PARENT_SCOPE)
ExternalProject_Get_property(lua_remote SOURCE_DIR)
message("liblua will be found at \'${SOURCE_DIR}/src\'")
set_property(TARGET Lua PROPERTY
             IMPORTED_LOCATION "${SOURCE_DIR}/src/liblua.a")
message(Lua)
target_include_directories(Lua INTERFACE "${SOURCE_DIR}/src")








