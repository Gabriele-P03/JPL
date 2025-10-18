/**
 * @file
 * 
 * An IllegalArgumentException is thrown whenever a wrong argument is passed to 
 * any type of function (method, constructor...) 
 * 
 * @date 2023-04-22
 * @copyright Copyright (c) 2023
 * @author Gabriele-P03
 */
#ifndef ILLEGAL_ARGUMENT_EXCEPTION_JPL
#define ILLEGAL_ARGUMENT_EXCEPTION_JPL

#include "RuntimeException.hpp"


namespace jpl{

    namespace _exception{

        class IllegalArgumentException : public RuntimeException{

            public:

                IllegalArgumentException(std::string msg) : RuntimeException("IllegalArgumentException", msg){

                }
                IllegalArgumentException() : IllegalArgumentException(""){}
        };
    }
}

#endif