/**
 * An IllegalAccessException is thrown whenever a non-public field is attempted to be get via reflexpr TS
 * or any other kind of reflection feature that will be implemented later in C++.
 * 
 * Since reflexpr TS is experimental this exception could seem as useless, indeed it is though for a later feature.
 * 
 * As explained in AbstractException's documentation, altough constructors are declared as public, you should avoid to call them.
 * It can be thrown by constructor with 1 parameter (as you can see by the macro defined at the end of this file)
 *
 * In this exception what() is overriden in order to print the field which is illegal.
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

            private:

                const char* _cause;
            
            public:

                IllegalAccessException(const char* const _cause, const char* msg, const char* file_name, const char* function_name, const int line) : 
                    AbstractException("IllegalAccessException", msg, file_name, function_name, line), _cause(_cause){}

                inline const char* what() const _GLIBCXX_TXN_SAFE_DYN _GLIBCXX_NOTHROW override{
                    std::string buffer = 
                        std::string(this->type_ex) + " thrown by " + std::string(this->function_name) + 
                                                     " at line "   + std::to_string(this->line) + 
                                                     " of "        + std::string(this->file_name) +
                                                     ": "   + std::string(this->_cause) + "\n";


                    char* c_buffer = new char[buffer.size()];
                    memcpy(c_buffer, buffer.c_str(), buffer.size());
                    return c_buffer;
                }
        };
    }
}


#define STRINGIFY(arg) #arg
/**
 * @brief object will be stringified
 * 
 * @param object the non-public field
 */
#define IllegalAccessException(object) jpl::_exception::IllegalAccessException(STRINGIFY(object), "",  __FILENAME__, __func__, __LINE__)


#endif