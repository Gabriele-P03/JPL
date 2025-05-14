/**
 * @file
 * 
 * A negativeinteger type
 * 
 * @author Gabriele-P03
 */
#ifndef NEGATIVE_INTEGER_TYPE_XSD_JPL
#define NEGATIVE_INTEGER_TYPE_XSD_JPL

#include "../Integer.hpp"

namespace jpl{
    namespace _parser{
        namespace _xsd{
            namespace _type{

                class NegativeInteger : public Integer{
                    
                    protected:
                        ~NegativeInteger(){}

                    public:
                        NegativeInteger() : Integer("NEGATIVE_INTEGER"){}
                        static const NegativeInteger* const NEGATIVE_INTEGER;
                };

            }
        }
    }
}

#endif