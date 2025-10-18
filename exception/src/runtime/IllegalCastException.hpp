/**
 * @file
 * 
 * An IllegalCastException is thrown whenever an invalid cast on an object is attempted
 * 
 * @date 2023-04-22
 * @copyright Copyright (c) 2023
 * @author Gabriele-P03
 */
#ifndef ILLEGAL_CAST_EXCEPTION_JPL
#define ILLEGAL_CAST_EXCEPTION_JPL

#include "RuntimeException.hpp"


namespace jpl{

    namespace _exception{

        class IllegalCastException : public RuntimeException{

            private:

                /**
                 * The class which the field has been attempted to be cast to
                 */
                const char* _cast_attempted;
                /**
                 * The class supplied
                 */
                const char* _cast_needed;
            
            public:

                IllegalCastException(const char* _cast_attempted, const char* _cast_needed, std::string msg) : 
                    RuntimeException("IllegalCastException", msg), _cast_attempted(_cast_attempted), _cast_needed(_cast_needed){

                    }
                IllegalCastException(const char* _cast_attempted, const char* _cast_needed) : IllegalCastException(_cast_attempted, _cast_needed, ""){}
                IllegalCastException(std::string msg) : IllegalCastException("UNKNOWN" ,"UNKNOWN", msg){}
                IllegalCastException() : IllegalCastException(""){}

                inline const char* what() const _GLIBCXX_TXN_SAFE_DYN _GLIBCXX_NOTHROW override{
                    std::string buffer = 
                        std::string(this->type_ex) + " Attempted Cast: " + std::string(this->_cast_attempted) +
                                                     " Required Cast: " + std::string(this->_cast_needed) + 
                                                     " " + std::string(this->msg);

                    char* c_buffer = new char[buffer.size()];
                    memcpy(c_buffer, buffer.c_str(), buffer.size());
                    return c_buffer;
                }
        };
    }
}

#endif