#include "ProgressBar.hpp"

jpl::_graphics::_engine::ProgressBar::ProgressBar(
    jpl::_graphics::_engine::Painter* painter, jpl::_graphics::_engine::_text::TextRender* textRender, 
    jpl::_graphics::_texture::Texture* background, float max){

    if(painter == nullptr){
        throw jpl::_exception::IllegalArgumentException("Painter is nullptr");
    }
    if(background == nullptr){
        throw jpl::_exception::IllegalArgumentException("Texture is nullptr");
    }    
    this->painter = painter;
    this->textRender = textRender;
    this->background = background;
    this->max = max;
    this->progress = 0;
}

void jpl::_graphics::_engine::ProgressBar::render() const noexcept{
    glActiveTexture(GL_TEXTURE0);
    this->background->bind();
    glBindVertexArray(painter->getVAO());
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->painter->getEBO());
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(this->indices), indices, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, this->painter->getVBO());
    glBufferData(GL_ARRAY_BUFFER, sizeof(this->coords), this->coords, GL_STATIC_DRAW);    //Empty part
    glVertexAttribPointer(
        0,
        3,   //It also means how many float values are referred to vertices
        GL_FLOAT,
        GL_FALSE,
        5*sizeof(float),
        (void*)0
    );
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(
        1,
        2, 
        GL_FLOAT,
        GL_FALSE,
        5*sizeof(float),
        (void*)(3*sizeof(float))
    );
    glEnableVertexAttribArray(1);
    glDrawElements(GL_TRIANGLES, 12, GL_UNSIGNED_INT, 0);
}