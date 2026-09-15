#include "Button.hpp"

jpl::_graphics::_engine::_button::Button::Button(float x, float y, float w, float h,  _texture::Texture* texture) : 
    jpl::_graphics::_engine::IClickable(x,y,w,h){
    if(texture == nullptr){
        throw jpl::_exception::IllegalArgumentException("texture cannot be nullptr");
    }
    this->texture = texture;
    this->textRender = nullptr;
}

void jpl::_graphics::_engine::_button::Button::setText(const std::string &text) {
    this->psTextRenderer->use();
    this->vaoTextRenderer->bind();
    this->vaoTextRenderer->VBOs.at(0)->bind();    
    this->textRender->setText(text);
}

void jpl::_graphics::_engine::_button::Button::render(jpl::_graphics::_engine::Painter* painter){
    painter->render(this->texture, this->x, this->y, this->w, this->h);
    if(this->textRender != nullptr){
        this->psTextRenderer->use();
        this->vaoTextRenderer->bind();
        this->textRender->render(painter);
    }
}