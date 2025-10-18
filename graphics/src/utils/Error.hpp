/**
 * A header file where there are some utility functions about error handling
 */

#ifndef ERRORUTILS_GRAPHICS_JPL
#define ERRORUTILS_GRAPHICS_JPL

#include <GLFW/glfw3.h>
#include <jpl/exception/runtime/RuntimeException.hpp>

namespace jpl{
    namespace _graphics{
        namespace _error{

                struct GLFWErrorJPL{
                    const unsigned int errorCode;
                    const char* msg;
                    GLFWErrorJPL(const unsigned int errorCode, const char* msg) : errorCode(errorCode), msg(msg){}
                    ~GLFWErrorJPL(){
                        //delete this->msg;
                    }
                };

                inline GLFWErrorJPL* getLastError(){
                    const char* msg;
                    int errorCode = glfwGetError(&msg);
                    if(errorCode == 0){
                        msg = "OK";
                    }
                    return new GLFWErrorJPL(errorCode, msg);
                }

                /**
                 * @param [out] er if passed a valid pointer, GLFWerrorJPL will be store in it
                 * @throw RuntimeException if current window could not be retrieved
                 * @return the current context window
                 */
                inline std::string getLastErrorAsString(GLFWErrorJPL** er = nullptr){
                    GLFWErrorJPL* cr = getLastError();
                    std::string buffer = "Error " + std::to_string(cr->errorCode) + ": " + std::string(cr->msg);
                    if(er != nullptr){
                        *er = cr;
                    }else{
                        delete cr;
                    }
                    return buffer;
                }
        }
    }
}

#endif