cmake_minimum_required(VERSION 3.9)
message("")
message("---------------------------------------- PARSER ----------------------------------------")
message("")
set(PARSER_VERSION_JPL "1.0.0")
project(ParserJPL VERSION ${PARSER_VERSION_JPL})
message(TRACE "Compiling Parser v: " ${LOGGER_VERSION_JPL})

set(FIRST_CALL_ADD_LIBRARY_SH TRUE)
set(FIRST_CALL_ADD_LIBRARY_ST TRUE)

function (addSRC PATH_SRC NAME_INFO STATIC_ENABLE SHARED_ENABLE)
    if(${STATIC_ENABLE})
        if(${FIRST_CALL_ADD_LIBRARY_SH})
            
            add_library( parserJPL_sh SHARED ${PATH_SRC})
            set_target_properties(parserJPL_sh PROPERTIES OUTPUT_NAME "parserJPL" SOVERSION ${PARSER_VERSION_JPL})
            set(FIRST_CALL_ADD_LIBRARY_SH FALSE PARENT_SCOPE)#CACHE BOOL "First Call to Add Library for Shared done")  
        else()
            target_sources(parserJPL_sh PUBLIC ${PATH_SRC})    
        endif()
    endif()    

    if(${SHARED_ENABLE})
        if(${FIRST_CALL_ADD_LIBRARY_ST})
            add_library( parserJPL_st STATIC ${PATH_SRC})
            set_target_properties(parserJPL_st PROPERTIES OUTPUT_NAME "parserJPL")
            set(FIRST_CALL_ADD_LIBRARY_ST FALSE PARENT_SCOPE)#CACHE BOOL "First Call to Add Library for Static done")
        else()
            target_sources(parserJPL_st PUBLIC ${PATH_SRC})
        endif()
    endif()

    message("-- Adding " ${NAME_INFO} "... ")

endfunction()

addSRC("src/wav/WAV.cpp" "WAVE" TRUE TRUE)
addSRC("src/bmp/BMP.cpp" "BMP" TRUE TRUE)

set(CMAKE_C_COMPILER gcc)
set(CMAKE_CXX_COMPILER g++)

set(CMAKE_CXX_STANDARD 23)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}")


include_directories(CPLUS_INCLUDE_PATH)
include_directories(C_INCLUDE_PATH)

option(BUILD_SHARED_LIBS "Build using shared libraries" ON)