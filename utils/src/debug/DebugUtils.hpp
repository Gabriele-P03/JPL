/**
 * In this header file there is only a function which can be used to check if a debugger is 
 * attached to the program
 * 
 * @file
 *
 * @date 2023-05-26
 * @copyright Copyright (c) 2023
 * @author Gabriele-P03
 */

#ifndef DEBUG_UTILS_JPL
#define DEBUG_UTILS_JPL

    #include "ErrorUtils.hpp"//Used to catch errors during reading status
    #include <jpl/exception/runtime/RuntimeException.hpp>

    #ifdef _WIN32
        #include <debugapi.h>
    #endif
    #ifdef __linux__
        #include <fcntl.h>
        #include <unistd.h>
    #endif

    namespace jpl{
        namespace _utils{
            namespace _debug{

                extern bool isDebugging();
            }
        }
    }

#endif