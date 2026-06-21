#include "ProgramManager.hpp"

jpl::_graphics::_shaders::ProgramManager* jpl::_graphics::_shaders::ProgramManager::INSTANCE = nullptr;

jpl::_graphics::_shaders::ProgramManager::ProgramManager(){
}

jpl::_graphics::_shaders::ProgramManager::~ProgramManager(){
    this->programShaders.clear();
}

jpl::_graphics::_shaders::ProgramShaders* const jpl::_graphics::_shaders::ProgramManager::getProgramShadersByProgram(unsigned int program) const{
    if(program == 0){
        throw jpl::_exception::IllegalArgumentException("Shader Program 0 is not a valid value");
    }
    for (const auto& pair : this->programShaders){
        if(pair.second->getProgramIndex() == program){
            return pair.second;
        }
    }

    throw jpl::_exception::NotFoundException("Program Shader with " + std::to_string(program) + " index not found");
}

jpl::_graphics::_shaders::ProgramShaders* const jpl::_graphics::_shaders::ProgramManager::getProgramShadersByIdentifier(const std::string &identifier) const {
    typename std::unordered_map<std::string, jpl::_graphics::_shaders::ProgramShaders* const>::const_iterator it = this->programShaders.find(identifier);
    if(it != this->programShaders.end()){
        return it->second;
    }
    throw jpl::_exception::NotFoundException("Program Shader with " + identifier + " identifier not found");
}

void jpl::_graphics::_shaders::ProgramManager::addProgramShaders(const std::string &identifier, jpl::_graphics::_shaders::ProgramShaders* const programShaders){
    if(programShaders == nullptr){
        throw jpl::_exception::IllegalArgumentException("programShaders is nullptr");
    }
    for (const auto& pair : this->programShaders){
        if( strcmp( pair.first.c_str(), identifier.c_str() ) == 0){
            throw jpl::_exception::IllegalArgumentException("ProgramShaders " + identifier + " has been already inserted");
        }
        if(pair.second == programShaders){
            throw jpl::_exception::IllegalArgumentException("Given programShaders has been already inserted");
        }
    }
    this->programShaders.insert({identifier, programShaders});
}

void jpl::_graphics::_shaders::ProgramManager::removeProgramShaders(const std::string &identifier){
    typename std::unordered_map<std::string, jpl::_graphics::_shaders::ProgramShaders* const>::iterator it = this->programShaders.find(identifier);
    if(it != this->programShaders.end()){
        this->programShaders.erase(it);
    }
    throw jpl::_exception::NotFoundException("Program Shader with " + identifier + " identifier not found");
}

void jpl::_graphics::_shaders::ProgramManager::initializeProgramManager(){
    jpl::_graphics::_shaders::ProgramManager::INSTANCE = new jpl::_graphics::_shaders::ProgramManager();
}