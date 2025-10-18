#include "GroupElement.hpp"

void jpl::_parser::_dtd::GroupElement::addElement(jpl::_parser::_dtd::ElementPresence* e){
    if(e == nullptr){
        throw jpl::_exception::NullPointerException("You cannot add a nullptr as element");
    }
    for(size_t i = 0; i < this->elements.getSize(); i++){
        if( !strcmp(this->elements.get(i)->element->getName().c_str(), e->element->getName().c_str())){
            throw jpl::_parser::_dtd::_exception::DTDException("Element " + e->element->getName() + " is already contained in elements list");
        }
    }
    this->elements.add(e);
}

void jpl::_parser::_dtd::GroupElement::addGroupElement(jpl::_parser::_dtd::GroupElement* eg){
    if(eg == nullptr){
        throw jpl::_exception::NullPointerException("You cannot add a nullptr as group element");
    }
    if(this->subGroups.contains(eg)){
       throw jpl::_parser::_dtd::_exception::DTDException("Group Element pointer already inserted in the current list"); 
    }
    this->subGroups.add(eg);
}

bool jpl::_parser::_dtd::GroupElement::hasElement(const std::string &name) const noexcept{
    for(size_t i = 0; i < this->elements.getSize(); i++){
        if( strcmp( this->elements.get(i)->element->getName().c_str(), name.c_str() ) ){
            return true;
        }
    }
    return false;
}

const jpl::_parser::_dtd::ElementPresence* jpl::_parser::_dtd::GroupElement::getElement(const std::string &name) const{
    for(size_t i = 0; i < this->elements.getSize(); i++){
        jpl::_parser::_dtd::ElementPresence* cr = this->elements.get(i);
        if( strcmp( cr->element->getName().c_str(), name.c_str() ) ){
            return cr;
        }
    }
    throw jpl::_parser::_dtd::_exception::DTDException("The current group element does not contain any " + name + " element");
}