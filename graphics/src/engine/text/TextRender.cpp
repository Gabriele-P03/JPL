#include "TextRender.hpp"
jpl::_graphics::_engine::_text::TextRender::TextRender()
    : jpl::_graphics::_engine::_text::TextRender(0,0,0,0){
}

jpl::_graphics::_engine::_text::TextRender::TextRender(float x, float y, float width, float height)
    : posX(x), posY(y), width(width), height(height){
        this->editable = false;
        this->focus = false;
        this->centered = false;
}

void jpl::_graphics::_engine::_text::TextRender::setDim(float x, float y, float w, float h){
    this->posX = x;
    this->posY = y;
    this->width = w;
    this->height = h;
}
void jpl::_graphics::_engine::_text::TextRender::updateCoords(){
    this->startX = jpl::_graphics::_metrics::XPosToScreenCoords(this->posX);
    //this->offsetX = ((float)this->font->getPixelWidthPerChar())/(jpl::_graphics::_metrics::width/2);
    this->startY = jpl::_graphics::_metrics::YPosToScreenCoords(this->posY+this->height);
    //this->offsetY = ((float)this->font->getPixelHeightPerChar())/(jpl::_graphics::_metrics::height/2);
    this->offsetX = ((float)this->font->getPixelWidthPerChar())/jpl::_graphics::_metrics::width;
    this->offsetY = ((float)this->font->getPixelHeightPerChar())/jpl::_graphics::_metrics::height;
    this->w1 = (this->width/jpl::_graphics::_metrics::width)*2.0f;
    this->h1 = (this->height/jpl::_graphics::_metrics::height)*2.0f;
}

void jpl::_graphics::_engine::_text::TextRender::setText(const std::string &text){
    
    if(text.empty()){
        return;
    }
    this->text = text;
    float x = this->startX;
    float y = this->startY;
    int l = -1; //Set -1 to pass (1) at i = 0
    //20 floats for each char: 3 for vertex coords and 2 for texture coords
    float* buffer = new float[20*this->text.size()];
    unsigned int* indices = new unsigned int[6*this->text.size()];

    for(int i = 0; i < this->text.size(); i++){
        if(this->centered){
            if(i > l){ //(1)
                x = jpl::_graphics::_engine::_text::TextRender::calculateStartXCentered(i, l);
                x = jpl::_graphics::_metrics::XPosToScreenCoords(x);
                l = std::max(0, l+i);
            }
        }

        char first_char = this->text.at(i);
        float r = first_char/this->font->getCharsPerWidth();//row(height by top side)
        float c = first_char%this->font->getCharsPerWidth();//col(width by right side)
        r *= this->offsetTexY;    //With c and r coords the rendered texture begins from top-right corner
        c *= this->offsetTexX;
        
        buffer[20*i] = x;                   //Bottom-left
        buffer[20*i+1] = y;
        buffer[20*i+2] = 0.0f;
        buffer[20*i+3] = c;
        buffer[20*i+4] = r+this->offsetTexY;

        buffer[20*i+5] = x+this->offsetX;  //Top-right
        buffer[20*i+6] = y+this->offsetY;
        buffer[20*i+7] = 0.0f;
        buffer[20*i+8] = c+this->offsetTexX;
        buffer[20*i+9] = r;

        buffer[20*i+10] = x;                //Top-left
        buffer[20*i+11] = y+this->offsetY;
        buffer[20*i+12] = 0.0f;
        buffer[20*i+13] = c;
        buffer[20*i+14] = r;

        buffer[20*i+15] = x+this->offsetX;   //Bottom-right
        buffer[20*i+16] = y;
        buffer[20*i+17] = 0.0f;
        buffer[20*i+18] = c+this->offsetTexX;
        buffer[20*i+19] = r+this->offsetTexY;

        indices[6*i] = 4*i;     //Empty part
        indices[6*i+1] = 4*i+1;
        indices[6*i+2] = 4*i+2;
        indices[6*i+3] = 4*i;
        indices[6*i+4] = 4*i+1;
        indices[6*i+5] = 4*i+3;

        //Next x pos
        x += this->offsetX;
        if(x >= this->startX+this->w1){
            x = this->startX;
            y -= this->offsetY;   //new line
            float tmp = this->startY-this->h1; //The lowest y pos that can be rendered
            if(y < tmp){
                break;
            }
        }
    }
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int)*6*this->text.size(), indices, GL_STATIC_DRAW);
    glBufferData(GL_ARRAY_BUFFER, 20*sizeof(float)*this->text.size(), buffer, GL_STATIC_DRAW);
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
    delete[] buffer;
}

void jpl::_graphics::_engine::_text::TextRender::render() const{
    glActiveTexture(GL_TEXTURE0);
    this->font->getTexture()->bind();
    glDrawElements(GL_TRIANGLES, 6*this->text.size(), GL_UNSIGNED_INT, 0);
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