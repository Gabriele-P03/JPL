cmake_minimum_required(VERSION 3.9)
message("")
message("---------------------------------------- PARSER ----------------------------------------")
message("")
set(PARSER_VERSION_JPL "1.0.0")
project(ParserJPL VERSION ${PARSER_VERSION_JPL})
message(TRACE "Compiling Parser v: " ${LOGGER_VERSION_JPL})

add_library( parserJPL_sh SHARED "src/bmp/BMP.cpp")
set_target_properties(parserJPL_sh PROPERTIES OUTPUT_NAME "parserJPL" SOVERSION ${PARSER_VERSION_JPL})
add_library( parserJPL_st STATIC "src/bmp/BMP.cpp")
set_target_properties(parserJPL_st PROPERTIES OUTPUT_NAME "parserJPL" SOVERSION ${PARSER_VERSION_JPL})

#target_link_libraries(parserJPL_sh libutilsJPL)

set(CMAKE_C_COMPILER gcc)
set(CMAKE_CXX_COMPILER g++)

set(CMAKE_CXX_STANDARD 23)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}")


include_directories(CPLUS_INCLUDE_PATH)
include_directories(C_INCLUDE_PATH)

option(BUILD_SHARED_LIBS "Build using shared libraries" ON)