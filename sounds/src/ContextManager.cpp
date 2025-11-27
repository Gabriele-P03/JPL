#include "ContextManager.hpp"

jpl::_sounds::ContextManager::ContextManager(ALCdevice* device, ALCint* attrlist){
    if(device == nullptr){
        throw jpl::_exception::IllegalArgumentException("Device is nullptr");
    }
    this->device = device;
    this->context = alcCreateContext(this->device, attrlist);
    if(this->context == nullptr){
        throw jpl::_exception::RuntimeException("Context could not be created");
    }
}

void jpl::_sounds::ContextManager::addSource(jpl::_sounds::Source* source){
    if(source == nullptr){
        throw jpl::_exception::IllegalArgumentException("Source is nullptr");
    }
    if(this->sources.contains(source->getRegName())){
        throw jpl::_exception::IllegalArgumentException("Source " + source->getRegName() + " already exists");
    }
    this->sources.insert( std::make_pair(source->getRegName(), source));
}

jpl::_sounds::Source* jpl::_sounds::ContextManager::genSource(const std::string &regName){
    if(regName.empty()){
        throw jpl::_exception::IllegalArgumentException("regName cannot be empty string");
    }
    jpl::_sounds::Source* s = new jpl::_sounds::Source(regName);
    this->sources.insert(std::make_pair(regName, s));
    return s;
}

jpl::_sounds::Source* jpl::_sounds::ContextManager::getSourceByRegName(const std::string &regName) const noexcept{
    try{
        return this->sources.at(regName);
    }catch(const std::out_of_range &e){
        return nullptr;
    }
}

jpl::_sounds::ContextManager::~ContextManager(){
    alcMakeContextCurrent(NULL);
    alcDestroyContext(this->context);
    alcCloseDevice(this->device);
}