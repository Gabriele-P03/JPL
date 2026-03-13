cmake_minimum_required(VERSION 3.9)
message("")
message("---------------------------------------- LOGGER ----------------------------------------")
message("")
set(LOGGER_VERSION_JPL "1.0.0")
project(LoggerJPL VERSION ${LOGGER_VERSION_JPL})
message(TRACE "Compiling Logger v: " ${LOGGER_VERSION_JPL})

add_library( loggerJPL_sh SHARED "src/Logger.cpp")
set_target_properties(loggerJPL_sh PROPERTIES OUTPUT_NAME "loggerJPL" SOVERSION ${LOGGER_VERSION_JPL})
add_library( loggerJPL_st STATIC "src/Logger.cpp")
set_target_properties(loggerJPL_st PROPERTIES OUTPUT_NAME "loggerJPL" SOVERSION ${LOGGER_VERSION_JPL})

target_link_libraries(loggerJPL_sh utilsJPL)
if(WIN32)
    message("Linking dbghelp powrprof pdh")
    target_link_libraries(loggerJPL_sh dbghelp)   
    target_link_libraries(loggerJPL_sh PowrProf)   
    target_link_libraries(loggerJPL_sh pdh)  
endif()

set(CMAKE_C_COMPILER gcc)
set(CMAKE_CXX_COMPILER g++)

set(CMAKE_CXX_STANDARD 23)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}")


include_directories(CPLUS_INCLUDE_PATH)
include_directories(C_INCLUDE_PATH)

option(BUILD_SHARED_LIBS "Build using shared libraries" ON)