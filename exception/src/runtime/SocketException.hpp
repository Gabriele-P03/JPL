/**
 * @file
 * 
 * A SocketException is thrown whenever a socket error is occurred in runtime.
 * 
 * You may even call the constructor with the FD of the socket and let JPL retrieve the last error code and its meaning via
 * _utils:_GetLastError() and _utils::_GetLastErrorAsString 
 *
 * 
 * @date 2023-05-27
 * @copyright Copyright (c) 2023
 * @author Gabriele-P03
 */
#ifndef SOCKET_EXCEPTION_HPP
#define SOCKET_EXCEPTION_HPP

#include "RuntimeException.hpp"
#include <jpl/utils/debug/ErrorUtils.hpp>

namespace jpl{

    namespace _exception{

        class SocketException : public RuntimeException{

            protected:

                /**
                 * The occurred error's code
                 */
                const unsigned int error_code;
                /**
                 * The file descriptor of the socket which has caused this exception
                 */
                const unsigned int fd;

            public:
                SocketException(const unsigned int fd, const unsigned int error_code, std::string msg) : RuntimeException("SocketException", msg), error_code(error_code), fd(fd){

                }
                SocketException(const unsigned int fd, const unsigned int error_code) : SocketException(fd, error_code, ""){
                    std::string msg_s = jpl::_utils::_error::_GetLastErrorAsString(this->error_code); 
                    this->msg = new char[msg_s.size()];
                    memcpy((char*)this->msg, msg_s.c_str(), msg_s.size());
                }
                SocketException(const unsigned int fd) : SocketException(fd, jpl::_utils::_error::_GetLastError()){}
       
                inline virtual const char* what() const _GLIBCXX_TXN_SAFE_DYN _GLIBCXX_NOTHROW override{
                    std::string buffer = 
                        std::string(this->type_ex) + ": FD("       + std::to_string(this->fd) + 
                                                     ") Code("     + std::to_string(this->error_code) + 
                                                     ") -> " + std::string(this->msg);


                    char* c_buffer = new char[buffer.size()];
                    strcpy(c_buffer, buffer.c_str());
                    return c_buffer;
                }

        };
    }
}

#endif