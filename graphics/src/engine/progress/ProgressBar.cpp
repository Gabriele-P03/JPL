#include "ProgressBar.hpp"

jpl::_graphics::_engine::ProgressBar::ProgressBar(
    jpl::_graphics::_texture::Texture* background, float max){
    if(background == nullptr){
        throw jpl::_exception::IllegalArgumentException("Texture is nullptr");
    }
    this->background = background;
    this->max = max;
    this->progress = 0;
}

void jpl::_graphics::_engine::ProgressBar::bind() const noexcept{
    float coords[40] = {
        //Empty part
        -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, //Bottom-left
        0.5f, 0.5f, 0.0f, 1.0f, 0.5f,   //Top-right
        -0.5, 0.5f, 0.0f, 0.0f, 0.5f,   //Top-Left
        0.5f, -0.5f, 0.0f, 1.0f, 0.0f,   //Bottom-right
        //Filled part
        -0.5f, -0.5f, 0.0f, 0.0f, 0.5f, //Bottom-left
        -0.5f, 0.5f, 0.0f, 1.0f, 1.0f,   //Top-right
        -0.5, 0.5f, 0.0f, 0.0f, 1.0f,   //Top-Left
        -0.5f, -0.5f, 0.0f, 1.0f, 0.5f   //Bottom-right
    };
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(this->indices), this->indices, GL_STATIC_DRAW);
    glBufferData(GL_ARRAY_BUFFER, sizeof(coords), coords, GL_STATIC_DRAW);
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
}

void jpl::_graphics::_engine::ProgressBar::setProgress(float i){
    if(i < 0){
        throw jpl::_exception::IllegalArgumentException("progress " + std::to_string(i) + " is less than 0");
    }
    if(i > this->max){
        throw jpl::_exception::IllegalArgumentException("progress " + std::to_string(i) + " is less than max " + std::to_string(this->max));
    }
    this->progress = i;
    glBufferSubData(GL_ARRAY_BUFFER, 28*sizeof(float), sizeof(float), &i);
    glBufferSubData(GL_ARRAY_BUFFER, 38*sizeof(float), sizeof(float), &i);
    i  -= 0.5f;
    glBufferSubData(GL_ARRAY_BUFFER, 25*sizeof(float), sizeof(float), &i);
    glBufferSubData(GL_ARRAY_BUFFER, 35*sizeof(float), sizeof(float), &i);
    
}

void jpl::_graphics::_engine::ProgressBar::render() const noexcept{
    glActiveTexture(GL_TEXTURE0);
    this->background->bind();
    glDrawElements(GL_TRIANGLES, 12, GL_UNSIGNED_INT, 0);
}