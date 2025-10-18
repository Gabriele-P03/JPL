/**
 * @file
 * 
 * An OutOfMemory exception can be thrown when a program has not enough memory available in order to instance
 * new objects
 * 
 * @date 2023-04-23
 * @copyright Copyright (c) 2023
 * @author Gabriele-P03
 */

#ifndef OUTOFMEMORY_EXCEPTION_JPL
#define OUTOFMEMORY_EXCEPTION_JPL

#include "RuntimeException.hpp"

namespace jpl{

    namespace _exception{

        class OutOfMemoryException : public RuntimeException{

            public:
                OutOfMemoryException(std::string msg) : RuntimeException("OutOfMemoryException", msg){

                }
                OutOfMemoryException() : OutOfMemoryException(""){}

        };
    }
}


#endif