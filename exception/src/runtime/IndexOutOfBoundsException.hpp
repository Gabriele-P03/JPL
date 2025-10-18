/**
 * @file
 * 
 * An IndexOutOfBounds exception is thrown whenever getting an element whose index is out-of-range of a list is attempted.
 * It throws an IllegalArgumentException if Max is greater than Attempted
 * 
 * 
 * @date 2023-04-23
 * @copyright Copyright (c) 2023
 * @author Gabriele-P03
 */

#ifndef INDEX_OUT_OF_BOUNDS_EXCEPTION_JPL
#define INDEX_OUT_OF_BOUNDS_EXCEPTION_JPL

#include "IllegalArgumentException.hpp"

namespace jpl{

    namespace _exception{

        class IndexOutOfBoundsException : public RuntimeException{

            private:
                /**
                 * the list which the operation had been performed on
                 */
                const char* _object;
                /**
                 * size of list
                 */
                const unsigned int max;
                /**
                 * the index attempted
                 */
                const unsigned int attempted;


            public:
                IndexOutOfBoundsException(const unsigned int max, const unsigned int attempted, std::string msg) : 
                    RuntimeException("IndexOutOfBoundsException", msg), max(max), attempted(attempted) {

                    }
                IndexOutOfBoundsException(const unsigned int max, const unsigned int attempted) :
                    IndexOutOfBoundsException(max, attempted, ""){}

                inline const char* what() const _GLIBCXX_TXN_SAFE_DYN _GLIBCXX_NOTHROW override{

                    std::string buffer =    std::string(this->type_ex) + 
                                            " Max: " + std::to_string(this->max) + 
                                            " Attempted: " + std::to_string(this->attempted) + 
                                            ". " + std::string(this->msg);

                    char* c_buffer = new char[buffer.size()];
                    strcpy(c_buffer, buffer.c_str());
                    return c_buffer;
                }
        };
    }
}

#endif