#include "VAOManager.hpp"

jpl::_graphics::_engine::VAO* jpl::_graphics::_engine::VAOManager::addNewVAO(const std::string &name){
    unsigned int i;
    glGenVertexArrays(1, &i);
    jpl::_graphics::_engine::VAO* vao = new jpl::_graphics::_engine::VAO(name, i);
    this->addNewVAO(vao);
}

void jpl::_graphics::_engine::VAOManager::addNewVAO(jpl::_graphics::_engine::VAO* vao){
    if(vao == nullptr){
        throw jpl::_exception::IllegalArgumentException("VAO is nullptr");
    }
    if( this->getVAOByName(vao->name) != nullptr){
        throw jpl::_exception::RuntimeException("There's already a VAO registered with " + vao->name);
    }
    this->list.push_back(vao);
}
void jpl::_graphics::_engine::VAOManager::addNewVAO(const std::string &name, unsigned int VAO){
    this->addNewVAO(new jpl::_graphics::_engine::VAO(name, VAO));
}

jpl::_graphics::_engine::VAO* jpl::_graphics::_engine::VAOManager::getVAOByName(const std::string &name) const noexcept{
    for(unsigned long i = 0; i <= this->list.size(); i++){
        jpl::_graphics::_engine::VAO* cr = this->list.at(i);
        if( strcmp(cr->name.c_str(), name.c_str()) == 0){
            return cr;
        } 
    }
    return nullptr;
}