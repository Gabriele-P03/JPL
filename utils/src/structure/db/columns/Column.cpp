#include "Column.hpp"


template<> std::string jpl::_utils::_collections::Column<std::string>::getValue(jpl::_utils::_collections::Tuple* tuple) const{
    return tuple->getValue(this->index);
};
template<> void jpl::_utils::_collections::Column<std::string>::setValue(jpl::_utils::_collections::Tuple* tuple, std::string value) const{
    tuple->setValue(this->getIndex()-1, value);
}


template<> bool jpl::_utils::_collections::Column<bool>::getValue(jpl::_utils::_collections::Tuple* tuple) const{
    std::string cr = tuple->getValue(this->index);
    if(cr.compare("0") || cr.compare("false"))
        return false;
    else if(cr.compare("1") || cr.compare("true"))
        return true;
    throw jpl::_exception::RuntimeException("Column " + this->name + " contains a non boolean value: " + cr);
};
template<> void jpl::_utils::_collections::Column<bool>::setValue(jpl::_utils::_collections::Tuple* tuple, bool value) const{
    tuple->setValue(this->getIndex()-1, value ? "1" : "0");
}


template<> double jpl::_utils::_collections::Column<double>::getValue(jpl::_utils::_collections::Tuple* tuple) const{
    std::string cr = tuple->getValue(this->index);
    try{
        return std::atof(cr.c_str());
    }catch(std::exception* ex){
        throw jpl::_exception::IllegalCastException(cr + " is not a valid double value");
    }
};
template<> void jpl::_utils::_collections::Column<double>::setValue(jpl::_utils::_collections::Tuple* tuple, double value) const{
    tuple->setValue(this->getIndex()-1, std::to_string(value));
}


template<> long jpl::_utils::_collections::Column<long>::getValue(jpl::_utils::_collections::Tuple* tuple) const{
    std::string cr = tuple->getValue(this->index);
    try{
        return std::atol(cr.c_str());
    }catch(std::exception* ex){
        throw jpl::_exception::IllegalCastException(cr + " is not a valid long value");
    }
};
template<> void jpl::_utils::_collections::Column<long>::setValue(jpl::_utils::_collections::Tuple* tuple, long value) const{
    tuple->setValue(this->getIndex()-1, std::to_string(value));
}


template<> int jpl::_utils::_collections::Column<int>::getValue(jpl::_utils::_collections::Tuple* tuple) const{
    std::string cr = tuple->getValue(this->index);
    try{
        return std::atoi(cr.c_str());
    }catch(std::exception* ex){
        throw jpl::_exception::IllegalCastException(cr + " is not a valid integer value");
    }
};
template<> void jpl::_utils::_collections::Column<int>::setValue(jpl::_utils::_collections::Tuple* tuple, int value) const{
    tuple->setValue(this->getIndex()-1, std::to_string(value));
}


template<> const char* jpl::_utils::_collections::Column<const char*>::getValue(jpl::_utils::_collections::Tuple* tuple) const{ //BLOB
    std::string cr = tuple->getValue(this->index);
    const char* buffer = new char[cr.size()+1];
    char* tmp = (char*)&buffer[0];
    memcpy(tmp, cr.c_str(), cr.size());
    tmp[cr.size()] = '\0';
    return buffer;
};
template<> void jpl::_utils::_collections::Column<const char*>::setValue(jpl::_utils::_collections::Tuple* tuple, const char* value) const{
    tuple->setValue(this->getIndex()-1, std::string(value));
}