#include "JSONObject.hpp"

jpl::_parser::_json::JSONObject::JSONObject(std::string name, unsigned long tabs){
    this->name = name;
    this->tabs = tabs;
} 
jpl::_parser::_json::JSONObject::JSONObject(std::string name) : jpl::_parser::_json::JSONObject(name, 0){} 
jpl::_parser::_json::JSONObject::JSONObject() : jpl::_parser::_json::JSONObject(""){}

jpl::_parser::_json::JSONObject::~JSONObject(){
    this->fields.clear();
    this->jsonArrays.clear();
    this->jsonObjects.clear();
}

/**
 * --------------------------------------- JSON Object ---------------------------------------
*/

jpl::_parser::_json::JSONObject* jpl::_parser::_json::JSONObject::getJSONObject(std::string name) const{
    for(size_t i = 0; i < this->jsonObjects.getSize(); i++){
        jpl::_parser::_json::JSONObject* cr = this->jsonObjects.get(i);
        if(cr->getName().compare(name) == 0)
            return cr;
    }
    throw jpl::_parser::_json::_exception::JSONKeyNotFoundException(name + " json object has not been found");
}

jpl::_parser::_json::JSONObject* jpl::_parser::_json::JSONObject::getJSONObject(unsigned long index) const{
    if(index >= this->jsonObjects.getSize())
        throw jpl::_exception::IndexOutOfBoundsException(this->jsonObjects.getSize(), index);
    return this->jsonObjects.get(index);
}

/**
 * --------------------------------------- JSON Array ---------------------------------------
*/

jpl::_parser::_json::JSONArray* jpl::_parser::_json::JSONObject::getJSONArray(std::string name) const{
    for(size_t i = 0; i < this->jsonArrays.getSize(); i++){
        jpl::_parser::_json::JSONArray* cr = this->jsonArrays.get(i);
        if(cr->getName().compare(name) == 0)
            return cr;
    }
    throw jpl::_parser::_json::_exception::JSONKeyNotFoundException(name + " json array has not been found");
}

jpl::_parser::_json::JSONArray* jpl::_parser::_json::JSONObject::getJSONArray(unsigned long index) const{
    if(index >= this->jsonArrays.getSize())
        throw jpl::_exception::IndexOutOfBoundsException(this->jsonArrays.getSize(), index);
    return this->jsonArrays.get(index);
}



/**
 * --------------------------------------- JSON Fields ---------------------------------------
*/

jpl::_parser::_json::JSONField* jpl::_parser::_json::JSONObject::getJSONField(std::string name) const{
    for(size_t i = 0; i < this->fields.getSize(); i++){
        jpl::_parser::_json::JSONField* cr = this->fields.get(i);
        if(cr->getName().compare(name) == 0)
            return cr;
    }
    throw jpl::_parser::_json::_exception::JSONKeyNotFoundException(name + " json field has not been found");
}

jpl::_parser::_json::JSONField* jpl::_parser::_json::JSONObject::getJSONField(unsigned long index) const{
    if(index >= this->fields.getSize())
        throw jpl::_exception::IndexOutOfBoundsException(this->fields.getSize(), index);
    return this->fields.get(index);
}

//  --------------------------------------- JSON Fields Int ---------------------------------------

jpl::_parser::_json::JSONFieldInt* jpl::_parser::_json::JSONObject::getJSONFieldInt(std::string name) const{
    for(size_t i = 0; i < this->fields.getSize(); i++){
        jpl::_parser::_json::JSONField* cr = this->fields.get(i);
        if(cr->getName().compare(name) == 0){
            if( typeid(*cr) == typeid(jpl::_parser::_json::JSONFieldInt) )
                return static_cast<jpl::_parser::_json::JSONFieldInt*>(cr);
        }
    }
    throw jpl::_parser::_json::_exception::JSONKeyNotFoundException(name + " json field int has not been found");
}

//  --------------------------------------- JSON Fields UInt ---------------------------------------

jpl::_parser::_json::JSONFieldUInt* jpl::_parser::_json::JSONObject::getJSONFieldUInt(std::string name) const{
    for(size_t i = 0; i < this->fields.getSize(); i++){
        jpl::_parser::_json::JSONField* cr = this->fields.get(i);
        if(cr->getName().compare(name) == 0){
            if( typeid(*cr) == typeid(jpl::_parser::_json::JSONFieldUInt) )
                return static_cast<jpl::_parser::_json::JSONFieldUInt*>(cr);
        }
    }
    throw jpl::_parser::_json::_exception::JSONKeyNotFoundException(name + " json field uint has not been found");
}

//  --------------------------------------- JSON Fields Long ---------------------------------------

jpl::_parser::_json::JSONFieldLong* jpl::_parser::_json::JSONObject::getJSONFieldLong(std::string name) const{
    for(size_t i = 0; i < this->fields.getSize(); i++){
        jpl::_parser::_json::JSONField* cr = this->fields.get(i);
        if(cr->getName().compare(name) == 0){
            if( typeid(*cr) == typeid(jpl::_parser::_json::JSONFieldLong) )
                return static_cast<jpl::_parser::_json::JSONFieldLong*>(cr);
        }
    }
    throw jpl::_parser::_json::_exception::JSONKeyNotFoundException(name + " json field long has not been found");
}
//  --------------------------------------- JSON Fields ULong ---------------------------------------

jpl::_parser::_json::JSONFieldULong* jpl::_parser::_json::JSONObject::getJSONFieldULong(std::string name) const{
    for(size_t i = 0; i < this->fields.getSize(); i++){
        jpl::_parser::_json::JSONField* cr = this->fields.get(i);
        if(cr->getName().compare(name) == 0){
            if( typeid(*cr) == typeid(jpl::_parser::_json::JSONFieldULong) )
                return static_cast<jpl::_parser::_json::JSONFieldULong*>(cr);
        }
    }
    throw jpl::_parser::_json::_exception::JSONKeyNotFoundException(name + " json field ulong has not been found");
}

//  --------------------------------------- JSON Fields Float ---------------------------------------

jpl::_parser::_json::JSONFieldFloat* jpl::_parser::_json::JSONObject::getJSONFieldFloat(std::string name) const{
    for(size_t i = 0; i < this->fields.getSize(); i++){
        jpl::_parser::_json::JSONField* cr = this->fields.get(i);
        if(cr->getName().compare(name) == 0){
            if( typeid(*cr) == typeid(jpl::_parser::_json::JSONFieldFloat) )
                return static_cast<jpl::_parser::_json::JSONFieldFloat*>(cr);
        }
    }
    throw jpl::_parser::_json::_exception::JSONKeyNotFoundException(name + " json field float has not been found");
}

//  --------------------------------------- JSON Fields Double ---------------------------------------

jpl::_parser::_json::JSONFieldDouble* jpl::_parser::_json::JSONObject::getJSONFieldDouble(std::string name) const{
    for(size_t i = 0; i < this->fields.getSize(); i++){
        jpl::_parser::_json::JSONField* cr = this->fields.get(i);
        if(cr->getName().compare(name) == 0){
            if( typeid(*cr) == typeid(jpl::_parser::_json::JSONFieldDouble) )
                return static_cast<jpl::_parser::_json::JSONFieldDouble*>(cr);
        }
    }
    throw jpl::_parser::_json::_exception::JSONKeyNotFoundException(name + " json field double has not been found");
}

//  --------------------------------------- JSON Fields Bool ---------------------------------------

jpl::_parser::_json::JSONFieldBool* jpl::_parser::_json::JSONObject::getJSONFieldBool(std::string name) const{
    for(size_t i = 0; i < this->fields.getSize(); i++){
        jpl::_parser::_json::JSONField* cr = this->fields.get(i);
        if(cr->getName().compare(name) == 0){
            if( typeid(*cr) == typeid(jpl::_parser::_json::JSONFieldBool) )
                return static_cast<jpl::_parser::_json::JSONFieldBool*>(cr);
        }
    }
    throw jpl::_parser::_json::_exception::JSONKeyNotFoundException(name + " json field bool has not been found");
}



//  --------------------------------------- JSON Fields TM ---------------------------------------

jpl::_parser::_json::JSONFieldTM* jpl::_parser::_json::JSONObject::getJSONFieldTM(std::string name) const{
    for(size_t i = 0; i < this->fields.getSize(); i++){
        jpl::_parser::_json::JSONField* cr = this->fields.get(i);
        if(cr->getName().compare(name) == 0){
            if( typeid(*cr) == typeid(jpl::_parser::_json::JSONFieldTM) )
                return static_cast<jpl::_parser::_json::JSONFieldTM*>(cr);
        }
    }
    throw jpl::_parser::_json::_exception::JSONKeyNotFoundException(name + " json field tm has not been found");
}

//  --------------------------------------- JSON Fields String ---------------------------------------

jpl::_parser::_json::JSONFieldString* jpl::_parser::_json::JSONObject::getJSONFieldString(std::string name) const{
    for(size_t i = 0; i < this->fields.getSize(); i++){
        jpl::_parser::_json::JSONField* cr = this->fields.get(i);
        if(cr->getName().compare(name) == 0){
            if( typeid(*cr) == typeid(jpl::_parser::_json::JSONFieldString) )
                return static_cast<jpl::_parser::_json::JSONFieldString*>(cr);
        }
    }
    throw jpl::_parser::_json::_exception::JSONKeyNotFoundException(name + " json field string has not been found");
}




void jpl::_parser::_json::JSONObject::addJSONObject(JSONObject* jsonObject){
    if(jsonObject == nullptr)
        throw jpl::_exception::IllegalArgumentException("You cannot add null json object");
    for(size_t i = 0; i < this->jsonObjects.getSize(); i++){
        if(this->jsonObjects.get(i)->getName().compare(jsonObject->getName()) == 0)
            throw jpl::_parser::_json::_exception::JSONDuplicateElementException(jsonObject->getName() + " object has been already parsed in the JSONObject " + this->name);
    }   
    this->jsonObjects.add(jsonObject);
}
void jpl::_parser::_json::JSONObject::addJSONArray(JSONArray* jsonArray){
    if(jsonArray == nullptr)
        throw jpl::_exception::IllegalArgumentException("You cannot add null json array");
    for(size_t i = 0; i < this->jsonArrays.getSize(); i++){
        if(this->jsonArrays.get(i)->getName().compare(jsonArray->getName()) == 0)
            throw jpl::_parser::_json::_exception::JSONDuplicateElementException(jsonArray->getName() + " has array been already parsed in the JSONObject " + this->name);
    }       
    this->jsonArrays.add(jsonArray);
}
void jpl::_parser::_json::JSONObject::addJSONField(JSONField* field){
    if(field == nullptr)
        throw jpl::_exception::IllegalArgumentException("You cannot add null json field");
    for(size_t i = 0; i < this->fields.getSize(); i++){
        if(this->fields.get(i)->getName().compare(field->getName()) == 0)
            throw jpl::_parser::_json::_exception::JSONDuplicateElementException(field->getName() + " field has been already parsed in the JSONObject " + this->name);
    }    
    this->fields.add(field);
}



std::string jpl::_parser::_json::JSONObject::toString() const noexcept{
    std::string buffer = std::string(this->tabs, '\t');
    if(!this->name.empty()){
        buffer += "\"" + this->name + "\":";
    }
    buffer += "{\n";
    size_t objSize = this->jsonObjects.getSize(), arrSize = this->jsonArrays.getSize(), fldSize = this->fields.getSize();
    for(size_t i = 0; i < objSize; i++){
        buffer += this->jsonObjects.get(i)->toString();
        buffer += (i < objSize-1 ? ",\n" : "\n");
    }
    for(size_t i = 0; i < arrSize; i++){
        buffer += this->jsonArrays.get(i)->toString();
        buffer += (i < arrSize-1 ? ",\n" : "\n");
    }
    for(size_t i = 0; i < fldSize; i++){
        buffer += this->fields.get(i)->toString();
        buffer += (i < fldSize-1 ? ",\n" : "\n");
    }
    buffer += std::string(this->tabs, '\t') + "}";
    return buffer;
}
