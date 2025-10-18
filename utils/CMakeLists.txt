cmake_minimum_required(VERSION 3.9)
message("")
message("---------------------------------------- UTILS ----------------------------------------")
message("")

set(ALLOW_DUPLICATE_CUSTOM_TARGETS TRUE)
set(UTILS_VERSION_JPL "1.0.0")
project(UTILS VERSION ${UTILS_VERSION_JPL})

set(CMAKE_C_COMPILER gcc)
set(CMAKE_CXX_COMPILER g++)
set(CMAKE_CXX_STANDARD 23)
message("Compiling Utils v: " ${UTILS_VERSION_JPL})
message("Compiling with Cxx v: " ${CMAKE_CXX_STANDARD})

set(FIRST_CALL_ADD_LIBRARY_SH TRUE)
set(FIRST_CALL_ADD_LIBRARY_ST TRUE)
set(STRING_UTILS_CPP TRUE)

function (addSRC PATH_SRC NAME_INFO STATIC_ENABLE SHARED_ENABLE)
    if(${STATIC_ENABLE})
        if(${FIRST_CALL_ADD_LIBRARY_SH})
            
            add_library( utilsJPL_sh SHARED ${PATH_SRC})
            set_target_properties(utilsJPL_sh PROPERTIES OUTPUT_NAME "utilsJPL" SOVERSION ${UTILS_VERSION_JPL})
            set(FIRST_CALL_ADD_LIBRARY_SH FALSE PARENT_SCOPE)#CACHE BOOL "First Call to Add Library for Shared done")
            target_link_libraries(utilsJPL_sh dbghelp)   
            target_link_libraries(utilsJPL_sh PowrProf)   
            target_link_libraries(utilsJPL_sh pdh)    
        else()
            target_sources(utilsJPL_sh PUBLIC ${PATH_SRC})    
        endif()
    endif()    

    if(${SHARED_ENABLE})
        if(${FIRST_CALL_ADD_LIBRARY_ST})
            add_library( utilsJPL_st STATIC ${PATH_SRC})
            set_target_properties(utilsJPL_st PROPERTIES OUTPUT_NAME "utilsJPL")
            set(FIRST_CALL_ADD_LIBRARY_ST FALSE PARENT_SCOPE)#CACHE BOOL "First Call to Add Library for Static done")
        else()
            target_sources(utilsJPL_st PUBLIC ${PATH_SRC})
        endif()
    endif()

    message("-- Adding " ${NAME_INFO} "... ")

endfunction()

addSRC("src/debug/DebugUtils.cpp" "DebugUtils" TRUE TRUE)

addSRC("src/debug/stacktrace/wrapper/StacktraceWrapper.cpp" "StacktraceWrapper" TRUE TRUE)
addSRC("src/debug/stacktrace/Stacktrace.cpp" "Stacktrace" TRUE TRUE)

#Utils's MACRO part
addSRC("src/debug/Profiler.cpp" "Profiler" TRUE TRUE)
addSRC("src/StringUtils.cpp" "StringUtils" TRUE TRUE)
#Adding DB structure cpp
addSRC("src/structure/db/Table.cpp" "Table" TRUE TRUE)
addSRC("src/structure/db/TableWrapper.cpp" "Table" TRUE TRUE)
addSRC("src/structure/db/columns/Column.cpp" "Table" TRUE TRUE)

set(CMAKE_C_COMPILER gcc)
set(CMAKE_CXX_COMPILER g++)

set(CMAKE_CXX_STANDARD 23)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}")


include_directories(CPLUS_INCLUDE_PATH)
include_directories(C_INCLUDE_PATH)

option(BUILD_SHARED_LIBS "Build using shared libraries" ON)
