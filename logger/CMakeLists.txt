cmake_minimum_required(VERSION 3.9)
message("")
message("---------------------------------------- LOGGER ----------------------------------------")
message("")
set(LOGGER_VERSION_JPL "1.0.0")
project(LoggerJPL VERSION ${LOGGER_VERSION_JPL})
message(TRACE "Compiling Logger v: " ${LOGGER_VERSION_JPL})

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

add_library( loggerJPL_sh SHARED "src/Logger.cpp")
set_target_properties(loggerJPL_sh PROPERTIES OUTPUT_NAME "loggerJPL" SOVERSION ${LOGGER_VERSION_JPL})
add_library( loggerJPL_st STATIC "src/Logger.cpp")
set_target_properties(loggerJPL_st PROPERTIES OUTPUT_NAME "loggerJPL" SOVERSION ${LOGGER_VERSION_JPL})

if(${AUTO_LOG_EXCEPTION_JPL})
    message(STATUS " LoggerWrapper.cpp added")
    target_sources(loggerJPL_sh PUBLIC "src/LoggerWrapper.cpp")  
    target_sources(loggerJPL_st PUBLIC "src/LoggerWrapper.cpp")  
endif()

target_link_libraries(loggerJPL_sh libutilsJPL)

set(CMAKE_C_COMPILER gcc)
set(CMAKE_CXX_COMPILER g++)

set(CMAKE_CXX_STANDARD 23)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}")


include_directories(CPLUS_INCLUDE_PATH)
include_directories(C_INCLUDE_PATH)

option(BUILD_SHARED_LIBS "Build using shared libraries" ON)