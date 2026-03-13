cmake_minimum_required(VERSION 3.9)
message("")
message("---------------------------------------- GRAPHICS ----------------------------------------")
message("")

find_package(GLEW REQUIRED)
find_package(OpenGL REQUIRED)

set(ALLOW_DUPLICATE_CUSTOM_TARGETS TRUE)
set(GRAPHICS_VERSION_JPL "1.0.0")
project(GRAPHICS VERSION ${GRAPHICS_VERSION_JPL})

set(CMAKE_C_COMPILER gcc)
set(CMAKE_CXX_COMPILER g++)
set(CMAKE_CXX_STANDARD 23)
message("Compiling Graphics v: " ${GRAPHICS_VERSION_JPL})
message("Compiling with Cxx v: " ${CMAKE_CXX_STANDARD})

set(FIRST_CALL_ADD_LIBRARY_SH TRUE)
set(FIRST_CALL_ADD_LIBRARY_ST TRUE)

add_compile_definitions(STB_IMAGE_IMPLEMENTATION)

function (addSRC PATH_SRC NAME_INFO STATIC_ENABLE SHARED_ENABLE)
    if(${STATIC_ENABLE})
        if(${FIRST_CALL_ADD_LIBRARY_SH})
            
            add_library( graphicsJPL_sh SHARED ${PATH_SRC})
            set_target_properties(graphicsJPL_sh PROPERTIES OUTPUT_NAME "graphicsJPL" SOVERSION ${GRAPHICS_VERSION_JPL})
            set(FIRST_CALL_ADD_LIBRARY_SH FALSE PARENT_SCOPE)#CACHE BOOL "First Call to Add Library for Shared done")
            target_link_libraries(graphicsJPL_sh dbghelp)   
            target_link_libraries(graphicsJPL_sh PowrProf)   
            target_link_libraries(graphicsJPL_sh pdh)    
        else()
            target_sources(graphicsJPL_sh PUBLIC ${PATH_SRC})    
        endif()
    endif()    

    if(${SHARED_ENABLE})
        if(${FIRST_CALL_ADD_LIBRARY_ST})
            add_library( graphicsJPL_st STATIC ${PATH_SRC})
            set_target_properties(graphicsJPL_st PROPERTIES OUTPUT_NAME "graphicsJPL")
            set(FIRST_CALL_ADD_LIBRARY_ST FALSE PARENT_SCOPE)#CACHE BOOL "First Call to Add Library for Static done")
        else()
            target_sources(graphicsJPL_st PUBLIC ${PATH_SRC})
        endif()
    endif()

    message("-- Adding " ${NAME_INFO} "... ")

endfunction()

addSRC("src/Metrics.cpp" "Metrics" TRUE TRUE)
addSRC("src/utils/Window.cpp" "Metrics" TRUE TRUE)
addSRC("src/shaders/Shader.cpp" "Metrics" TRUE TRUE)
addSRC("src/shaders/ProgramShaders.cpp" "Metrics" TRUE TRUE)
addSRC("src/shaders/ProgramManager.cpp" "Metrics" TRUE TRUE)
addSRC("src/mesh/Mesh.cpp" "Metrics" TRUE TRUE)
addSRC("src/mesh/MeshLoader.cpp" "Metrics" TRUE TRUE)
addSRC("src/input/Keyboard.cpp" "Metrics" TRUE TRUE)
addSRC("src/input/Mouse.cpp" "Metrics" TRUE TRUE)
addSRC("src/engine/Painter.cpp" "Metrics" TRUE TRUE)
addSRC("src/engine/button/Button.cpp" "Metrics" TRUE TRUE)
addSRC("src/engine/camera/BaseCamera.cpp" "Metrics" TRUE TRUE)
addSRC("src/engine/camera/OrthoCamera.cpp" "Metrics" TRUE TRUE)
addSRC("src/engine/camera/PerspectiveCamera.cpp" "Metrics" TRUE TRUE)
addSRC("src/engine/text/Font.cpp" "Metrics" TRUE TRUE)
addSRC("src/engine/text/TextRender.cpp" "Metrics" TRUE TRUE)
addSRC("src/engine/progress/ProgressBar.cpp" "Metrics" TRUE TRUE)
addSRC("src/engine/VAO.cpp" "Metrics" TRUE TRUE)
addSRC("src/engine/VAOManager.cpp" "Metrics" TRUE TRUE)

set(CMAKE_C_COMPILER gcc)
set(CMAKE_CXX_COMPILER g++)

set(CMAKE_CXX_STANDARD 23)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}")


include_directories(CPLUS_INCLUDE_PATH)
include_directories(C_INCLUDE_PATH)

target_link_libraries(graphicsJPL_sh libglfw3)
target_link_libraries(graphicsJPL_sh libglew32)
target_link_libraries(graphicsJPL_sh Opengl32)
target_link_libraries(graphicsJPL_sh libparserJPL)

option(BUILD_SHARED_LIBS "Build using shared libraries" ON)

add_executable(GRAPHICS${SOURCE_FILES})