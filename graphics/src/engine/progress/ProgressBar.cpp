#include "ProgressBar.hpp"

jpl::_graphics::_engine::ProgressBar::ProgressBar(
    jpl::_graphics::_engine::Painter* painter, jpl::_graphics::_engine::_text::TextRender* textRender, 
    jpl::_graphics::_texture::Texture* background, float max, unsigned int xSep, unsigned int ySep, unsigned int w, unsigned int h){

    if(painter == nullptr){
        throw jpl::_exception::IllegalArgumentException("Painter is nullptr");
    }
    if(texture == nullptr){
        throw jpl::_exception::IllegalArgumentException("Texture is nullptr");
    }    
    this->painter = painter;
    this->textRender = textRender;
    this->background = background;
    this->max = max;
    this->xSep = xSep;
    this->ySep = ySep;
    this->w = w;
    this->h = h;
    this->progress = 0;
    this->setCoordsArray();
}

void jpl::_graphics::_engine::ProgressBar::render(){
    glActiveTexture(GL_TEXTURE0);
    this->background->bind();
    glBindVertexArray(painter->getVAO());
    glBindBuffer(GL_ARRAY_BUFFER, this->painter->getVBO());
    glBufferData(GL_ARRAY_BUFFER, sizeof(this->emptyCoords)*sizeof(float), &this->emptyCoords, GL_DYNAMIC_DRAW);    //Empty part
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
    glDrawArrays(GL_TRIANGLES, 0, 4);
    glBufferData(GL_ARRAY_BUFFER, sizeof(this->filledCoords)*sizeof(float), &this->filledCoords, GL_DYNAMIC_DRAW);  //Filled part
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
    glDrawArrays(GL_TRIANGLES, 0, 4);
}