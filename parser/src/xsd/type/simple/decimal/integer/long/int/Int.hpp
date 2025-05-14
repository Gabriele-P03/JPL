/**
 * @file
 * 
 * A short type
 * 
 * @author Gabriele-P03
 */
#ifndef SHORT_TYPE_XSD_JPL
#define SHORT_TYPE_XSD_JPL

#include "../Long.hpp"

namespace jpl{
    namespace _parser{
        namespace _xsd{
            namespace _type{

                class Short : public Long{
                    
                    protected:
                        Short(const std::string &name) : Long(name) {}
                        ~Short(){}

                    public:
                        Short() : Long("SHORT"){}
                        static const Short* const SHORT;
                };

            }
        }
    }
}

#endif