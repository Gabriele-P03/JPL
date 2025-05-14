/**
 * @file
 * 
 * A short type
 * 
 * @author Gabriele-P03
 */
#ifndef BYTE_TYPE_XSD_JPL
#define BYTE_TYPE_XSD_JPL

#include "../Short.hpp"

namespace jpl{
    namespace _parser{
        namespace _xsd{
            namespace _type{

                class Byte : public Short{
                    
                    protected:
                        ~Byte(){}

                    public:
                        Byte() : Short("BYTE"){}
                        static const Byte* const BYTE;
                };

            }
        }
    }
}

#endif