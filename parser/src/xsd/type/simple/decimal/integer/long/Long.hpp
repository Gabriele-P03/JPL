/**
 * @file
 * 
 * A long type
 * 
 * @author Gabriele-P03
 */
#ifndef LONG_TYPE_XSD_JPL
#define LONG_TYPE_XSD_JPL

#include "../Integer.hpp"

namespace jpl{
    namespace _parser{
        namespace _xsd{
            namespace _type{

                class Long : public Integer{
                    
                    protected:
                        Long(const std::string &name) : Integer(name) {}
                        ~Long(){}

                    public:
                        Long() : Integer("LONG"){}
                        static const Long* const LONG;
                };

            }
        }
    }
}

#endif