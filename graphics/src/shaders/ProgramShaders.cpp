#include "ProgramShaders.hpp"

jpl::_graphics::_shaders::ProgramShaders::ProgramShaders()
    : jpl::_graphics::_shaders::ProgramShaders::ProgramShaders(glCreateProgram()){
}

jpl::_graphics::_shaders::ProgramShaders::ProgramShaders(unsigned int program) : program(program){
    if(program == 0){
        throw jpl::_exception::RuntimeException(jpl::_graphics::_error::getLastErrorAsString());
    }
    jpl::_logger::info("New ProgramShader generated: " + std::to_string(this->program));
}

jpl::_graphics::_shaders::ProgramShaders::~ProgramShaders(){
    this->shaders.clear();
    glDeleteProgram(this->program);
}

void jpl::_graphics::_shaders::ProgramShaders::addNewShader(const std::string &identifier, jpl::_graphics::_shaders::Shader* shader){
    if (shader == nullptr){
        throw jpl::_exception::IllegalArgumentException("Shader is nullptr");
    }
    typename std::unordered_map<std::string, jpl::_graphics::_shaders::Shader* const>::iterator it = this->shaders.find(identifier);
    if(it != this->shaders.end()){
        throw jpl::_exception::IllegalArgumentException("This shader pointer has been already inserted");
    }
    glAttachShader(this->program, jpl::_graphics::_shaders::getShaderIndexByShader(shader));
    jpl::_graphics::_error::GLFWErrorJPL* er;
    std::string errStr = jpl::_graphics::_error::getLastErrorAsString(&er);
    if(er->errorCode != 0){
        throw jpl::_exception::RuntimeException("Error occurred during attaching shader by ProgramShader: " + errStr);
    }
    this->shaders.insert({identifier, shader});
    jpl::_logger::info("ProgramShader " + std::to_string(this->program) + ": new shader attached " + identifier);
}

jpl::_graphics::_shaders::Shader* jpl::_graphics::_shaders::ProgramShaders::getShaderByIdentifier(const std::string &identifier) const{
    typename std::unordered_map<std::string, jpl::_graphics::_shaders::Shader* const>::const_iterator it = this->shaders.find(identifier);
    if(it == this->shaders.end()){
        throw jpl::_exception::IllegalArgumentException("No shader " + identifier + " has been registered, yet");
    }
    return it->second;
}

void jpl::_graphics::_shaders::ProgramShaders::detachShader(const std::string &identifier){
    typename std::unordered_map<std::string, jpl::_graphics::_shaders::Shader* const>::iterator it = this->shaders.find(identifier);
    if(it == this->shaders.end()){
        throw jpl::_exception::RuntimeException("There's no shader " + identifier);
    }
    glDetachShader(this->program, jpl::_graphics::_shaders::getShaderIndexByShader(it->second));
    this->shaders.erase(it);
}

void jpl::_graphics::_shaders::ProgramShaders::link() const{
    jpl::_logger::info("Linking ProgramShader " + std::to_string(this->program));
    glLinkProgram(this->program);
}

void jpl::_graphics::_shaders::ProgramShaders::use() const{
    glUseProgram(this->program);
}