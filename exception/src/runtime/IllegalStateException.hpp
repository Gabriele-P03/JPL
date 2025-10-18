/**
 * @file
 * 
 * An IllegalStateException is thrown whenever any function is called at a bad time, 
 * i.e. wheter an another function had to be called before the one which threw this exception
 * 
 * e.g.
 *  
 *  The foo() function must be called after have set flag as true, then:
 *  
 *  void foo(){
 *      if(!flag){
 *          throw new IllegalStateException()
 *      }
 *  }
 * 
 * @date 2023-04-22
 * @copyright Copyright (c) 2023
 * @author Gabriele-P03
 */

#ifndef ILLEGAL_STATE_EXCEPTION_JPL
#define ILLEGAL_STATE_EXCEPTION_JPL

#include "RuntimeException.hpp"

namespace jpl{

    namespace _exception{
        
        class IllegalStateException : public RuntimeException{

            public:
                IllegalStateException(std::string msg) : RuntimeException("IllegalStateException", msg){

                }
                IllegalStateException() : IllegalStateException(""){}
        };
    }
}



#endif