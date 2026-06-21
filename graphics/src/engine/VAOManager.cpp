#include "VAOManager.hpp"

jpl::_graphics::_engine::VAOManager* jpl::_graphics::_engine::VAOManager::INSTANCE = nullptr;

jpl::_graphics::_engine::VAOManager::VAOManager(){}

jpl::_graphics::_engine::VAO* const jpl::_graphics::_engine::VAOManager::getVAOByName(const std::string &identifier) const noexcept{
    typename std::unordered_map<std::string, jpl::_graphics::_engine::VAO* const>::const_iterator it = this->vaos.find(identifier);
    if(it != this->vaos.end()){
        return it->second;
    }
    return nullptr;
}

jpl::_graphics::_engine::VAO* const jpl::_graphics::_engine::VAOManager::addNewVAO(const std::string &identifier, jpl::_graphics::_engine::VAO* const vao){
    if(vao == nullptr){
        throw jpl::_exception::IllegalArgumentException("VAO is nullptr");
    }
    if( this->getVAOByName(identifier) != nullptr){
        throw jpl::_exception::RuntimeException("There's already a VAO registered with " + identifier);
    }
    this->vaos.insert({identifier, vao});
    return vao;
}
jpl::_graphics::_engine::VAO* const jpl::_graphics::_engine::VAOManager::addNewVAO(const std::string &identifier, const unsigned int VAO){
    return this->addNewVAO(identifier, new jpl::_graphics::_engine::VAO(VAO));
}
jpl::_graphics::_engine::VAO* const jpl::_graphics::_engine::VAOManager::addNewVAO(const std::string &identifier){
    unsigned int vao;
    glGenVertexArrays(1, &vao);
    return this->addNewVAO(identifier, vao);
}


void jpl::_graphics::_engine::VAOManager::removeVAOByName(const std::string &identifier){
    typename std::unordered_map<std::string, jpl::_graphics::_engine::VAO* const>::iterator it = this->vaos.find(identifier);
    if(it != this->vaos.end()){
        this->vaos.erase(it);
    }
    throw jpl::_exception::IllegalArgumentException("No VAO with " + identifier + " has been found");
}

void jpl::_graphics::_engine::VAOManager::initializeVAOManager(){
    jpl::_graphics::_engine::VAOManager::INSTANCE = new jpl::_graphics::_engine::VAOManager();
}

jpl::_graphics::_engine::VAOManager::~VAOManager(){
    this->vaos.clear();
}
