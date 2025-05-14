/**
 * @file
 * 
 * A decimal type
 * 
 * @author Gabriele-P03
 */
#ifndef DECIMAL_TYPE_XSD_JPL
#define DECIMAL_TYPE_XSD_JPL

#include "../AbstractType.hpp"

namespace jpl{
    namespace _parser{
        namespace _xsd{
            namespace _type{

                class Decimal : public AbstractType{
                    
                    protected:
                        Decimal(const std::string &name) : AbstractType(name){}
                        ~Decimal(){}

                    public:
                        Decimal() : AbstractType("DECIMAL"){}
                        static const Decimal* const DECIMAL;
                };

            }
        }
    }
}

#endif