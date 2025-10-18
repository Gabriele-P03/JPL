#include "ProgramManager.hpp"

jpl::_graphics::_shaders::ProgramManager::ProgramManager(){
    this->programShaders = new jpl::_utils::_collections::_list::LinkedList<jpl::_graphics::_shaders::ProgramShaders*>();
}

jpl::_graphics::_shaders::ProgramManager::~ProgramManager(){
    delete this->programShaders;
}

jpl::_graphics::_shaders::ProgramShaders* jpl::_graphics::_shaders::ProgramManager::getProgramShadersByProgram(unsigned int program){
    if(program == 0){
        throw jpl::_exception::IllegalArgumentException("Shader Program 0 is not a valid value");
    }
    for(size_t i = 0; i < this->programShaders->getSize(); i++){
        jpl::_graphics::_shaders::ProgramShaders* cr = this->programShaders->get(i);
        if(cr->getProgramIndex() == program){
            return cr;
        }
    }
    return nullptr;
}

void jpl::_graphics::_shaders::ProgramManager::addProgramShaders(jpl::_graphics::_shaders::ProgramShaders* programShaders){
    if(programShaders == nullptr){
        throw jpl::_exception::IllegalArgumentException("programShaders is nullptr");
    }
    for(size_t i = 0; i < this->programShaders->getSize(); i++){
        jpl::_graphics::_shaders::ProgramShaders* cr = this->programShaders->get(i);
        if(cr == programShaders){
            throw jpl::_exception::IllegalArgumentException("Given programShaders has been already inserted");
        }
        if(cr->getProgramIndex() == programShaders->getProgramIndex()){
            throw jpl::_exception::IllegalArgumentException("programShaders with " + std::to_string(cr->getProgramIndex()) + " has been already inserted");
        }
    }
    this->programShaders->add(programShaders);
}

void jpl::_graphics::_shaders::ProgramManager::addProgramShader(unsigned int program, jpl::_graphics::_shaders::Shader* shader){
    if(program == 0){
        throw jpl::_exception::IllegalArgumentException("Shader Program 0 is not a valid value");
    }
    jpl::_graphics::_shaders::ProgramShaders* cr = this->getProgramShadersByProgram(program);
    if(cr == nullptr){
        cr = new jpl::_graphics::_shaders::ProgramShaders(program);
        this->programShaders->add(cr);
    }
    cr->addNewShader(shader);
}