#include "TextRender.hpp"

jpl::_graphics::_engine::_text::TextRender::TextRender(unsigned int ps, float x, float y, float w, float h)
    : x(x), y(y), w(w), h(h){

    this->programShader = ps;
    this->colorsLocation = glGetUniformLocation(this->programShader, "colors");
    this->projectionLocation = glGetUniformLocation(this->programShader, "projection");
    this->sizeFont = 1;
    this->charsToRender = 0;
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
    
    if(text.empty()){
        return;
    }
    this->text = text;
    float offsetX = this->offsetX*this->sizeFont;
    float offsetY = this->offsetY*this->sizeFont;
    float x = this->x;
    float y = this->y+this->h-offsetY;
    //20 floats for each char: 3 for vertex coords and 2 for texture coords
    float* buffer = new float[20*this->text.size()];
    unsigned int* indices = new unsigned int[6*this->text.size()];
    this->charsToRender = 0;
    for(int i = 0; i < this->text.size(); i++){

        char cr = this->text.at(i);
        bool newline = cr == '\n';
        if(!newline){
            float r = cr/this->font->getCharsPerWidth();//row(height by top side)
            float c = cr%this->font->getCharsPerWidth();//col(width by left side)
            r *= this->offsetTexY;    //With c and r coords the rendered texture begins from top-left corner
            c *= this->offsetTexX;
            
            buffer[20*i] = x;                   //Bottom-left
            buffer[20*i+1] = y;
            buffer[20*i+2] = 0.0f;
            buffer[20*i+3] = c;
            buffer[20*i+4] = r+this->offsetTexY;

            buffer[20*i+5] = x+offsetX;  //Top-right
            buffer[20*i+6] = y+offsetY;
            buffer[20*i+7] = 0.0f;
            buffer[20*i+8] = c+this->offsetTexX;
            buffer[20*i+9] = r;

            buffer[20*i+10] = x;                //Top-left
            buffer[20*i+11] = y+offsetY;
            buffer[20*i+12] = 0.0f;
            buffer[20*i+13] = c;
            buffer[20*i+14] = r;

            buffer[20*i+15] = x+offsetX;   //Bottom-right
            buffer[20*i+16] = y;
            buffer[20*i+17] = 0.0f;
            buffer[20*i+18] = c+this->offsetTexX;
            buffer[20*i+19] = r+this->offsetTexY;

            indices[6*i]   = 4*i;     // Bottom-left
            indices[6*i+1] = 4*i+3;   // Bottom-right
            indices[6*i+2] = 4*i+1;   // Top-right
            indices[6*i+3] = 4*i;     // Bottom-left
            indices[6*i+4] = 4*i+1;   // Top-right
            indices[6*i+5] = 4*i+2;   // Top-left
            this->charsToRender++;
            //Next x pos
            x += offsetX;
        }else{
            x = this->w; //Force next if-statement to be taken
        }
        //x+this->offsetX in order to check whereas there's enough space for the next char
        if(x+this->offsetX >= this->x + this->w){
            x = this->x;
            y -= offsetY;   //new line
            if(y < this->y){
                break;
            }
        }
    }
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int)*6*this->charsToRender, indices, GL_STATIC_DRAW);
    glBufferData(GL_ARRAY_BUFFER, 20*sizeof(float)*this->charsToRender, buffer, GL_STATIC_DRAW);
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
    delete[] indices;
}

void jpl::_graphics::_engine::_text::TextRender::render() const{
    glActiveTexture(GL_TEXTURE0);
    this->font->getTexture()->bind();
    glUniform4fv(this->colorsLocation, 4, glm::value_ptr(glm::vec4(this->r, this->g, this->b, this->a)));
    glUniformMatrix4fv(this->projectionLocation, 1, GL_FALSE, 
        glm::value_ptr(
            glm::ortho(0.0f, (float)jpl::_graphics::_metrics::monitorWidth, 0.0f, (float)jpl::_graphics::_metrics::monitorHeight)
        )
    );
    glDrawElements(GL_TRIANGLES, 6*this->charsToRender, GL_UNSIGNED_INT, 0);
}

void jpl::_graphics::_engine::_text::TextRender::render(const std::string &text, float x, float y, float w, float h, float r, float g, float b, float a){
    this->setDim(x,y,w,h);
    this->setRGBA(r,g,b,a);
    this->setText(text);
    this->render();
}