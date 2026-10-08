#include "Button.hpp"

jpl::_graphics::_engine::_button::Button::Button(float x, float y, float w, float h, _texture::Texture* texture) : IClickable(x,y,w,h){
    if(texture == nullptr){
        throw jpl::_exception::IllegalArgumentException("texture cannot be nullptr");
    }
    this->texture = texture;
}

void jpl::_graphics::_engine::_button::Button::setText(const std::string &text) {
    this->label = text;
    this->labelDirty = true;
}

void jpl::_graphics::_engine::_button::Button::setTextScale(float scale) {
    this->labelScale = scale;
    this->labelDirty = true;
}

void jpl::_graphics::_engine::_button::Button::render(jpl::_graphics::_engine::Painter* painter){
    painter->render(this->texture, this->x, this->y, this->w, this->h);
}

void jpl::_graphics::_engine::_button::Button::renderLabel(_text::TextRender &tr){
    if(this->label.empty()){
        return;
    }
    if(this->labelDirty){
        this->labelSize = tr.measure(this->label, this->labelScale);
        this->labelDirty = false;
    }
    float tx = this->x + (this->w - this->labelSize.x) * 0.5f;
    float ty = this->y + this->h * 0.5f - tr.capCenterOffset(this->labelScale);
    tr.draw(this->label, tx, ty, this->labelScale, this->labelColor);
}
