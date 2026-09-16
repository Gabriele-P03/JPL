#include "TextRender.hpp"

jpl::_graphics::_engine::_text::TextRender::TextRender(float x, float y, float w, float h)
    : jpl::_graphics::_engine::IClickable::IClickable(x,y,w,h), jpl::_graphics::_engine::ITextEditable(""){
    this->sizeFont = 1;
    this->setFont(jpl::_graphics::_engine::_text::ARIALS);
    this->charsToRender = 0;
    this->editable = false;
    this->focused = false;
}

void jpl::_graphics::_engine::_text::TextRender::setRGBA(float r, float g, float b, float a){
    this->r = r;
    this->g = g;
    this->b = b;
    this->a = a;
}

void jpl::_graphics::_engine::_text::TextRender::setFont(jpl::_graphics::_engine::_text::Font* font){
    if(font == nullptr)
        throw jpl::_exception::IllegalArgumentException("Font is nullptr");
    this->font = font;
    this->offsetX = font->getPixelWidthPerChar();
    this->offsetY = font->getPixelHeightPerChar();
    this->offsetTexX = (float)this->font->getPixelWidthPerChar()/(float)this->font->getWidthFontMap();
    this->offsetTexY = (float)this->font->getPixelHeightPerChar()/(float)this->font->getHeightFontMap();
}

void jpl::_graphics::_engine::_text::TextRender::setText(const std::string &text){
    this->ps->use();
    this->vao->bind();
    if(text.empty()){
        return;
    }
    jpl::_graphics::_engine::ITextEditable::setText(text);
    float offsetX = this->offsetX*this->sizeFont;
    float offsetY = this->offsetY*this->sizeFont;
    float x = this->x;
    float y = this->y+this->h-offsetY;

    std::vector<float> buffer(20 * text.size(), 0.0f);
    
    this->charsToRender = 0;
    for(int i = 0; i < this->text.size(); i++){
        char cr = this->text.at(i);
        bool newline = cr == '\n';
        if(!newline){
            float r = cr/this->font->getCharsPerWidth();
            float c = cr%this->font->getCharsPerWidth();
            r = 1.0f - r*this->offsetTexY;    
            c *= this->offsetTexX;

            int baseIdx = 20 * this->charsToRender;
            //BR
            buffer[baseIdx]     = x;                    
            buffer[baseIdx + 1] = y;
            buffer[baseIdx + 2] = 0.0f;
            buffer[baseIdx + 3] = c;
            buffer[baseIdx + 4] = r - this->offsetTexY;
            //TR
            buffer[baseIdx + 5] = x + offsetX;  
            buffer[baseIdx + 6] = y + offsetY;
            buffer[baseIdx + 7] = 0.0f;
            buffer[baseIdx + 8] = c + this->offsetTexX;
            buffer[baseIdx + 9] = r;
            //TL
            buffer[baseIdx + 10] = x;                
            buffer[baseIdx + 11] = y + offsetY;
            buffer[baseIdx + 12] = 0.0f;
            buffer[baseIdx + 13] = c;
            buffer[baseIdx + 14] = r;
            //BR
            buffer[baseIdx + 15] = x + offsetX;   
            buffer[baseIdx + 16] = y;
            buffer[baseIdx + 17] = 0.0f;
            buffer[baseIdx + 18] = c + this->offsetTexX;
            buffer[baseIdx + 19] = r - this->offsetTexY;
            
            this->charsToRender++;
            x += offsetX;
        }else{
            x = this->w; 
        }
        
        if(x + this->offsetX >= this->x + this->w){
            x = this->x;
            y -= offsetY;   
            if(y < this->y){
                break;
            }
        }
    }
    glBufferSubData(GL_ARRAY_BUFFER, 0, 20*sizeof(float)*this->charsToRender, buffer.data());
}

void jpl::_graphics::_engine::_text::TextRender::render(jpl::_graphics::_engine::Painter* painter){
    this->ps->use();
    this->vao->bind();
    glActiveTexture(GL_TEXTURE0);
    this->font->getTexture()->bind();

    GLint x = 0;
    glGetIntegerv(GL_CURRENT_PROGRAM, &x);
    int i = glGetUniformLocation(x, "projection");
    glUniformMatrix4fv(i, 1, GL_FALSE, glm::value_ptr(jpl::_graphics::_metrics::ortho));

    i = glGetUniformLocation(x, "colors");
    glUniform4fv(2, 1, glm::value_ptr(glm::vec4(this->r, this->g, this->b, this->a)));

    //glDisable(GL_CULL_FACE); // Spegne temporaneamente il culling per il testo
    glDrawElements(GL_TRIANGLES, 6 * this->charsToRender, GL_UNSIGNED_INT, 0);
    //glEnable(GL_CULL_FACE);  // Lo riaccende subito dopo
}

void jpl::_graphics::_engine::_text::TextRender::render(const std::string &text, float x, float y, float w, float h, float r, float g, float b, float a){
    this->setDim(x,y,w,h);
    this->setRGBA(r,g,b,a);
    this->setText(text);
    this->render(nullptr);
}