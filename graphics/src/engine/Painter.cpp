#include "Painter.hpp"

jpl::_graphics::_engine::Painter::Painter(){
}


void jpl::_graphics::_engine::Painter::pushData(const jpl::_graphics::_mesh::Mesh* mesh, GLenum mode){
    glBufferData(GL_ARRAY_BUFFER, mesh->getSizeVertices()*sizeof(float), mesh->getVertices(), mode);
    if(mesh->getIndices() != nullptr){
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, mesh->getSizeIndices()*sizeof(unsigned int), mesh->getIndices(), GL_STATIC_DRAW);
    }
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
            mesh->getCoordsPerPoint() *sizeof(float),
            (void*)(mesh->getOffsetTexture()*sizeof(float))
        );
        glEnableVertexAttribArray(1);
        this->sizeIndices = mesh->getSizeIndices();
    }else{
        this->sizeIndices = 0;
        this->valuesPerPoint =  mesh->getSizeVertices()/mesh->getCoordsPerPoint();
    }
}

void jpl::_graphics::_engine::Painter::render(jpl::_graphics::_texture::Texture* texture, float x, float y, float w, float h) const{
    glActiveTexture(GL_TEXTURE0);
    texture->bind();
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(x, y, 0.0f));
    model = glm::scale(model, glm::vec3(w, h, 1.0f));
    model = glm::translate(model, glm::vec3(0.5f, 0.5f, 0.0f));
    glUniformMatrix4fv(2, 1, GL_FALSE, glm::value_ptr(model));
    glUniformMatrix4fv(4, 1, GL_FALSE, glm::value_ptr(jpl::_graphics::_metrics::ortho));
    if(this->sizeIndices > 0){
        glDrawElements(GL_TRIANGLES, this->sizeIndices, GL_UNSIGNED_INT, 0);
    }else{
        glDrawArrays(GL_TRIANGLES, 0, this->valuesPerPoint);
    }
}