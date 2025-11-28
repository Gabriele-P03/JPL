#include "ProgramManager.hpp"

jpl::_graphics::_shaders::ProgramManager::ProgramManager(){
    this->programShaders = new jpl::_utils::_collections::_list::LinkedList<std::tuple<std::string, jpl::_graphics::_shaders::ProgramShaders*>>();
}

jpl::_graphics::_shaders::ProgramManager::~ProgramManager(){
    delete this->programShaders;
}

jpl::_graphics::_shaders::ProgramShaders* jpl::_graphics::_shaders::ProgramManager::getProgramShadersByProgram(unsigned int program){
    if(program == 0){
        throw jpl::_exception::IllegalArgumentException("Shader Program 0 is not a valid value");
    }
    for(size_t i = 0; i < this->programShaders->getSize(); i++){
        jpl::_graphics::_shaders::ProgramShaders* cr = std::get<1>(this->programShaders->get(i));
        if(cr->getProgramIndex() == program){
            return cr;
        }
    }
    throw jpl::_exception::NotFoundException("Program Shader with " + std::to_string(program) + " index not found");
}

jpl::_graphics::_shaders::ProgramShaders* jpl::_graphics::_shaders::ProgramManager::getProgramShadersByIdentifier(const std::string &identifier){
    for(size_t i = 0; i < this->programShaders->getSize(); i++){
        std::tuple<std::string, jpl::_graphics::_shaders::ProgramShaders*> t = this->programShaders->get(i);
        if( strcmp( std::get<0>(t).c_str(), identifier.c_str() ) == 0){
            return std::get<1>(t);
        }
    }
    throw jpl::_exception::NotFoundException("Program Shader with " + identifier + " identifier not found");
}

void jpl::_graphics::_shaders::ProgramManager::addProgramShaders(const std::string &identifier, jpl::_graphics::_shaders::ProgramShaders* programShaders){
    if(programShaders == nullptr){
        throw jpl::_exception::IllegalArgumentException("programShaders is nullptr");
    }
    for(size_t i = 0; i < this->programShaders->getSize(); i++){
        std::tuple<std::string, jpl::_graphics::_shaders::ProgramShaders*> t = this->programShaders->get(i);
        if( strcmp( std::get<0>(t).c_str(), identifier.c_str() ) == 0){
            jpl::_graphics::_shaders::ProgramShaders* cr = std::get<1>(t);
            if(cr == programShaders){
                throw jpl::_exception::IllegalArgumentException("Given programShaders has been already inserted");
            }
            if(cr->getProgramIndex() == programShaders->getProgramIndex()){
                throw jpl::_exception::IllegalArgumentException("programShaders with " + std::to_string(cr->getProgramIndex()) + " has been already inserted");
            }
        }
    }
    this->programShaders->add(std::make_tuple(identifier, programShaders));
}