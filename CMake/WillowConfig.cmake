# WillowConfig.cmake - package configuration file
include(CMakeFindDependencyMacro)


find_dependency(glfw3)
list(APPEND CMAKE_PREFIX_PATH "/usr/local/share/spriv_cross_core")
find_dependency(spirv_cross_core)
list(APPEND CMAKE_PREFIX_PATH "/usr/local/share/spriv_cross_glsl")
find_dependency(spirv_cross_glsl)
list(APPEND CMAKE_PREFIX_PATH "/usr/local/share/spriv_cross_cpp")
find_dependency(spirv_cross_cpp)

find_dependency(Wua)

find_dependency(Vulkan)
find_dependency(glm)
get_filename_component(SELF_DIR "${CMAKE_CURRENT_LIST_FILE}" PATH)
include(${SELF_DIR}/Willow.cmake)

set(WILLOW_ASSETS ${SELF_DIR}/Willow)
set(PROJECT_ASSETS ${CMAKE_CURRENT_LIST_DIR}/Assets)
function(WillowBuildProjectStructure)
    make_directory(${CMAKE_BINARY_DIR}/Assets/Shaders)
    make_directory(${CMAKE_BINARY_DIR}/Assets/Textures)
    make_directory(${CMAKE_BINARY_DIR}/Assets/Scripts)
endfunction()

function(WillowCopyEngineAssets)
    file(GLOB_RECURSE WillowScripts "${WILLOW_ASSETS}/scripts/*")
    file(COPY ${WillowScripts} DESTINATION ${CMAKE_BINARY_DIR}/Assets/Scripts/)
    endfunction()



add_custom_target(Shaders
        COMMAND ${CMAKE_COMMAND} -DPROJECT_ROOT=${PROJECT_SOURCE_DIR} -P ${SELF_DIR}/BuildWillowShaders.cmake
)

add_custom_target(AssetMove
        COMMAND ${CMAKE_COMMAND} -DPROJECT_ROOT=${PROJECT_SOURCE_DIR} -P ${SELF_DIR}/MoveWillowAssets.cmake
        )

macro(WillowApplication APPLICATION)
    target_link_libraries(${APPLICATION} PRIVATE Willow)
    add_dependencies(${APPLICATION} AssetMove)
    add_dependencies(${APPLICATION} Shaders)
    WillowBuildProjectStructure()
    WillowCopyEngineAssets()
    endmacro()