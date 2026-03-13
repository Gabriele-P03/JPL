
/**
 * @file
 * 
 * This is the wrapper which provides an over-platform including system in order to include the right StacktraceWrapper for your machine.<br>
 * You oughta always include this one in order to prevent any errors about the wrong included wrapper.
 * 
 * Into each specific wrapper there is a queryStacktrace(unsigned int, unsigned int) which returns a pointer to vector of struct Frame - what is a 
 * struct Frame will be discussed later.<br>
 * The first parameter is the amount of frames that you would like to skip (removed from the vector); you should always pass DEFAULT_SKIP_FRAMES (read its comment to get more
 * information about what it does mean).<br>
 * The second parameter is the max amount of frames that you would like to retrieve.
 * <br>
 * @a Frame description:
 * @param bool frame_valid it indicates if the frame is valid (you should check jpl::_utils::_error::_GetLastError() to get known about the error that happened)
 * @param line number line of the called function into file_name
 * @param function_name name of the function
 * @param address address of the function   
 * @param file_name name of the file
 * 
 * All these parameters are set to 0 or empty string, if frame_valid is false 
 * <br>
 * About the -1 when it stores the line number: <p>
 * The stackframe is recognized via PC (Program Counter), that register is increased once the istruction is decoded - therefore before it is actually performed - 
 * then the line number retrieved is the one of the next istruction, then it is decreased.<br>
 * Although the decreasing, you could even encounter bugs about it, e.g. when there are some blank lines between two consecutive functions.<br>
 * You can get more information about it at {@link https://github.com/JochenKalmbach/StackWalker/issues/31}
 * 
 * Whenever a Stack symbol could not be solved, an Invalid Frame will be inserted into the list.
 * 
 * @author Gabriele-P03
 * @date 2023-08-03
 * @copyright Copyright (c) 2023
 */

#ifndef STACKTRACE_WRAPPER_JPL
#define STACKTRACE_WRAPPER_JPL

    /**
     * @brief it is the amount of called functions that you should always skip, it begins from the call to Stacktrace's constructor 'till queryStacktrace() function.<br>
     * I guess that, into a printed stacktrace, you do not care about these functions which had been called after the thrown exception
     * 
     * Windows case:
     * 
     * First call to skip: jpl::_utils::_debug::queryStacktrace()
     * Second call to skip: jpl::_utils::_debug::Stacktrace()
     * Third call to skip: the function which invoke the Stacktrace constructor
     * 
     * Linux case:
     * 
     * 
     * 
     * This field is used by default constructor, increased by 2 (check it to get more information).<br>
     * If you use it with the constructor with skipped solo parameter, increase it of 1, otherwise pass it as it is
     */
    #define DEFAULT_SKIP_STACKFRAMES_JPL 3

    #define MAX_STACKFRAMES_JPL 512


    #ifdef _WIN32
        #include <eh.h>
        #include <windows.h>
        #include <dbghelp.h>
    #elif __linux__
        #include <regex>
        #include <iostream>
        #include <execinfo.h>
    #endif

    #include <string>

    #ifdef AUTO_LOG_EXCEPTION_JPL
        #include <jpl/logger/Logger.hpp>
    #endif

    #ifdef _WIN32
        #include "../../ErrorUtils.hpp"
    #endif

    #include "../Frame.hpp"
    
    namespace jpl{
        namespace _utils{

            namespace _collections{
                namespace _list{
                    template<class T>
                    class LinkedList;
                }
            }
            
            namespace _debug{

                /**
                 * 
                 * @param skipped amount of frames to skip
                 * @param maxSize max amount of frames to retrieve
                 * @return std::vector<Frame>* vector containing all retrieved frames
                 */

                _collections::_list::LinkedList<jpl::_utils::_debug::Frame*>* queryStacktrace(unsigned int skipped, unsigned int maxSize);
            }
        }
    }

    #include "../../../structure/list/LinkedList.hpp"

#endif