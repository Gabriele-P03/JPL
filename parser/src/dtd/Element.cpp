#include "Element.hpp"

void jpl::_parser::_dtd::Element::addAttribute(jpl::_parser::_dtd::Attribute* attribute){
    std::string name = attribute->getName();
    for(unsigned int i=0; i < this->attributes.getSize(); i++){
        if(name == this->attributes.get(i)->getName()){
            throw jpl::_parser::_dtd::_exception::DTDException("Element " + this->name + " has already " + name + " attribute");
        }
    }
    this->attributes.add(attribute);
}

void jpl::_parser::_dtd::Element::addElement(jpl::_parser::_dtd::GroupElement* e){
    for(size_t i = 0; i < this->groupElements.getSize(); i++){
        jpl::_parser::_dtd::GroupElement* pr = this->groupElements.get(i);
        if(pr == e){
            throw jpl::_parser::_dtd::_exception::DTDException("Duplicate pointer to group-elements for the element " + this->name);
        }
    }
    this->groupElements.add(e);
}