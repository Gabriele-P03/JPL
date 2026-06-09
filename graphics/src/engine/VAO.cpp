#include "VAO.hpp"

void jpl::_graphics::_engine::VAO::addVBO(jpl::_graphics::_engine::VBO* vbo){
    if(vbo == nullptr){
        throw jpl::_exception::IllegalArgumentException("VBO is nullptr");
    }
    if( this->getVBOByName(vbo->name) != nullptr){
        throw jpl::_exception::RuntimeException("There's already a VBO registered with " + name);
    }
    this->VBOs.push_back(vbo);
}

jpl::_graphics::_engine::VBO* jpl::_graphics::_engine::VAO::addVBO(const std::string &name){
    unsigned int vbo;
    glGenBuffers(1, &vbo);
    return this->addVBO(name, vbo);
}

jpl::_graphics::_engine::VBO* jpl::_graphics::_engine::VAO::addVBO(const std::string &name, const unsigned int vbo){
    jpl::_graphics::_engine::VBO* vbo_ptr = new jpl::_graphics::_engine::VBO(name, vbo);
    this->addVBO(vbo_ptr);
    return vbo_ptr;
}

jpl::_graphics::_engine::VBO* jpl::_graphics::_engine::VAO::getVBOByName(const std::string &name) noexcept{
    for(long i = 0; i < this->VBOs.size(); i++){
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
        throw jpl::_exception::RuntimeException("There's already an EBO registered with " + name);
    }
    this->EBOs.push_back(ebo);
}


jpl::_graphics::_engine::EBO* jpl::_graphics::_engine::VAO::addEBO(const std::string &name){
    unsigned int ebo;
    glGenBuffers(1, &ebo);
    return this->addEBO(name, ebo);
}
jpl::_graphics::_engine::EBO* jpl::_graphics::_engine::VAO::addEBO(const std::string &name, const unsigned int ebo){
    jpl::_graphics::_engine::EBO* ebo_ptr = new jpl::_graphics::_engine::EBO(name, ebo);
    this->addEBO(ebo_ptr);
    return ebo_ptr;
}


jpl::_graphics::_engine::EBO* jpl::_graphics::_engine::VAO::getEBOByName(const std::string &name) noexcept{
    for(long i = 0; i < this->EBOs.size(); i++){
        jpl::_graphics::_engine::EBO* cr = this->EBOs.at(i);
        if( strcmp(cr->name.c_str(), name.c_str()) == 0){
            return cr;
        } 
    }
    return nullptr;
}