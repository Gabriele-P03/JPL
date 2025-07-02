#include "DTD.hpp"

jpl::_parser::_dtd::DTD* jpl::_parser::_dtd::parse(std::istream* s){

    return nullptr;
}

std::string jpl::_parser::_dtd::parseSingleTag(std::istream* s){

    std::string buffer = "";
    bool tagPending = false; //Set to True when the for-loop below get a new tag

    for ( std::string line; std::getline(*s, line); ){
        
        if( jpl::_utils::_string::startWith(line, std::regex("<!")) ){
            if(tagPending){
                throw jpl::_parser::_dtd::_exception::DTDException("It seems like there is ");
            }
        }
    }
}