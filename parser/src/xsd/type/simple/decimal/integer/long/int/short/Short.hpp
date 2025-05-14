/**
 * @file
 * 
 * A short type
 * 
 * @author Gabriele-P03
 */
#ifndef SHORT_TYPE_XSD_JPL
#define SHORT_TYPE_XSD_JPL

#include "../Int.hpp"

namespace jpl{
    namespace _parser{
        namespace _xsd{
            namespace _type{

                class Short : public Int{
                    
                    protected:
                        Short(const std::string &name) : Int(name) {}
                        ~Short(){}

                    public:
                        Short() : Int("SHORT"){}
                        static const Short* const SHORT;
                };

            }
        }
    }
}

#endif