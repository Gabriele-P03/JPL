#include "TextRender.hpp"


jpl::_graphics::_engine::_text::TextRender::TextRender(float x, float y, float width, float height)
    : posX(x), posY(x), width(width), height(height){
}

void jpl::_graphics::_engine::_text::TextRender::setDim(float x, float y, float w, float h){
    this->posX = x;
    this->posY = y;
    this->width = w;
    this->height = h;
}
void jpl::_graphics::_engine::_text::TextRender::updateCoords(){
    this->startX = jpl::_graphics::_metrics::XPosToScreenCoords(this->posX);
    this->offsetX = ((float)this->font->getPixelWidthPerChar())/(jpl::_graphics::_metrics::width/2);
    this->startY = jpl::_graphics::_metrics::YPosToScreenCoords(this->posY+this->height);
    this->offsetY = ((float)this->font->getPixelHeightPerChar())/(jpl::_graphics::_metrics::height/2);
    this->w1 = this->width/jpl::_graphics::_metrics::width;
    this->h1 = this->height/jpl::_graphics::_metrics::height;
}

void jpl::_graphics::_engine::_text::TextRender::setText(const std::string &text){
    this->text = text;
    float x = this->startX;
    float y = this->startY;
    int l = -1; //Set -1 to pass (1) at i = 0
    for(int i = 0; i < this->text.size(); i++){
        if(this->centered){
            if(i > l){ //(1)
                x = jpl::_graphics::_engine::_text::TextRender::calculateStartXCentered(i, l);
                x = jpl::_graphics::_metrics::XPosToScreenCoords(x);
                l = std::max(0, l+i);
            }
        }
        char cr = this->text.at(i);
        float r = cr/this->font->getCharsPerWidth();//row(height by top side)
        float c = cr%this->font->getCharsPerWidth();//col(width by right side)
        r *= this->offsetTexY;    //With c and r coords the rendered texture begins from top-right corner
        c *= this->offsetTexX;
        
        float buffer[20] = {
            x,y, 0.0f, c, r+this->offsetTexY,  //Bottom-left
            x+this->offsetX, y, 0.0f, c+this->offsetTexX,r+this->offsetTexY,    //Bottom-right
            x+this->offsetX, y+this->offsetY, 0.0f, c+this->offsetTexX, r,       //Top-right
            x, y+this->offsetY, 0.0f, c, r     //Top-left
        };
        glBufferData(GL_ARRAY_BUFFER, 20*sizeof(float), &buffer, GL_STATIC_DRAW);
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
        x += this->offsetX;
        if(x >= std::abs(this->startX)+w1){
            x = this->startX;
            y -= this->offsetY;   //new line
            if(y > this->startY-this->h1){
                break;
            }
        }
    }
}

void jpl::_graphics::_engine::_text::TextRender::render() const{
    glActiveTexture(GL_TEXTURE0);
    this->font->getTexture()->bind();
    glDrawElements(GL_QUADS, 4*this->text.size(), GL_UNSIGNED_INT, 0);
}

void jpl::_graphics::_engine::_text::TextRender::render(const std::string &text, unsigned int x, unsigned int y, unsigned int w, unsigned int h){
    this->setText(text);
    this->setDim(x,y,w,h);
    this->updateCoords();
    this->render();
}

unsigned int jpl::_graphics::_engine::_text::TextRender::calculateStartXCentered(unsigned int i, int &l) const noexcept{
    std::string textWONL = this->text.substr(i, this->text.find('\n')); //text within first new line char
    l = textWONL.size();
    if(l*this->font->getPixelWidthPerChar() > this->width){
        l = this->width/this->font->getPixelWidthPerChar(); //Max chars renderizable is equal to width/pixelWidthPerChar
        textWONL = textWONL.substr(0, l);
    }
    size_t s1px = this->font->getPixelWidthPerChar();  //Pixel needed for the textWONL
    s1px *= l; 
    int x = this->width/2-s1px/2;
    return x;
}