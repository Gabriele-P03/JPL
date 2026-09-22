#include "JSONUtils.hpp"


jpl::_parser::_json::JSONElement* jpl::_parser::_json::parseJSONString(std::string &buffer, bool parseName, size_t tabs){
    buffer = jpl::_utils::_string::trim(buffer);
    std::string name;
    if(parseName){
        jpl::_utils::_string::RegexMatch m = jpl::_utils::_string::findFirstOf(buffer, std::regex("\"[ ]*:"));
        name = jpl::_utils::_string::trim(buffer.substr(1, m.pos-1));
        buffer = buffer.substr(m.pos + m.length);
        buffer = jpl::_utils::_string::trim(buffer);
    }
    char s = buffer.at(0);
    if(s == '{'){
        size_t eS = jpl::_utils::_string::getIndexGroupOver(buffer, std::regex("\\{"), std::regex("\\}"));
        char e = buffer.at(eS);
        std::string b = jpl::_utils::_string::trim( buffer.substr(1, eS-1) ); //it also remove opening and closing bracket 
        jpl::_parser::_json::stct(buffer, eS+1);
        if(e != '}')
            throw jpl::_parser::_json::_exception::JSONParseException("Json String does not terminate with }");
        jpl::_parser::_json::JSONObject* obj = jpl::_parser::_json::parseJSONObjectByString(b, parseName, tabs);
        if(parseName)
            obj->setName(name);
        return obj;
    }else if(s == '['){
        size_t eS = jpl::_utils::_string::getIndexGroupOver(buffer, std::regex("\\["), std::regex("\\]"));
        char e = buffer.at(eS);
        std::string b = jpl::_utils::_string::trim( buffer.substr(1, eS-1) ); //it also remove opening and closing bracket 
        jpl::_parser::_json::stct(buffer, eS+1);
        if(e != ']')
            throw jpl::_parser::_json::_exception::JSONParseException("Json String does not terminate with ]");
        jpl::_parser::_json::JSONArray* array = jpl::_parser::_json::parseJSONArrayByString(b, parseName, tabs);
        if(parseName)
            array->setName(name);
        return array;
    }else{
        jpl::_parser::_json::JSONField* field = jpl::_parser::_json::parseJSONFieldByString(buffer, parseName, tabs);
        if(parseName)
            field->setName(name);
        buffer = "";     
        return field;
    }
    throw jpl::_parser::_json::_exception::JSONParseException(s + " is not a valid char at the beginning of a json file");
}

jpl::_parser::_json::JSONArray* jpl::_parser::_json::parseJSONArrayByString(std::string &buffer, bool parseName, size_t tabs){
    jpl::_parser::_json::JSONArray* root = new jpl::_parser::_json::JSONArray("", tabs);
    while(!buffer.empty()){
        void* ptr = parseJSONString(buffer, false, tabs+1);
        if(static_cast<jpl::_parser::_json::JSONArray*>(ptr)){
            root->addJSONArray( (jpl::_parser::_json::JSONArray*)ptr );
        }else if(static_cast<jpl::_parser::_json::JSONObject*>(ptr)){
            root->addJSONObject( (jpl::_parser::_json::JSONObject*)ptr );
        }else if(static_cast<jpl::_parser::_json::JSONField*>(ptr)){
            root->addJSONField( (jpl::_parser::_json::JSONField*)ptr );
        }
    }
    return root;
}

jpl::_parser::_json::JSONObject* jpl::_parser::_json::parseJSONObjectByString(std::string &buffer, bool parseName, size_t tabs ){
    jpl::_parser::_json::JSONObject* root = new jpl::_parser::_json::JSONObject("", tabs);
    while(!buffer.empty()){
        jpl::_parser::_json::JSONElement* ptr = jpl::_parser::_json::parseJSONString(buffer, true, tabs+1);
        if(dynamic_cast<jpl::_parser::_json::JSONArray*>(ptr)){
            root->addJSONArray( (jpl::_parser::_json::JSONArray*)ptr );
        }else if(dynamic_cast<jpl::_parser::_json::JSONObject*>(ptr)){
            root->addJSONObject( (jpl::_parser::_json::JSONObject*)ptr );
        }else if(dynamic_cast<jpl::_parser::_json::JSONField*>(ptr)){
            root->addJSONField( (jpl::_parser::_json::JSONField*)ptr );
        }
    }
    return root;
}

jpl::_parser::_json::JSONField* jpl::_parser::_json::parseJSONFieldByString(std::string &buffer, bool parseName, size_t tabs){
    if(buffer.empty())
        throw jpl::_parser::_json::_exception::JSONParseException("Trying to parse an empty string");
    try{
        std::tm tm;       //Try TM
        if(strptime(buffer.c_str(), "%Y-%m-%d %H:%M:%S", &tm) == NULL)  //As most of all DBMS require
            throw std::exception();
        return new jpl::_parser::_json::JSONFieldTM("", tabs, tm);
    }catch(const std::exception* ex){}
    if(jpl::_utils::_string::match(buffer, std::regex("^-?[0-9]+\\.?[0-9]*$"))){
        try{unsigned int ui = std::stoul(buffer.c_str()); return new jpl::_parser::_json::JSONFieldUInt("", tabs, ui);}catch(const std::exception* ex){} //String to ui
        try{unsigned int ul = std::stoul(buffer.c_str()); return new jpl::_parser::_json::JSONFieldULong("", tabs, ul);}catch(const std::exception* ex){} //String to ul
        try{long l = std::stol(buffer.c_str()); return new jpl::_parser::_json::JSONFieldLong("", tabs, l);}catch(const std::exception* ex){} //String to long
        try{int i = std::stoi(buffer.c_str()); return new jpl::_parser::_json::JSONFieldInt("", tabs, i);}catch(const std::exception* ex){} //String to int
        try{double d = std::stod(buffer.c_str()); return new jpl::_parser::_json::JSONFieldDouble("", tabs, d);}catch(const std::exception* ex){} //String to double
        try{float f = std::stof(buffer.c_str()); return new jpl::_parser::_json::JSONFieldFloat("", tabs, f);}catch(const std::exception* ex){} //String to float
    }
    if(buffer.compare("true") == 0 || buffer.compare("false"))
        try{bool b = std::stof(buffer.c_str()); return new jpl::_parser::_json::JSONFieldBool("", tabs, b);}catch(const std::exception* ex){} //String to bool
    return new jpl::_parser::_json::JSONFieldString("", tabs, buffer);
}

void jpl::_parser::_json::stct(std::string &buffer, size_t i){
    buffer = jpl::_utils::_string::trim(buffer.substr(i)); //substring and trim
    if(buffer.size() > 0){
        if(buffer.at(0) == ','){
            buffer = jpl::_utils::_string::trim(buffer.erase(0,1));   //remove comma and trim
        }
        if(!jpl::_utils::_string::startWith(buffer, jpl::_parser::_json::stct_regex)){
            throw jpl::_parser::_json::_exception::JSONParseException(buffer.at(0) + " is not a valid char for JSONobject/JSONArray/JSONField beginning");
        }
    }
}