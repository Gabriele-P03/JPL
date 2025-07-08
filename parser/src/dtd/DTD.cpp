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
        flagComment = removeComment(line, flagComment);
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

}

bool jpl::_parser::_dtd::checkValidName(const std::string &name){
    return jpl::_utils::_string::match(name, std::regex("^[a-zA-Z0-9]+$"));
}

bool jpl::_parser::_dtd::removeComment(std::string &line, bool flagComment) noexcept{
    size_t s = line.find("<!--");
    bool another_comment = false;
    if (s != std::string::npos){
        size_t s1 = line.find("<!--", (s+sizeof("<!--")));
        bool s1_npos = s1 != std::string::npos;
        if(flagComment || s1_npos){
            if(s1_npos){
                another_comment = true;
            }
            if(line.find("-->") > s1){
                throw jpl::_parser::_dtd::_exception::DTDException("It seems like there's an innested comment");
            }
        }
        flagComment = true;
    }else{
        s = 0;
    }
    size_t e = line.find("-->");
    if(e != std::string::npos){
        if(!flagComment){
            throw jpl::_parser::_dtd::_exception::DTDException("It seems like an end-tag does not close any comment");
        }
        if(e<s){
            s = 0;  //It means that a new comment starts on the same line where end tag of the previous comment is located at
            another_comment = true;
        }
        flagComment = false;
        e += sizeof("-->")-1;
    }else{
        if(flagComment){
            e = line.size();    //Previous call of this function did not found end-tag of comment
        }else{
            e = 0;
        }
    }
    line = jpl::_utils::_string::trim(line.erase(s,e-s));
    if(another_comment){
        flagComment = removeComment(line, false);
    }
    return flagComment;
}

void jpl::_parser::_dtd::DTD::addElement(jpl::_parser::_dtd::Element* element){
    jpl::_parser::_dtd::Element* e = this->getElementByName(element->getName());
    if (e != nullptr){
        throw jpl::_parser::_dtd::_exception::DTDException("DTD has already parsed " + element->getName() + " element");
    }
    this->elements.add(element);
}