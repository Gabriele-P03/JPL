#include "Shader.hpp"

jpl::_graphics::_shaders::Shader::Shader(std::fstream* s){
    char* buffer;
    unsigned long size;
    jpl::_utils::_files::readFile(s, buffer, size);
    this->rawProgram = const_cast<const char*>(buffer);
    this->sizeRawProgram = (int)size;
    this->compiled = false;
}

jpl::_graphics::_shaders::Shader::Shader(const char* rawProgram, int sizeRawProgram){
    this->rawProgram = rawProgram;
    this->sizeRawProgram = sizeRawProgram;
    this->compiled = false;
}

jpl::_graphics::_shaders::Shader::~Shader(){
    delete this->rawProgram;
    glDeleteShader(this->shaderIndex);
}

void jpl::_graphics::_shaders::Shader::probeShader(int shaderType){
    this->shaderIndex = glCreateShader(shaderType);
    if(this->shaderIndex == 0){ //it may throw GL_INVALID_ENUM due to invalid shaderType
        throw jpl::_exception::RuntimeException(jpl::_graphics::_error::getLastErrorAsString());
    }
    this->shaderType = shaderType;
    glShaderSource(this->shaderIndex, 1, &this->rawProgram, &this->sizeRawProgram);//Now let's attach source code
    jpl::_graphics::_error::GLFWErrorJPL* er;
    std::string errStr = jpl::_graphics::_error::getLastErrorAsString(&er);
    if(er->errorCode != 0){
        throw jpl::_exception::RuntimeException("THe shader source code could not be attached to program: " + errStr);
    }
    delete er;
    glCompileShader(this->shaderIndex);
    errStr = jpl::_graphics::_error::getLastErrorAsString(&er);
    if(er->errorCode != 0){
        throw jpl::_exception::RuntimeException("The shader program could not be compiled: " + errStr);
    }
    delete er;
    this->compiled = true;
}

unsigned int jpl::_graphics::_shaders::Shader::getShaderType() const{
    if(!this->compiled){
        throw jpl::_exception::IllegalStateException("Shader program has not been compiled yet");
    }
    return this->shaderType;
}

unsigned int jpl::_graphics::_shaders::getShaderIndexByShader(jpl::_graphics::_shaders::Shader* shader){
    if(shader == nullptr){
        throw jpl::_exception::IllegalArgumentException("Shader is nullptr");
    }
    if(!shader->isCompiled()){
        throw jpl::_exception::IllegalStateException("Shader has not been created, yet. Please call probeShader(shaderType) before!");
    }
    return shader->getShaderIndexProgram();
}