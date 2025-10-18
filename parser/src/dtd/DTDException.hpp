/**
 * 
 * A DTDException is thrown whenever a runtime error occurred during a DTD parsing
 * 
 * @author Gabriele-P03
 * @date 2025-07-01
 * 
 */

#ifndef DTD_EXCEPTION_JPL
#define DTD_EXCEPTION_JPL


#include <jpl/exception/runtime/RuntimeException.hpp>

namespace jpl{

    namespace _parser{

        namespace _dtd{

            namespace _exception{

                class DTDException : public jpl::_exception::RuntimeException{

                    public:
                        DTDException(std::string msg) : jpl::_exception::RuntimeException("DTDException", msg){
                        }
                };
            }
        }
    }
}


#endif