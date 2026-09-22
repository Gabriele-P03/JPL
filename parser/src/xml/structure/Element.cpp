#include "Element.hpp"

jpl::_parser::_xml::Element::Element(std::string name) : jpl::_parser::_xml::Element::Element(
    name, "", 0, jpl::_utils::_collections::_list::LinkedList<Element*>(), jpl::_utils::_collections::_list::LinkedList<Attribute*>()){
}


jpl::_parser::_xml::Element::Element(std::string name, std::string text, unsigned short tabs, jpl::_utils::_collections::_list::LinkedList<Element*> elements, jpl::_utils::_collections::_list::LinkedList<Attribute*> attributes)
    : name(name){    
    validateName(name);
    this->text = text;
    this->tabs = tabs;
    this->elements = elements;
    this->attributes = attributes;
}


/**
 * ELEMENT
 */

bool jpl::_parser::_xml::Element::hasElement(std::string name) const noexcept{
    for(unsigned long i = 0; i < this->elements.getSize(); i++){
        jpl::_parser::_xml::Element* cr = this->elements.get(i);
        if(cr->getName().compare(name) == 0){
            return true;
        }
    }
    return false;
}
jpl::_parser::_xml::Element* jpl::_parser::_xml::Element::getElement(std::string name) const{
    for(unsigned long i = 0; i < this->elements.getSize(); i++){
        jpl::_parser::_xml::Element* cr = this->elements.get(i);
        if(cr->getName().compare(name) == 0){
            return cr;
        }
    }
    throw jpl::_parser::_xml::_exception::XMLNotFoundException("Element " + name + " not found in " + this->name + " element");
}
void jpl::_parser::_xml::Element::removeElement(Element* element){
    if(element == nullptr){
        throw jpl::_exception::IllegalArgumentException("XML Remove Element: nullptr as element");
    }
    if(!this->elements.remove(element))
        throw jpl::_parser::_xml::_exception::XMLNotFoundException("Element " + element->getName() + " does not exist into element " + this->name);
} 
void jpl::_parser::_xml::Element::removeElement(std::string name){
    for(size_t i = 0; i < this->elements.getSize(); i++){
        Element* cr = this->elements.get(i);
        if(cr->getName().compare(name) == 0){
            this->elements.removeAt(i);
            return;
        }
    }
    throw jpl::_parser::_xml::_exception::XMLNotFoundException("Element " + name + " does not exist into element " + this->name);
} 
void jpl::_parser::_xml::Element::addElement(Element* element){
    if(element == nullptr){
        throw jpl::_exception::IllegalArgumentException("You cannot add nullptr as element");
    }
    element->setTabs(this->tabs+1);
    this->elements.add(element);
}


/**
 * ATTRIBUTE
 */

bool jpl::_parser::_xml::Element::hasAttribute(std::string name) const noexcept{
    for(unsigned long i = 0; i < this->attributes.getSize(); i++){
        jpl::_parser::_xml::Attribute* cr = this->attributes.get(i);
        if(cr->getName().compare(name) == 0){
            return true;
        }
    }
    return false;
}
jpl::_parser::_xml::Attribute* jpl::_parser::_xml::Element::getAttribute(std::string name) const{
    for(unsigned long i = 0; i < this->attributes.getSize(); i++){
        jpl::_parser::_xml::Attribute* cr = this->attributes.get(i);
        if(cr->getName().compare(name) == 0){
            return cr;
        }
    }
    throw jpl::_parser::_xml::_exception::XMLNotFoundException("Attribute " + name + " not found in " + this->name + " element");
}
void jpl::_parser::_xml::Element::removeAttribute(Attribute* attribute){
    if(attribute == nullptr){
        throw jpl::_exception::NullPointerException("XML Remove Attribute: nullptr as attribute");
    }
    if(!this->attributes.remove(attribute))
        throw jpl::_parser::_xml::_exception::XMLNotFoundException("Attribute " + attribute->getName() + " does not exist into element " + this->name);
} 
void jpl::_parser::_xml::Element::removeAttribute(std::string name){
    for(size_t i = 0; i < this->attributes.getSize(); i++){
        Attribute* cr = this->attributes.get(i);
        if(cr->getName().compare(name) == 0){
            this->attributes.removeAt(i);
            return;
        }
    }
    throw jpl::_parser::_xml::_exception::XMLNotFoundException("Attribute " + name + " does not exist into element " + this->name);
} 
void jpl::_parser::_xml::Element::addAttribute(Attribute* attribute){
    if(attribute == nullptr){
        throw jpl::_exception::IllegalArgumentException("You cannot add nullptr as attribute");
    }
    for(size_t i = 0; i < this->attributes.getSize(); i++){
        Attribute* cr = this->attributes.get(i);
        if(cr->getName().compare(attribute->getName()) == 0){
            throw jpl::_parser::_xml::_exception::XMLDuplicateException("There is already an attribute named as" + attribute->getName() + " into " + this->name);
        }
    }
    this->attributes.add(attribute);
}


jpl::_parser::_xml::Element::~Element(){
    this->elements.clear();
    this->attributes.clear();
}

std::string jpl::_parser::_xml::Element::toString() const noexcept{
    std::string tabsString = std::string(this->tabs, '\t');
    std::string buffer = tabsString + "<" + this->name;
    if(!this->attributes.isEmpty()){
        buffer += " ";
        for(size_t i = 0; i < this->attributes.getSize(); i++){
            Attribute* cr = this->attributes.get(i);
            buffer += cr->toString();
        }
    }
    if(this->text.empty() && this->elements.isEmpty()){
        buffer += "/>";
    }else{
        buffer += ">\n";
        if(!this->text.empty()){
            buffer += tabsString + "\t" + this->text + "\n";
        }
        for(size_t i = 0; i < this->elements.getSize(); i++){
            Element* cr = this->elements.get(i);
            buffer += cr->toString() + "\n";
        }
        buffer += tabsString + "</" + this->name + ">";
    } 
    return buffer;
}