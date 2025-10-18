/**
 * @file
 * 
 * An IllegalAccessException is thrown whenever a non-public field is attempted to be get via reflexpr TS
 * or any other kind of reflection feature that will be implemented later in C++.
 * 
 * Since reflexpr TS is experimental this exception could seem as useless, indeed it is though for a later feature.
 * 
 * 
 * @date 2023-04-22
 * @copyright Copyright (c) 2023
 * @author Gabriele-P03
 */
#ifndef ILLEGAL_ACCESS_EXCEPTION_JPL
#define ILLEGAL_ACCESS_EXCEPTION_JPL

#include "../AbstractException.hpp"


namespace jpl{

    namespace _exception{

        class IllegalAccessException : public AbstractException{
            
            public:

                IllegalAccessException(std::string msg) : AbstractException("ArithmeticException", msg){

                }
                IllegalAccessException() : IllegalAccessException(""){}

        };
    }
}

#endif