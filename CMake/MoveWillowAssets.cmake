cmake_minimum_required(VERSION 3.16)


message("PASSED IN PROJECT ROOT  ${PROJECT_ROOT}")

message("Transferring Assets...")
function(MoveAssetFolder FolderName)
    set(InputDirectory ${PROJECT_ROOT}/Assets/${FolderName})
    set(OutputDirectory ${CMAKE_BINARY_DIR}/Assets/${FolderName})
    if(EXISTS ${InputDirectory})
        file(GLOB_RECURSE InputAssets "${InputDirectory}/*")
        file(COPY ${InputAssets} DESTINATION ${OutputDirectory})
        else()
            message("COULD NOT FIND ${InputDirectory} Skipping the copy")
        endif()
    endfunction()


MoveAssetFolder(Textures)
MoveAssetFolder(Scripts)
