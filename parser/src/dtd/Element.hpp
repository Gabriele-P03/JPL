/**
 * @file Element.hpp
 * @author Gabriele-P03
 * @date 2025-06-30
 * 
 */
#ifndef DTD_ELEMENT_JPL
#define DTD_ELEMENT_JPL

#include <string>
#include <jpl/utils/structure/list/LinkedList.hpp>
#include "Attribute.hpp"
#include "DTDException.hpp"

namespace jpl{
    
    namespace _parser{
    
        namespace _dtd{

            class GroupElement;
    
            class Element{
    
                private:

                    std::string name;
                    _utils::_collections::_list::LinkedList<jpl::_parser::_dtd::GroupElement*> groupElements;
                    _utils::_collections::_list::LinkedList<Attribute*> attributes;
    
                public:

                    Element(std::string name){
                        this->name = name;
                    }

                    void addAttribute(Attribute* attribute);

                    void addElement(jpl::_parser::_dtd::GroupElement* e);

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

#endif