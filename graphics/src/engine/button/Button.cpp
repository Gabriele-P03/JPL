#include "Button.hpp"

jpl::_graphics::_engine::_button::Button::Button(unsigned int posX, unsigned int posY, unsigned int width, unsigned int height, _text::TextRender* textRender, _texture::Texture* texture){
    if(textRender != nullptr){
        this->textRender = textRender;
    }else{
        this->textRender = nullptr;
    }
    if(texture == nullptr){
        throw jpl::_exception::IllegalArgumentException("texture cannot be nullptr");
    }
    this->texture = texture;
    this->setPos(posX, posY, width, height);
    this->textRender->setDim(posX, posY, width, height);
    this->textRender->updateCoords();
}

void jpl::_graphics::_engine::_button::Button::setPos(unsigned int posX, unsigned int posY, unsigned int width, unsigned int height){
    float startX = jpl::_graphics::_metrics::XPosToScreenCoords(posX);
    float startY = jpl::_graphics::_metrics::YPosToScreenCoords(posY);
    float w = ((float)width)/jpl::_graphics::_metrics::width;
    float h = ((float)height)/jpl::_graphics::_metrics::height;
    float vertices[20] = {
        startX, startY, 0.0f, 0.0f, 0.0f,
        startX+w, startY+h, 0.0f, 1.0f, 1.0f,
        startX, startY+h, 0.0f, 0.0f, 1.0f,
        startX+w, startY, 0.0f, 1.0f, 0.0f
    };
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
    this->posX = posX;
    this->posY = posY;
    this->width = width;
    this->height = height;
}

void jpl::_graphics::_engine::_button::Button::render() const{
    glActiveTexture(GL_TEXTURE0);
    this->texture->bind();
    glDrawElements(GL_QUADS, 4, GL_UNSIGNED_INT, 0);
    if(this->textRender != nullptr){
        this->textRender->render();
    }
}