/**
 * @file
 * 
 * A float type
 * 
 * @author Gabriele-P03
 */
#ifndef DATE_TYPE_XSD_JPL
#define DATE_TYPE_XSD_JPL

#include "../AbstractType.hpp"

namespace jpl{
    namespace _parser{
        namespace _xsd{
            namespace _type{

                class Date : public AbstractType{
                    
                    protected:
                        ~Date(){}

                    public:
                        Date() : AbstractType("DATE"){}
                        static const Date* const DATE;
                };

            }
        }
    }
}

#endif