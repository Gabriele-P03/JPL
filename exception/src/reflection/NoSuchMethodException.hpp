/**
 * @file 
 * 
 * A NoSuchMethodException is thrown whenever a method which had been attempted to be get via reflexpr TS
 * or any other kind of reflection feature that will be implemented later in C++, has not been found.
 * 
 * Since reflexpr TS is experimental this exception could seem as useless, indeed it is though for a later feature.
 * 
 * 
 * @date 2023-04-22
 * @copyright Copyright (c) 2023
 * @author Gabriele-P03
 */
#ifndef NO_SUCH_METHOD_EXCEPTION_JPL
#define NO_SUCH_METHOD_EXCEPTION_JPL

#include "../AbstractException.hpp"


namespace jpl{

    namespace _exception{

        class NoSuchMethodException : public AbstractException{
            
            public:

                NoSuchMethodException(std::string msg) : AbstractException("ArithmeticException", msg){

                }
                NoSuchMethodException() : NoSuchMethodException(""){}
        };
    }
}

#endif