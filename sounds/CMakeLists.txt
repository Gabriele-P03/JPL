cmake_minimum_required(VERSION 3.9)
message("")
message("---------------------------------------- SOUNDS ----------------------------------------")
message("")

find_package(OpenAl REQUIRED)

set(ALLOW_DUPLICATE_CUSTOM_TARGETS TRUE)
set(SOUNDS_VERSION_JPL "1.0.0")
project(SOUNDS VERSION ${SOUNDS_VERSION_JPL})

set(CMAKE_C_COMPILER gcc)
set(CMAKE_CXX_COMPILER g++)
set(CMAKE_CXX_STANDARD 23)
message("Compiling Sounds v: " ${SOUNDS_VERSION_JPL})
message("Compiling with Cxx v: " ${CMAKE_CXX_STANDARD})

set(FIRST_CALL_ADD_LIBRARY_SH TRUE)
set(FIRST_CALL_ADD_LIBRARY_ST TRUE)

add_compile_definitions(STB_IMAGE_IMPLEMENTATION)

#Logger's MACRO part
if( ${QUIET_LOGGER_JPL} AND ${CUSTOM_LOGGER_JPL})
    message(FATAL_ERROR "You are trying to build Custom mode and Quiet one together, which is a useless practice since QUIET macro is used only if CUSTOM is not defined")
else()

    add_compile_definitions(USE_LOGGER_JPL) #Standard macro

    if(${QUIET_LOGGER_JPL})
        add_compile_definitions(QUIET_LOGGER_JPL)
        message(STATUS " MACRO: Compiling with QUIET MODE")
    elseif(${CUSTOM_LOGGER_JPL})
        add_compile_definitions(CUSTOM_LOGGER_JPL)
        message(STATUS " MACRO: Compiling with QUIET MODE")
    endif()

    if(${UFW_LOGGER_JPL})
        add_compile_definitions(UFW_LOGGER_JPL)
        message(STATUS " MACRO: Compiling with UFW MODE")
    endif()

    if(${USE_STACKTRACE_W_EXCEPTION_JPL})
        add_compile_definitions(USE_STACKTRACE_W_EXCEPTION_JPL)
        message(STATUS " MACRO: Exception with Stacktrace have been enabled")
        if(${AUTO_LOG_EXCEPTION_JPL})
            add_compile_definitions(AUTO_LOG_EXCEPTION_JPL)
            message(STATUS " MACRO: Stacktrace auto-logged after exception has been enabled")
        endif()
    endif()
endif()

function (addSRC PATH_SRC NAME_INFO STATIC_ENABLE SHARED_ENABLE)
    if(${STATIC_ENABLE})
        if(${FIRST_CALL_ADD_LIBRARY_SH})
            
            add_library( soundsJPL_sh SHARED ${PATH_SRC})
            set_target_properties(soundsJPL_sh PROPERTIES OUTPUT_NAME "soundsJPL" SOVERSION ${SOUNDS_VERSION_JPL})
            set(FIRST_CALL_ADD_LIBRARY_SH FALSE PARENT_SCOPE)#CACHE BOOL "First Call to Add Library for Shared done")  
        else()
            target_sources(soundsJPL_sh PUBLIC ${PATH_SRC})    
        endif()
    endif()    

    if(${SHARED_ENABLE})
        if(${FIRST_CALL_ADD_LIBRARY_ST})
            add_library( soundsJPL_st STATIC ${PATH_SRC})
            set_target_properties(soundsJPL_st PROPERTIES OUTPUT_NAME "soundsJPL")
            set(FIRST_CALL_ADD_LIBRARY_ST FALSE PARENT_SCOPE)#CACHE BOOL "First Call to Add Library for Static done")
        else()
            target_sources(soundsJPL_st PUBLIC ${PATH_SRC})
        endif()
    endif()

    message("-- Adding " ${NAME_INFO} "... ")

endfunction()

addSRC("src/BufferManager.cpp" "BufferManager" TRUE TRUE)
addSRC("src/ContextManager.cpp" "BufferManager" TRUE TRUE)
addSRC("src/Utils.cpp" "BufferManager" TRUE TRUE)
addSRC("src/SourceBufferWrapper.cpp" "BufferManager" TRUE TRUE)

set(CMAKE_C_COMPILER gcc)
set(CMAKE_CXX_COMPILER g++)

set(CMAKE_CXX_STANDARD 23)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}")

include_directories(CPLUS_INCLUDE_PATH)
include_directories(C_INCLUDE_PATH)

target_link_libraries(soundsJPL_sh OpenAl)

option(BUILD_SHARED_LIBS "Build using shared libraries" ON)
