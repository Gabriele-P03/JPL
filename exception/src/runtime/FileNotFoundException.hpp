/**
 * @file
 * 
 * A FileNotFoundException is thrown whenever a file has not been found.
 * 
 * @date 2023-04-22
 * @copyright Copyright (c) 2023
 * @author Gabriele-P03
 */
#ifndef FILE_NOT_FOUND_EXCEPTION_JPL
#define FILE_NOT_FOUND_EXCEPTION_JPL

#include "RuntimeException.hpp"


namespace jpl{

    namespace _exception{

        class FileNotFoundException : public RuntimeException{

            
            public:

                FileNotFoundException(std::string msg) : RuntimeException("FileNotFoundException", msg){

                }
                FileNotFoundException() : FileNotFoundException(""){}
        };
    }
}

#endif