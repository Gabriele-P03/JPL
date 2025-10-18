#include "ProgramShaders.hpp"

jpl::_graphics::_shaders::ProgramShaders::ProgramShaders()
    : jpl::_graphics::_shaders::ProgramShaders::ProgramShaders(glCreateProgram()){
}

jpl::_graphics::_shaders::ProgramShaders::ProgramShaders(unsigned int program) : program(program){
    if(program == 0){
        throw jpl::_exception::RuntimeException(jpl::_graphics::_error::getLastErrorAsString());
    }
    this->shaders = new jpl::_utils::_collections::_list::LinkedList<jpl::_graphics::_shaders::Shader*>();
}

jpl::_graphics::_shaders::ProgramShaders::~ProgramShaders(){
    delete this->shaders;
    glDeleteProgram(this->program);
}

void jpl::_graphics::_shaders::ProgramShaders::addNewShader(jpl::_graphics::_shaders::Shader* shader){
    if (shader == nullptr){
        throw jpl::_exception::IllegalArgumentException("Shader is nullptr");
    }
    for(size_t i = 0; i < this->shaders->getSize(); i++){
        jpl::_graphics::_shaders::Shader* cr = this->shaders->get(i);
        if(cr == shader){
            throw jpl::_exception::IllegalArgumentException("This shader pointer has been already inserted");
        }
    }
    glAttachShader(this->program, jpl::_graphics::_shaders::getShaderIndexByShader(shader));
    jpl::_graphics::_error::GLFWErrorJPL* er;
    std::string errStr = jpl::_graphics::_error::getLastErrorAsString(&er);
    if(er->errorCode != 0){
        throw jpl::_exception::RuntimeException("Error occurred during attaching shader by ProgramShader: " + errStr);
    }
    this->shaders->add(shader);
}

void jpl::_graphics::_shaders::ProgramShaders::detachShader(jpl::_graphics::_shaders::Shader* shader){
    if(shader == nullptr){
        throw jpl::_exception::IllegalArgumentException("Shader is nullptr");
    }
    this->detachShader(jpl::_graphics::_shaders::getShaderIndexByShader(shader));
}

void jpl::_graphics::_shaders::ProgramShaders::detachShader(unsigned int shaderIndex){
    if(shaderIndex == 0){
        throw jpl::_exception::IllegalArgumentException("Shader Index not valid: " + std::to_string(shaderIndex));
    }
    size_t pos = 0;
    bool flagFound = false; 
    for(size_t i = 0; i <= this->shaders->getSize(); i++){
        jpl::_graphics::_shaders::Shader* cr = this->shaders->get(i);
        if(shaderIndex == jpl::_graphics::_shaders::getShaderIndexByShader(cr)){
            pos = i;
            flagFound = true;
            glDetachShader(this->program, jpl::_graphics::_shaders::getShaderIndexByShader(cr));
            break;
        }
    }
    if(!flagFound){
        throw jpl::_exception::RuntimeException("Sahder with index " + std::to_string(shaderIndex) + " is not present in list");
    }
    this->shaders->removeAt(pos);
}