/**
 * @file Attribute.hpp
 * @author Gabriele-P03
 * @date 2025-06-30
 * 
 */

#ifndef DTD_ATTRIBUTE_JPL
#define DTD_ATTRIBUTE_JPL

namespace jpl{

    namespace _parser{

        namespace _dtd{

            class Attribute{

                private: 
                    std::string name;

                public:
                    Attribute(const std::string &name){
                        this->name = name;
                    }

                    const std::string getName() const noexcept{
                        return this->name;
                    }

                    ~Attribute(){
                        
                    }
            };
        }
    }
}
#endif
 