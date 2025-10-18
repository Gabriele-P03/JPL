/**
 * @file
 * 
 * A PermissionException is thrown whenever an error has been caused by a missing permission
 * 
 * @date 2023-05-20
 * @copyright Copyright (c) 2023
 * @author Gabriele-P03
 */

#ifndef PERMISSION_EXCEPTION_JPL
#define PERMISSION_EXCEPTION_JPL

#include "RuntimeException.hpp"

namespace jpl{

    namespace _exception{

        class PermissionException : public RuntimeException{

            public:
                PermissionException(std::string msg) : RuntimeException("RuntimeException", msg){

                }
                PermissionException() : PermissionException(""){}

        };
    }
}


#endif