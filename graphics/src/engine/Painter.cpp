#include "Painter.hpp"

jpl::_graphics::_engine::Painter::Painter(){
    glGenBuffers(1, &this->VBO);
    glGenVertexArrays(1, &this->VAO);
    glGenBuffers(1, &this->EBO);
    this->x = 0.0f;
    this->y = 0.0f;
    this->z = 0.0f;
}

void jpl::_graphics::_engine::Painter::bindBuffer() const noexcept{
    glBindVertexArray(this->VAO);
    glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->EBO);
}

void jpl::_graphics::_engine::Painter::pushData(const float* data, unsigned int sizeData, GLenum mode) const noexcept{
    glBufferData(GL_ARRAY_BUFFER, sizeData, data, mode);
}
void jpl::_graphics::_engine::Painter::pushData(const jpl::_graphics::_mesh::Mesh* mesh, GLenum mode) const noexcept{
    glBindVertexArray(this->VAO);
    if(mesh->getIndices() != nullptr){
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, mesh->getSizeIndices()*sizeof(unsigned int), mesh->getIndices(), GL_STATIC_DRAW);
    }
    glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
    glBufferData(GL_ARRAY_BUFFER, mesh->getSizeVertices()*sizeof(float), mesh->getVertices(), mode);
    glVertexAttribPointer(
        0,
        3,   //It also means how many float values are referred to vertices
        GL_FLOAT,
        GL_FALSE,
        5*sizeof(float),
        (void*)0
    );
    glEnableVertexAttribArray(0);
    if(mesh->getOffsetTexture() > 0){
        glVertexAttribPointer(
            1,
            2, 
            GL_FLOAT,
            mesh->isNormalized(),
            5*sizeof(float),
            (void*)(3*sizeof(float))
        );
        glEnableVertexAttribArray(1);
    }
}

void jpl::_graphics::_engine::drawMesh(jpl::_graphics::_engine::Painter* painter, const jpl::_graphics::_mesh::Mesh* mesh){
    glBindVertexArray(painter->getVAO());
    glm::mat4 model(1.0f);
    if(painter->getModelMatrixLocation() > 0 && painter->isPosUpdated()){
        model = glm::translate(model, glm::vec3(painter->getX(), painter->getY(), painter->getZ()));
        glUniformMatrix4fv(painter->getModelMatrixLocation(), 1, GL_FALSE, glm::value_ptr(model));
        painter->setPosUpdated(false);
    }
    if(mesh->getIndices() != nullptr){
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, painter->getEBO());
        glDrawElements(GL_TRIANGLES, mesh->getSizeIndices(), GL_UNSIGNED_INT, 0);
    }else{
        glBindBuffer(GL_VERTEX_ARRAY, painter->getVBO());
        glDrawArrays(GL_TRIANGLES, 0, mesh->getSizeVertices()/mesh->getCoordsPerPoint());
    }
}