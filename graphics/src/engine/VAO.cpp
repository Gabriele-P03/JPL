#include "VAO.hpp"

jpl::_graphics::_engine::VAO::VAO(const unsigned int vao) : vao(vao){}

jpl::_graphics::_engine::VBO* const jpl::_graphics::_engine::VAO::addVBO(jpl::_graphics::_engine::VBO* const vbo){
    for( const auto &cr : this->VBOs)
        if(cr == vbo || cr->vbo == vbo->vbo)
            throw jpl::_exception::RuntimeException("VBO duplicate: registered (" + std::to_string(cr->vbo) + "), passed(" + std::to_string(vbo->vbo) + ")");
    this->VBOs.push_back(vbo);
    return const_cast<jpl::_graphics::_engine::VBO* const>(vbo);
}
jpl::_graphics::_engine::VBO* const jpl::_graphics::_engine::VAO::addVBO(const unsigned int vbo){
    return jpl::_graphics::_engine::VAO::addVBO(new jpl::_graphics::_engine::VBO(vbo));
}
jpl::_graphics::_engine::VBO* const jpl::_graphics::_engine::VAO::addVBO(){
    unsigned int vbo;
    glGenBuffers(1, &vbo);
    return this->addVBO(vbo);
}


jpl::_graphics::_engine::EBO* const jpl::_graphics::_engine::VAO::addEBO(jpl::_graphics::_engine::EBO* const ebo){
    for( const auto &cr : this->EBOs)
        if(cr == ebo || cr->ebo == ebo->ebo)
            throw jpl::_exception::RuntimeException("EBO" + std::to_string(cr->ebo) + "), passed(" + std::to_string(ebo->ebo) + ")");
    this->EBOs.push_back(ebo);
    return const_cast<jpl::_graphics::_engine::EBO* const>(ebo);
}
jpl::_graphics::_engine::EBO* const jpl::_graphics::_engine::VAO::addEBO(const unsigned int ebo){
    return jpl::_graphics::_engine::VAO::addEBO(new jpl::_graphics::_engine::EBO(ebo));
}
jpl::_graphics::_engine::EBO* const jpl::_graphics::_engine::VAO::addEBO(){
    unsigned int ebo;
    glGenBuffers(1, &ebo);
    return this->addEBO(ebo);
}


jpl::_graphics::_engine::VAO::~VAO(){
    this->VBOs.clear();
    this->EBOs.clear();
}