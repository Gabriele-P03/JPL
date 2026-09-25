#include "ProgressBar.hpp"

jpl::_graphics::_engine::ProgressBar::ProgressBar(
    jpl::_graphics::_texture::Texture* empty, jpl::_graphics::_texture::Texture* filled, float max){
    if(empty == nullptr){
        throw jpl::_exception::IllegalArgumentException("Empty Texture is nullptr");
    }
    if(filled == nullptr){
        throw jpl::_exception::IllegalArgumentException("Filled Texture is nullptr");
    }
    this->empty = empty;
    this->filled = filled;
    this->max = max;
    this->progress = 0;
}


void jpl::_graphics::_engine::ProgressBar::setProgress(float i){
    if(i < 0){
        throw jpl::_exception::IllegalArgumentException("progress " + std::to_string(i) + " is less than 0");
    }
    if(i > this->max){
        throw jpl::_exception::IllegalArgumentException("progress " + std::to_string(i) + " is less than max " + std::to_string(this->max));
    }
    this->progress = i;
}

void jpl::_graphics::_engine::ProgressBar::render(jpl::_graphics::_engine::Painter* p, float x, float y, float w, float h) const noexcept{
    p->render(this->empty, x,y,w,h);
    p->render(this->filled, x+2.0f, y+2.0f, w-4.0f, h-4.0f);
}