/**
 * @file
 * 
 * A short type
 * 
 * @author Gabriele-P03
 */
#ifndef NON_NEGATIVE_INTEGER_TYPE_XSD_JPL
#define NON_NEGATIVE_INTEGER_TYPE_XSD_JPL

#include "../Integer.hpp"

namespace jpl{
    namespace _parser{
        namespace _xsd{
            namespace _type{

                class NonNegativeInteger : public Integer{
                    
                    protected:
                        NonNegativeInteger(const std::string &name) : Integer(name){}
                        ~NonNegativeInteger(){}

                    public:
                        NonNegativeInteger() : Integer("NON_NEGATIVE_INTEGER"){}
                        static const NonNegativeInteger* const NON_NEGATIVE_INTEGER;
                };

            }
        }
    }
}

#endif