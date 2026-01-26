#include "Button.hpp"


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
}