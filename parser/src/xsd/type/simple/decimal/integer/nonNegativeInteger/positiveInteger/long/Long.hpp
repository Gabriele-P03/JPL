/**
 * @file
 * 
 * A long type
 * 
 * @author Gabriele-P03
 */
#ifndef UNSIGNED_LONG_TYPE_XSD_JPL
#define LONG_TYPE_XSD_JPL

#include "../PositiveInteger.hpp"

namespace jpl{
    namespace _parser{
        namespace _xsd{
            namespace _type{

                class Long : public PositiveInteger{
                    
                    protected:
                        Long(const std::string &name) : PositiveInteger(name) {}
                        ~Long(){}

                    public:
                        Long() : PositiveInteger("LONG"){}
                        static const Long* const LONG;
                };

            }
        }
    }
}

#endif