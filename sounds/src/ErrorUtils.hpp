/**
 * Utility functions to retrieve OpenAL's occurred errors
 */
#ifndef OPENAL_ERRORS_UTILS_JPL
#define OPENAL_ERRORS_UTILS_JPL

#include <al/al.h>
#include <jpl/exception/runtime/RuntimeException.hpp>

namespace jpl{
    namespace _sounds{
        namespace _errors{

            inline ALenum getLastError(){
                return alGetError();
            }

            inline std::string getLastErrorAsString(const ALenum &v){
                if(v == AL_NO_ERROR){
                    return "OK";
                }
                if(v == AL_INVALID_NAME){
                    return "BUffer Name is invalid";
                }
                if(v == AL_INVALID_ENUM){
                    return "The specified format (or parameter) does not exists";
                }
                if(v == AL_INVALID_VALUE){
                    return "Value not valid";
                }
                if(v == AL_INVALID_OPERATION){
                    return "Operation not valid";
                }
                if(v == AL_OUT_OF_MEMORY){
                    return "Out of Memory Error";
                }
                return "Enum not valid";
            }

            inline std::string getLastErrorAsString(){
                return getLastErrorAsString(getLastError());
            }

            /**
             * Check for any OpenAL error. If occurred, a RuntimeException is thrown
             */
            inline void checkForError() {
                int err = getLastError();
                if(err != AL_NO_ERROR){
                    throw jpl::_exception::RuntimeException(getLastErrorAsString(err));
                }
            }
        }
    }
}

#endif