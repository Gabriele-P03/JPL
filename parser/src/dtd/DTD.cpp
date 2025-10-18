#include "DTD.hpp"

jpl::_parser::_dtd::DTD* jpl::_parser::_dtd::parse(std::istream &s){

    std::string rootTag;
    std::getline(s, rootTag);
    rootTag = jpl::_utils::_string::trim(rootTag);
    //CHeck if it starts with '<!DOCTYPE name ['
    jpl::_parser::_dtd::DTD* dtd = nullptr;
    if( !jpl::_utils::_string::startWith(rootTag, std::regex("^<!DOCTYPE [a-zA-Z0-9_]+ \\[")) ){
        throw jpl::_parser::_dtd::_exception::DTDException("Root tag does not begin with <! DOCTYPE name [");
    }else{
        std::string name = rootTag.substr(sizeof("<!DOCTYPE"));
        name = name.substr(0, name.find(" "));
        dtd = new jpl::_parser::_dtd::DTD(name);
    }

    parseSingleTag(dtd, s);

    std::string endRootTag;
    std::getline(s, endRootTag);
    endRootTag = jpl::_utils::_string::trim(endRootTag);
    if(strcmp(endRootTag.c_str(), ">") != 0){
        throw jpl::_parser::_dtd::_exception::DTDException("DTD stream does not end with '>'");
    }
    return dtd;
}

void jpl::_parser::_dtd::parseSingleTag(jpl::_parser::_dtd::DTD* &dtd, std::istream &s){

    std::string buffer = "";
    bool tagPending = false; //Set to True when the for-loop below get a new tag
    std::string line = "";
    bool flagComment = false;
    while (std::getline(s, line) ){
        line = jpl::_utils::_string::trim(line);
        flagComment = jpl::_parser::_common::removeHTMLComment(line, flagComment);
        if(line.empty()){
            continue;   
        }
        if(!buffer.empty()){
            buffer += " ";
        }
        buffer += line;
        if(jpl::_utils::_string::startWith(line, std::regex("<!"))){
            if(tagPending){
                throw jpl::_parser::_dtd::_exception::DTDException("There's already a tag parsing");
            }
            tagPending = true;
        }
        if(jpl::_utils::_string::endWith(line, std::regex(">"))){
            break;
        }
    }
    buffer = buffer.erase(0, 2);
    if(jpl::_utils::_string::startWith(buffer, std::regex("ELEMENT "))){
        parseElement(dtd, buffer);
    }
}

void jpl::_parser::_dtd::parseElement(jpl::_parser::_dtd::DTD* &dtd, std::string &s){
    s = s.erase(0, sizeof("ELEMENT"));
    //s will always terminate with enclosing bracket, therefore it may erase last char and trim it
    s = jpl::_utils::_string::endTrim(s.erase(s.size()-1, 1));
    size_t l = s.find(" ");
    std::string name = s.substr(0, l);
    s = jpl::_utils::_string::trim(s.erase(0,l));
    name = jpl::_utils::_string::trim(name);
    if(!checkValidName(name)){
        throw jpl::_parser::_dtd::_exception::DTDException("DTD name is not valid: " + name);
    }
    jpl::_parser::_dtd::Element* element = dtd->getElementByName(name);
    if(element == nullptr){
        element = new jpl::_parser::_dtd::Element(name);
        dtd->addElement(element);
    }
    //Let's parse element's definition
    if(s == "ANY" || s == "EMPTY"){

    }else{
        if(s.at(0) != '(' || s.at(s.size()-1) != ')'){
            throw jpl::_parser::_dtd::_exception::DTDException("Element's definition must be enclosed by brackets: " + s);
        }
        s = s.substr(1, s.size()-2);
        jpl::_parser::_dtd::parseGroupElement(s, dtd);
    }
}

jpl::_parser::_dtd::GroupElement* jpl::_parser::_dtd::parseGroupElement(std::string &e, jpl::_parser::_dtd::DTD* &dtd){
    jpl::_utils::_collections::_list::LinkedList<std::string>* splitted_elements = jpl::_utils::_string::split(e, std::regex("(?<=\\().*(?=\\))|[a-zA-Z0-9]+[ ]*[+|?|*]?", std::regex_constants::basic|std::regex_constants::egrep));
    jpl::_parser::_dtd::GroupElement* cr = nullptr;
    for(size_t i = 0; i < splitted_elements->getSize(); i++){
        std::string cr_name = jpl::_utils::_string::trim(splitted_elements->get(i));
        jpl::_parser::_dtd::Amount amount;
        e = jpl::_utils::_string::trim(e.erase(0, cr_name.size()));
        if(!e.empty()){ //There's no left stuff to parse 
            char amount_describer = e.at(0);
            if(amount_describer == ','){
                amount = jpl::_parser::_dtd::SEQUENCE;
            }else if(amount_describer == '|'){
                amount = jpl::_parser::_dtd::CHOISE;
            }else{
                throw jpl::_parser::_dtd::_exception::DTDException("Amount describer " + std::string(1, amount_describer) + " for " + cr_name + " is not valid. Only ',' or '|')");
            }
            e = jpl::_utils::_string::startTrim(e.erase(0, 1));
        }
        if(cr == nullptr){
            cr = new jpl::_parser::_dtd::GroupElement(amount, jpl::_parser::_dtd::ANY);
        }else{
            if(cr->getAmount() != amount){
                throw jpl::_parser::_dtd::_exception::DTDException("Multiple use of amount describer for " + e);
            }
        }

        jpl::_parser::_dtd::Presence presence = jpl::_parser::_dtd::POSITIVE;
        if(cr_name.at(cr_name.size()-1) != ')'){    //Positive is implictly if no presence is declared
            presence = jpl::_parser::_dtd::parsePresence(cr_name);
        }

        if(cr_name.at(0) == '('){   //Sub-group detected
            cr_name = jpl::_utils::_string::startTrim(cr_name.erase(0,1));  //Remove opening bracket and start trim
            cr_name = jpl::_utils::_string::endTrim(cr_name.substr(0, cr_name.find_last_of(')')));//Remove closing bracket and trim
            jpl::_parser::_dtd::GroupElement* sub = jpl::_parser::_dtd::parseGroupElement(cr_name, dtd); //Recursive parsing
            cr->addGroupElement(sub);
        }else{//Single element to add
            jpl::_parser::_dtd::Element* element = dtd->getElementByName(cr_name);//Parse name
            if(element == nullptr){
                element = new jpl::_parser::_dtd::Element(cr_name);
                dtd->addElement(element);
            }
            jpl::_parser::_dtd::ElementPresence* elementPresence = new jpl::_parser::_dtd::ElementPresence(element, presence);
            cr->addElement(elementPresence);
        }
    }
    delete splitted_elements;
    return cr;
}

jpl::_parser::_dtd::Presence jpl::_parser::_dtd::parsePresence(std::string &e){
    char p = e.at(e.size()-1);//Parse Presence
    jpl::_parser::_dtd::Presence presence;
    bool removePresence = true;
    switch (p){
        case '*':
            presence = jpl::_parser::_dtd::ANY;
        break;
        case '?':
            presence = jpl::_parser::_dtd::BINARY;
        break;           
        default:
            if(p != '+'){
                removePresence = false;
            }
            presence = jpl::_parser::_dtd::POSITIVE;
        break;
    }
    if(removePresence){
        std::string g = jpl::_utils::_string::trim(e.erase(e.size()-1,1));    //Remove presence
        e = g;
    }
    return presence;
}

bool jpl::_parser::_dtd::checkValidName(const std::string &name){
    return jpl::_utils::_string::match(name, std::regex("^[a-zA-Z0-9]+$"));
}


void jpl::_parser::_dtd::DTD::addElement(jpl::_parser::_dtd::Element* element){
    jpl::_parser::_dtd::Element* e = this->getElementByName(element->getName());
    if (e != nullptr){
        throw jpl::_parser::_dtd::_exception::DTDException("DTD has already parsed " + element->getName() + " element");
    }
    this->elements.add(element);
}