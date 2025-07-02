/**
 * @file Element.hpp
 * @author Gabriele-P03
 * @date 2025-06-30
 * 
 */

#include <string>
#include <jpl/utils/structure/list/LinkedList.hpp>
#include "Attribute.hpp"
#include "ElementPresence.hpp"

namespace jpl{
    
    namespace _parser{
    
        namespace _dtd{
    
            class Element{
    
                private:

                    std::string name;
                    _utils::_collections::_list::LinkedList<ElementPresence*> elements;
                    _utils::_collections::_list::LinkedList<Attribute*> attributes;
    
                public:

                    Element(std::string name){
                        this->name = name;
                    }

                    void addAttribute(const Attribute* &attribute){
                        for(unsigned int i=0; i < this->attributes.getSize(); i++){
                            if (strcmp(attribute->getName(), this->attributes.get(i)->getName()) == 0){

                            }
                        }
                    }

                    void addElement(const ElementPresence* e);

                    const std::string getName() const noexcept{
                        return this->name;
                    }

                    ~Element(){
                        this->attributes.clear();
                    }
            };
        }
    }
}