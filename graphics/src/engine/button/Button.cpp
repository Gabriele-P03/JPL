#include "Button.hpp"

void jpl::_graphics::_engine::_button::Button::renderTexture() const{
    if(this->texture != nullptr){
        glActiveTexture(GL_TEXTURE0);
        this->texture->bind();
        this->painter->setPos(this->posX, this->posY, 0.0f);
        this->painter->pushData(jpl::_graphics::_mesh::QUAD, GL_DYNAMIC_DRAW);
        jpl::_graphics::_engine::drawMesh(this->painter, jpl::_graphics::_mesh::QUAD);
    }
    
}
void jpl::_graphics::_engine::_button::Button::renderText() const{
    this->textRender->setDim(this->posX, this->posY, this->width, this->height);
    this->textRender->setText(this->text);
    this->textRender->updateCoords();
    this->textRender->render();
}
void jpl::_graphics::_engine::_button::Button::render() const {
    this->renderTexture();
    this->renderText();
}

void jpl::_graphics::_engine::_button::Button::setBackgroundTexture(jpl::_graphics::_texture::Texture* texture){
    if(texture == nullptr){
        throw jpl::_exception::IllegalArgumentException("texture is nullptr");
    }
    if(this->painter = nullptr){
        throw jpl::_exception::IllegalStateException("Painter has not been set, yet");
    }
    this->texture = texture;
}

void jpl::_graphics::_engine::_button::Button::setPainter(jpl::_graphics::_engine::Painter* painter){
    if(painter == nullptr){
        throw jpl::_exception::IllegalArgumentException("painter is nullptr");
    }
    this->painter = painter;
}

void jpl::_graphics::_engine::_button::Button::setBackgroundColor(float r, float g, float b, float a){
    if(r < 0.0f || r > 1.0f){
        throw jpl::_exception::IllegalArgumentException("Value of r outside of range: " + std::to_string(r));
    }
    if(g < 0.0f || g > 1.0f){
        throw jpl::_exception::IllegalArgumentException("Value of g outside of range: " + std::to_string(g));
    }
    if(b < 0.0f || b > 1.0f){
        throw jpl::_exception::IllegalArgumentException("Value of b outside of range: " + std::to_string(b));
    }
    if(a < 0.0f || a > 1.0f){
        throw jpl::_exception::IllegalArgumentException("Value of a outside of range: " + std::to_string(a));
    }
    this->r = r;
    this->g = g;
    this->b = b;
    this->a = a;
}

void jpl::_graphics::_engine::_button::Button::setTextRender(jpl::_graphics::_engine::_text::TextRender* textRender){
    if(textRender == nullptr){
        throw jpl::_exception::IllegalArgumentException("textRender is nullptr");
    }
    this->textRender = textRender;
}