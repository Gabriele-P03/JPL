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
    throw new jpl::_parser::_json::_exception::JSONKeyNotFoundException(name + " json object has not been found");
}

jpl::_parser::_json::JSONObject* jpl::_parser::_json::JSONObject::getJSONObject(unsigned long index) const{
    if(index >= this->jsonObjects.getSize())
        throw new jpl::_exception::IndexOutOfBoundsException(this->jsonObjects.getSize(), index);
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
    throw new jpl::_parser::_json::_exception::JSONKeyNotFoundException(name + " json array has not been found");
}

jpl::_parser::_json::JSONArray* jpl::_parser::_json::JSONObject::getJSONArray(unsigned long index) const{
    if(index >= this->jsonArrays.getSize())
        throw new jpl::_exception::IndexOutOfBoundsException(this->jsonArrays.getSize(), index);
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
    throw new jpl::_parser::_json::_exception::JSONKeyNotFoundException(name + " json field has not been found");
}

jpl::_parser::_json::JSONField* jpl::_parser::_json::JSONObject::getJSONField(unsigned long index) const{
    if(index >= this->fields.getSize())
        throw new jpl::_exception::IndexOutOfBoundsException(this->fields.getSize(), index);
    return this->fields.get(index);
}

//  --------------------------------------- JSON Fields Int ---------------------------------------

jpl::_parser::_json::JSONFieldInt* jpl::_parser::_json::JSONObject::getJSONFieldInt(std::string name) const{
    for(size_t i = 0; i < this->fields.getSize(); i++){
        jpl::_parser::_json::JSONField* cr = this->fields.get(i);
        if(cr->getName().compare(name) == 0){
            if( typeid(cr) == typeid(jpl::_parser::_json::JSONFieldInt) )
                return static_cast<jpl::_parser::_json::JSONFieldInt*>(cr);
        }
    }
    throw new jpl::_parser::_json::_exception::JSONKeyNotFoundException(name + " json field int has not been found");
}

jpl::_parser::_json::JSONFieldInt* jpl::_parser::_json::JSONObject::getJSONFieldInt(unsigned long index) const{
    if(index >= this->intSize)
        throw new jpl::_exception::IndexOutOfBoundsException(this->fields.getSize(), index);
    size_t c = 0;
    for(size_t i = 0; i < this->fields.getSize(); i++){
        jpl::_parser::_json::JSONField* cr = this->fields.get(i);
        if( typeid(cr) == typeid(jpl::_parser::_json::JSONFieldInt) ){
            if(c++ == index)
                return static_cast<jpl::_parser::_json::JSONFieldInt*>(cr);
        }
    }  
    throw new jpl::_exception::RuntimeException("Altough index is into the range, there's no valid json field. This is a JPL bug, please report...");
}

//  --------------------------------------- JSON Fields UInt ---------------------------------------

jpl::_parser::_json::JSONFieldUInt* jpl::_parser::_json::JSONObject::getJSONFieldUInt(std::string name) const{
    for(size_t i = 0; i < this->fields.getSize(); i++){
        jpl::_parser::_json::JSONField* cr = this->fields.get(i);
        if(cr->getName().compare(name) == 0){
            if( typeid(cr) == typeid(jpl::_parser::_json::JSONFieldUInt) )
                return static_cast<jpl::_parser::_json::JSONFieldUInt*>(cr);
        }
    }
    throw new jpl::_parser::_json::_exception::JSONKeyNotFoundException(name + " json field uint has not been found");
}

jpl::_parser::_json::JSONFieldUInt* jpl::_parser::_json::JSONObject::getJSONFieldUInt(unsigned long index) const{
    if(index >= this->uintSize)
        throw new jpl::_exception::IndexOutOfBoundsException(this->fields.getSize(), index);
    size_t c = 0;
    for(size_t i = 0; i < this->fields.getSize(); i++){
        jpl::_parser::_json::JSONField* cr = this->fields.get(i);
        if( typeid(cr) == typeid(jpl::_parser::_json::JSONFieldUInt) ){
            if(c++ == index)
                return static_cast<jpl::_parser::_json::JSONFieldUInt*>(cr);
        }
    }  
    throw new jpl::_exception::RuntimeException("Altough index is into the range, there's no valid json field. This is a JPL bug, please report...");
}

//  --------------------------------------- JSON Fields Long ---------------------------------------

jpl::_parser::_json::JSONFieldLong* jpl::_parser::_json::JSONObject::getJSONFieldLong(std::string name) const{
    for(size_t i = 0; i < this->fields.getSize(); i++){
        jpl::_parser::_json::JSONField* cr = this->fields.get(i);
        if(cr->getName().compare(name) == 0){
            if( typeid(cr) == typeid(jpl::_parser::_json::JSONFieldLong) )
                return static_cast<jpl::_parser::_json::JSONFieldLong*>(cr);
        }
    }
    throw new jpl::_parser::_json::_exception::JSONKeyNotFoundException(name + " json field long has not been found");
}

jpl::_parser::_json::JSONFieldLong* jpl::_parser::_json::JSONObject::getJSONFieldLong(unsigned long index) const{
    if(index >= this->longSize)
        throw new jpl::_exception::IndexOutOfBoundsException(this->fields.getSize(), index);
    size_t c = 0;
    for(size_t i = 0; i < this->fields.getSize(); i++){
        jpl::_parser::_json::JSONField* cr = this->fields.get(i);
        if( typeid(cr) == typeid(jpl::_parser::_json::JSONFieldLong) ){
            if(c++ == index)
                return static_cast<jpl::_parser::_json::JSONFieldLong*>(cr);
        }
    }  
    throw new jpl::_exception::RuntimeException("Altough index is into the range, there's no valid json field. This is a JPL bug, please report...");
}

//  --------------------------------------- JSON Fields ULong ---------------------------------------

jpl::_parser::_json::JSONFieldULong* jpl::_parser::_json::JSONObject::getJSONFieldULong(std::string name) const{
    for(size_t i = 0; i < this->fields.getSize(); i++){
        jpl::_parser::_json::JSONField* cr = this->fields.get(i);
        if(cr->getName().compare(name) == 0){
            if( typeid(cr) == typeid(jpl::_parser::_json::JSONFieldULong) )
                return static_cast<jpl::_parser::_json::JSONFieldULong*>(cr);
        }
    }
    throw new jpl::_parser::_json::_exception::JSONKeyNotFoundException(name + " json field ulong has not been found");
}

jpl::_parser::_json::JSONFieldULong* jpl::_parser::_json::JSONObject::getJSONFieldULong(unsigned long index) const{
    if(index >= this->ulongSize)
        throw new jpl::_exception::IndexOutOfBoundsException(this->fields.getSize(), index);
    size_t c = 0;
    for(size_t i = 0; i < this->fields.getSize(); i++){
        jpl::_parser::_json::JSONField* cr = this->fields.get(i);
        if( typeid(cr) == typeid(jpl::_parser::_json::JSONFieldULong) ){
            if(c++ == index)
                return static_cast<jpl::_parser::_json::JSONFieldULong*>(cr);
        }
    }  
    throw new jpl::_exception::RuntimeException("Altough index is into the range, there's no valid json field. This is a JPL bug, please report...");
}


//  --------------------------------------- JSON Fields Float ---------------------------------------

jpl::_parser::_json::JSONFieldFloat* jpl::_parser::_json::JSONObject::getJSONFieldFloat(std::string name) const{
    for(size_t i = 0; i < this->fields.getSize(); i++){
        jpl::_parser::_json::JSONField* cr = this->fields.get(i);
        if(cr->getName().compare(name) == 0){
            if( typeid(cr) == typeid(jpl::_parser::_json::JSONFieldFloat) )
                return static_cast<jpl::_parser::_json::JSONFieldFloat*>(cr);
        }
    }
    throw new jpl::_parser::_json::_exception::JSONKeyNotFoundException(name + " json field float has not been found");
}

jpl::_parser::_json::JSONFieldFloat* jpl::_parser::_json::JSONObject::getJSONFieldFloat(unsigned long index) const{
    if(index >= this->floatSize)
        throw new jpl::_exception::IndexOutOfBoundsException(this->fields.getSize(), index);
    size_t c = 0;
    for(size_t i = 0; i < this->fields.getSize(); i++){
        jpl::_parser::_json::JSONField* cr = this->fields.get(i);
        if( typeid(cr) == typeid(jpl::_parser::_json::JSONFieldFloat) ){
            if(c++ == index)
                return static_cast<jpl::_parser::_json::JSONFieldFloat*>(cr);
        }
    }  
    throw new jpl::_exception::RuntimeException("Altough index is into the range, there's no valid json field. This is a JPL bug, please report...");
}


//  --------------------------------------- JSON Fields Double ---------------------------------------

jpl::_parser::_json::JSONFieldDouble* jpl::_parser::_json::JSONObject::getJSONFieldDouble(std::string name) const{
    for(size_t i = 0; i < this->fields.getSize(); i++){
        jpl::_parser::_json::JSONField* cr = this->fields.get(i);
        if(cr->getName().compare(name) == 0){
            if( typeid(cr) == typeid(jpl::_parser::_json::JSONFieldDouble) )
                return static_cast<jpl::_parser::_json::JSONFieldDouble*>(cr);
        }
    }
    throw new jpl::_parser::_json::_exception::JSONKeyNotFoundException(name + " json field double has not been found");
}

jpl::_parser::_json::JSONFieldDouble* jpl::_parser::_json::JSONObject::getJSONFieldDouble(unsigned long index) const{
    if(index >= this->doubleSize)
        throw new jpl::_exception::IndexOutOfBoundsException(this->fields.getSize(), index);
    size_t c = 0;
    for(size_t i = 0; i < this->fields.getSize(); i++){
        jpl::_parser::_json::JSONField* cr = this->fields.get(i);
        if( typeid(cr) == typeid(jpl::_parser::_json::JSONFieldDouble) ){
            if(c++ == index)
                return static_cast<jpl::_parser::_json::JSONFieldDouble*>(cr);
        }
    }  
    throw new jpl::_exception::RuntimeException("Altough index is into the range, there's no valid json field. This is a JPL bug, please report...");
}


//  --------------------------------------- JSON Fields Bool ---------------------------------------

jpl::_parser::_json::JSONFieldBool* jpl::_parser::_json::JSONObject::getJSONFieldBool(std::string name) const{
    for(size_t i = 0; i < this->fields.getSize(); i++){
        jpl::_parser::_json::JSONField* cr = this->fields.get(i);
        if(cr->getName().compare(name) == 0){
            if( typeid(cr) == typeid(jpl::_parser::_json::JSONFieldBool) )
                return static_cast<jpl::_parser::_json::JSONFieldBool*>(cr);
        }
    }
    throw new jpl::_parser::_json::_exception::JSONKeyNotFoundException(name + " json field bool has not been found");
}

jpl::_parser::_json::JSONFieldBool* jpl::_parser::_json::JSONObject::getJSONFieldBool(unsigned long index) const{
    if(index >= this->boolSize)
        throw new jpl::_exception::IndexOutOfBoundsException(this->fields.getSize(), index);
    size_t c = 0;
    for(size_t i = 0; i < this->fields.getSize(); i++){
        jpl::_parser::_json::JSONField* cr = this->fields.get(i);
        if( typeid(cr) == typeid(jpl::_parser::_json::JSONFieldBool) ){
            if(c++ == index)
                return static_cast<jpl::_parser::_json::JSONFieldBool*>(cr);
        }
    }  
    throw new jpl::_exception::RuntimeException("Altough index is into the range, there's no valid json field. This is a JPL bug, please report...");
}


//  --------------------------------------- JSON Fields TM ---------------------------------------

jpl::_parser::_json::JSONFieldTM* jpl::_parser::_json::JSONObject::getJSONFieldTM(std::string name) const{
    for(size_t i = 0; i < this->fields.getSize(); i++){
        jpl::_parser::_json::JSONField* cr = this->fields.get(i);
        if(cr->getName().compare(name) == 0){
            if( typeid(cr) == typeid(jpl::_parser::_json::JSONFieldTM) )
                return static_cast<jpl::_parser::_json::JSONFieldTM*>(cr);
        }
    }
    throw new jpl::_parser::_json::_exception::JSONKeyNotFoundException(name + " json field tm has not been found");
}

jpl::_parser::_json::JSONFieldTM* jpl::_parser::_json::JSONObject::getJSONFieldTM(unsigned long index) const{
    if(index >= this->tmSize)
        throw new jpl::_exception::IndexOutOfBoundsException(this->fields.getSize(), index);
    size_t c = 0;
    for(size_t i = 0; i < this->fields.getSize(); i++){
        jpl::_parser::_json::JSONField* cr = this->fields.get(i);
        if( typeid(cr) == typeid(jpl::_parser::_json::JSONFieldTM) ){
            if(c++ == index)
                return static_cast<jpl::_parser::_json::JSONFieldTM*>(cr);
        }
    }  
    throw new jpl::_exception::RuntimeException("Altough index is into the range, there's no valid json field. This is a JPL bug, please report...");
}


//  --------------------------------------- JSON Fields String ---------------------------------------

jpl::_parser::_json::JSONFieldString* jpl::_parser::_json::JSONObject::getJSONFieldString(std::string name) const{
    for(size_t i = 0; i < this->fields.getSize(); i++){
        jpl::_parser::_json::JSONField* cr = this->fields.get(i);
        if(cr->getName().compare(name) == 0){
            if( typeid(cr) == typeid(jpl::_parser::_json::JSONFieldString) )
                return static_cast<jpl::_parser::_json::JSONFieldString*>(cr);
        }
    }
    throw new jpl::_parser::_json::_exception::JSONKeyNotFoundException(name + " json field string has not been found");
}

jpl::_parser::_json::JSONFieldString* jpl::_parser::_json::JSONObject::getJSONFieldString(unsigned long index) const{
    if(index >= this->stringSize)
        throw new jpl::_exception::IndexOutOfBoundsException(this->fields.getSize(), index);
    size_t c = 0;
    for(size_t i = 0; i < this->fields.getSize(); i++){
        jpl::_parser::_json::JSONField* cr = this->fields.get(i);
        if( typeid(cr) == typeid(jpl::_parser::_json::JSONFieldString) ){
            if(c++ == index)
                return static_cast<jpl::_parser::_json::JSONFieldString*>(cr);
        }
    }  
    throw new jpl::_exception::RuntimeException("Altough index is into the range, there's no valid json field. This is a JPL bug, please report...");
}

void jpl::_parser::_json::JSONObject::addJSONObject(JSONObject* jsonObject){
    if(jsonObject == nullptr)
        throw new jpl::_exception::IllegalArgumentException("You cannot add null json object");
    this->jsonObjects.add(jsonObject);
}
void jpl::_parser::_json::JSONObject::addJSONArray(JSONArray* jsonArray){
    if(jsonArray == nullptr)
        throw new jpl::_exception::IllegalArgumentException("You cannot add null json array");
    this->jsonArrays.add(jsonArray);
}
void jpl::_parser::_json::JSONObject::addJSONField(JSONField* field){
    if(field == nullptr)
        throw new jpl::_exception::IllegalArgumentException("You cannot add null json field");
    this->fields.add(field);
}



std::string jpl::_parser::_json::JSONObject::toString() const noexcept{
    std::string buffer = std::string(this->tabs, '\t');
    if(!this->name.empty()){
        buffer += "\"" + this->name + "\":";
    }
    buffer += "{\n";
    for(size_t i = 0; i < this->jsonObjects.getSize(); i++){
        buffer += this->jsonObjects.get(i)->toString();
    }
    for(size_t i = 0; i < this->jsonArrays.getSize(); i++){
        buffer += this->jsonArrays.get(i)->toString();
    }
    for(size_t i = 0; i < this->fields.getSize(); i++){
        buffer += this->fields.get(i)->toString();
    }
    buffer += std::string(this->tabs, '\t') + "}";
    return buffer;
}