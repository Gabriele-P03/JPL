cmake_minimum_required(VERSION 3.9)
message("")
message("---------------------------------------- NETWORK ----------------------------------------")
message("")

set(ALLOW_DUPLICATE_CUSTOM_TARGETS TRUE)
set(NETWORK_VERSION_JPL "1.0.0")
project(NETWORK VERSION ${NETWORK_VERSION_JPL})

set(CMAKE_C_COMPILER gcc)
set(CMAKE_CXX_COMPILER g++)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g")
message("Compiling Network v: " ${NETWORK_VERSION_JPL})
message("Compiling with Cxx v: " ${CMAKE_CXX_STANDARD})

set(FIRST_CALL_ADD_LIBRARY_SH TRUE)
set(FIRST_CALL_ADD_LIBRARY_ST TRUE)
set(STRING_NETWORK_CPP TRUE)

function (addSRC PATH_SRC NAME_INFO STATIC_ENABLE SHARED_ENABLE)
    if(${STATIC_ENABLE})
        if(${FIRST_CALL_ADD_LIBRARY_SH})
            
            add_library( networkJPL_sh SHARED ${PATH_SRC})
            set_target_properties(networkJPL_sh PROPERTIES OUTPUT_NAME "networkJPL" SOVERSION ${NETWORK_VERSION_JPL})
            set(FIRST_CALL_ADD_LIBRARY_SH FALSE PARENT_SCOPE)#CACHE BOOL "First Call to Add Library for Shared done")

            if(WIN32)
                message("Linking dbghelp powrprof pdh")
                target_link_libraries(networkJPL_sh dbghelp)   
                target_link_libraries(networkJPL_sh PowrProf)   
                target_link_libraries(networkJPL_sh pdh)  
            endif()
        else()
            target_sources(networkJPL_sh PUBLIC ${PATH_SRC})    
        endif()
    endif()    

    if(${SHARED_ENABLE})
        if(${FIRST_CALL_ADD_LIBRARY_ST})
            add_library( networkJPL_st STATIC ${PATH_SRC})
            set_target_properties(networkJPL_st PROPERTIES OUTPUT_NAME "networkJPL")
            set(FIRST_CALL_ADD_LIBRARY_ST FALSE PARENT_SCOPE)#CACHE BOOL "First Call to Add Library for Static done")
        else()
            target_sources(networkJPL_st PUBLIC ${PATH_SRC})
        endif()
    endif()

    message("-- Adding " ${NAME_INFO} "... ")

endfunction()

addSRC("src/tls/TLS.cpp" "TLS" TRUE TRUE)
addSRC("src/tls/TLSWrapper.cpp" "TLSWrapper" TRUE TRUE)
addSRC("src/socket/AbstractSocket.cpp" "AbstractSocket" TRUE TRUE)
addSRC("src/socket/ClientSocket.cpp" "ClientSocket" TRUE TRUE)
addSRC("src/socket/ServerSocket.cpp" "ServerSocket" TRUE TRUE)
addSRC("src/socket/clientmanager/ClientManager.cpp" "ClientManager" TRUE TRUE)
addSRC("src/socket/clientmanager/Client.cpp" "Client" TRUE TRUE)
addSRC("src/http/Response.cpp" "Response" TRUE TRUE)
addSRC("src/http/HttpClientAbstract.cpp" "HttpClientAbstract" TRUE TRUE)
addSRC("src/http/HttpClient.cpp" "HttpClient" TRUE TRUE)

target_link_libraries(networkJPL_sh curl)
target_link_libraries(networkJPL_sh ssl)
target_link_libraries(networkJPL_sh crypto)
target_link_libraries(networkJPL_sh utilsJPL)
target_link_libraries(networkJPL_sh loggerJPL)
if(WIN32)
    target_link_libraries(networkJPL_sh ws2_32)   
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
