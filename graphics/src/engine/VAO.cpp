#include "VAO.hpp"

void jpl::_graphics::_engine::VAO::addVBO(jpl::_graphics::_engine::VBO* vbo){
    if(vbo == nullptr){
        throw jpl::_exception::IllegalArgumentException("VBO is nullptr");
    }
    if( this->getVBOByName(vbo->name) != nullptr){
        throw jpl::_exception::RuntimeException("There's already a VAO registered with " + name);
    }
    this->VBOs.push_back(vbo);
}

void jpl::_graphics::_engine::VAO::addVBO(const std::string &name, const unsigned int vbo){
    this->addVBO(new jpl::_graphics::_engine::VBO(name, vbo));
}

jpl::_graphics::_engine::VBO* jpl::_graphics::_engine::VAO::getVBOByName(const std::string &name) noexcept{
    for(unsigned long i = 0; i <= this->VBOs.size(); i++){
        jpl::_graphics::_engine::VBO* cr = this->VBOs.at(i);
        if( strcmp(cr->name.c_str(), name.c_str()) == 0){
            return cr;
        } 
    }
    return nullptr;
}

void jpl::_graphics::_engine::VAO::addEBO(jpl::_graphics::_engine::EBO* ebo){
    if(ebo == nullptr){
        throw jpl::_exception::IllegalArgumentException("EBO is nullptr");
    }
    if( this->getEBOByName(ebo->name) != nullptr){
        throw jpl::_exception::RuntimeException("There's already a VAO registered with " + name);
    }
    this->EBOs.push_back(ebo);
}

void jpl::_graphics::_engine::VAO::addEBO(const std::string &name, const unsigned int vbo){
    this->addEBO(new jpl::_graphics::_engine::EBO(name, vbo));
}


jpl::_graphics::_engine::EBO* jpl::_graphics::_engine::VAO::getEBOByName(const std::string &name) noexcept{
    for(unsigned long i = 0; i <= this->EBOs.size(); i++){
        jpl::_graphics::_engine::EBO* cr = this->EBOs.at(i);
        if( strcmp(cr->name.c_str(), name.c_str()) == 0){
            return cr;
        } 
    }
    return nullptr;
}