/**
 * @file
 * 
 * A integer type
 * 
 * @author Gabriele-P03
 */
#ifndef INTEGER_TYPE_XSD_JPL
#define INTEGER_TYPE_XSD_JPL

#include "../Decimal.hpp"

namespace jpl{
    namespace _parser{
        namespace _xsd{
            namespace _type{

                class Integer : public Decimal{
                    
                    protected:
                        Integer(const std::string &name) : Decimal(name) {}
                        ~Integer(){}

                    public:
                        Integer() : Decimal("INTEGER"){}
                        static const Integer* const INTEGER;
                };

            }
        }
    }
}

#endif