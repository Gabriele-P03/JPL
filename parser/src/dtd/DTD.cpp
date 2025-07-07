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
    while (std::getline(s, line) ){
        line = jpl::_utils::_string::trim(line);
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
    size_t l = s.find(" ");
    std::string name = s.substr(0, l);
    s = jpl::_utils::_string::trim(s.erase(0,l));
    name = jpl::_utils::_string::trim(name);
    if(!checkValidName(name)){
        throw jpl::_parser::_dtd::_exception::DTDException("DTD name is not valid: " + name);
    }
}

bool jpl::_parser::_dtd::checkValidName(const std::string &name){
    return jpl::_utils::_string::match(name, std::regex("^[a-zA-Z0-9]+$"));
}