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

void jpl::_parser::_dtd::Element::addElement(jpl::_parser::_dtd::ElementPresence* e){
    std::string name = e->element->getName();
    for(size_t i = 0; i < this->elements.getSize(); i++){
        jpl::_parser::_dtd::ElementPresence* pr = this->elements.get(i);
        if(pr->element->getName() == name){
            throw jpl::_parser::_dtd::_exception::DTDException("For the element " + this->name + " the sub-element named as " + name + " has been already added");
        }
    }
    this->elements.add(e);
}