#include "BufferManager.hpp"

jpl::_sounds::BufferManager::BufferManager(){

}

void jpl::_sounds::BufferManager::addBuffer(jpl::_sounds::Buffer* buffer){
    if(buffer == nullptr){
        throw jpl::_exception::IllegalArgumentException("Buffer is nullptr");
    }
    if(this->buffers.contains(buffer->getRegName())){
        throw jpl::_exception::IllegalArgumentException("Buffer " + buffer->getRegName() + " already exists");
    }
    this->buffers.insert( std::make_pair(buffer->getRegName(), buffer));
}

jpl::_sounds::Buffer* jpl::_sounds::BufferManager::genBuffer(const std::string &regName){
    if(regName.empty()){
        throw jpl::_exception::IllegalArgumentException("regName cannot be empty string");
    }
    jpl::_sounds::Buffer* b = new jpl::_sounds::Buffer(regName);
    this->addBuffer(b);
    return b;
}

jpl::_sounds::Buffer* jpl::_sounds::BufferManager::getBufferByRegName(const std::string &regName) const noexcept{
    try{
        return this->buffers.at(regName);
    }catch(const std::out_of_range &e){
        return nullptr;
    }
}