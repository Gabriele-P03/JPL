/**
 * @file
 * 
 * A NotFoundException is thrown whenever a field has not been found into a list of objects.
 * 
 * This exception is a bit Runtime. There are some extended version of it  
 * 
 * @date 2023-04-22
 * @copyright Copyright (c) 2023
 * @author Gabriele-P03
 */
#ifndef NOT_FOUND_EXCEPTION_JPL
#define NOT_FOUND_EXCEPTION_JPL

#include "RuntimeException.hpp"

namespace jpl{

    namespace _exception{

        class NotFoundException : public RuntimeException{
            
            public:

                NotFoundException(std::string msg) : RuntimeException("NotFoundException", msg){

                }
                NotFoundException() : NotFoundException(""){}

        };
    }
}

#endif