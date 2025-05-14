/**
 * @file
 * 
 * A short type
 * 
 * @author Gabriele-P03
 */
#ifndef POSITIVE_INTEGER_TYPE_XSD_JPL
#define POSITIVE_INTEGER_TYPE_XSD_JPL

#include "../NonNegativeInteger.hpp"

namespace jpl{
    namespace _parser{
        namespace _xsd{
            namespace _type{

                class PositiveInteger : public NonNegativeInteger{
                    
                    protected:
                        PositiveInteger(const std::string &name) : NonNegativeInteger(name){}
                        ~PositiveInteger(){}

                    public:
                        PositiveInteger() : NonNegativeInteger("POSITIVE_INTEGER"){}
                        static const PositiveInteger* const POSITIVE_INTEGER;
                };

            }
        }
    }
}

#endif