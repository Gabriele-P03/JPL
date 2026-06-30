#include "Button.hpp"

jpl::_graphics::_engine::_button::Button::Button(unsigned int programIndex, float x, float y, float w, float h, 
    _text::TextRender* textRender, _texture::Texture* texture){
    if(texture == nullptr){
        throw jpl::_exception::IllegalArgumentException("texture cannot be nullptr");
    }
    this->texture = texture;
    this->setPos(x, y, w, h);
    if(textRender != nullptr){
        this->textRender = textRender;
        this->textRender->setDim(x, y, w, h);
    }else{
        this->textRender = nullptr;
    }
    this->programIndex = programIndex;
    this->projectionLocation = glGetUniformLocation(this->programIndex, "projection");
}

void jpl::_graphics::_engine::_button::Button::setPos(float x, float y, float w, float h){
    float vertices[20] = {
        x, y, 0.0f, 0.0f, 0.0f,
        x+w, y+h, 0.0f, 1.0f, 1.0f,
        x, y+h, 0.0f, 0.0f, 1.0f,
        x+w, y, 0.0f, 1.0f, 0.0f
    };
    glBufferData(GL_ARRAY_BUFFER, sizeof(float)*sizeof(vertices), vertices, GL_STATIC_DRAW);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int)*sizeof(indices), indices, GL_STATIC_DRAW);
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
    this->x = x;
    this->y = y;
    this->w = w;
    this->h = h;
}

void jpl::_graphics::_engine::_button::Button::render() const{
    glActiveTexture(GL_TEXTURE0);
    this->texture->bind();
    glUniformMatrix4fv(this->projectionLocation, 1, GL_FALSE, 
        glm::value_ptr(
            glm::ortho(0.0f, (float)jpl::_graphics::_metrics::monitorWidth, 0.0f, (float)jpl::_graphics::_metrics::monitorHeight)
        )
    );
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    if(this->textRender != nullptr){
        glUseProgram(this->textRender->getProgramShader());
        this->vaoTextRenderer->bind();
        this->textRender->render();
    }
}