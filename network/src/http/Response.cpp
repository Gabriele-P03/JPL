#include "Response.hpp"

jpl::_network::_http::Response::Response(const std::string &responseBody, const std::string &responseHeader)
    : responseBody(responseBody){

    jpl::_utils::_collections::_list::LinkedList<std::string>* splitted = jpl::_utils::_string::split(responseHeader, std::regex("\n"));
    for(long i = 0; i < splitted->getSize(); i++){
        std::string cr = jpl::_utils::_string::trim(splitted->get(i));
        if(!cr.empty()){
            jpl::_utils::_collections::_list::LinkedList<std::string>* keyvalue = jpl::_utils::_string::split(cr, std::regex(":"));

            //For any parameter which does not have a valid value separated by ':' (e.g. "HTTP/1.1 OK")
            std::string value = "";
            if(keyvalue->getSize() > 1){
                value = jpl::_utils::_string::trim(keyvalue->get(1)); 
            }

            this->headers.push_back(new jpl::_network::_http::HeaderParameter(
                jpl::_utils::_string::trim(keyvalue->get(0)),
                value
            ));
            delete keyvalue;
        }
    }
    delete splitted;
}

std::string jpl::_network::_http::Response::getHeader(const std::string &headerParKey) const{
    for(long i = 0; i < this->headers.size(); i++){
        Parameter* cr = this->headers.at(i);
        if(strcmp(cr->getKey().c_str(), headerParKey.c_str()) == 0){
            return cr->getValue();
        }
    }
    throw jpl::_exception::NotFoundException("Header " + headerParKey + " is not contained into this header list");
}

jpl::_network::_http::Response::~Response(){
    this->headers.clear();
}