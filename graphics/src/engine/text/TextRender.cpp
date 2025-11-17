#include "TextRender.hpp"


jpl::_graphics::_engine::_text::TextRender::TextRender(float x, float y, float width, float height)
    : posX(x), posY(x), width(width), height(height), r(0.0f), g(0.0f), b(0.0f), a(1.0f){
}

void jpl::_graphics::_engine::_text::TextRender::setDim(float x, float y, float w, float h){
    this->posX = x;
    this->posY = y;
    this->width = w;
    this->height = h;
}
void jpl::_graphics::_engine::_text::TextRender::updateCoords(){
    this->startX = jpl::_graphics::_metrics::XPosToScreenCoords(this->posX);
    this->offsetX = ((float)this->font->getPixelWidthPerChar())/jpl::_graphics::_metrics::width;
    this->startY = jpl::_graphics::_metrics::YPosToScreenCoords(this->posY+this->height);
    this->offsetY = ((float)this->font->getPixelHeightPerChar())/jpl::_graphics::_metrics::height;
    this->w1 = this->width/jpl::_graphics::_metrics::width;
    this->h1 = this->height/jpl::_graphics::_metrics::height;
}

void jpl::_graphics::_engine::_text::TextRender::setColors(float r, float g, float b, float a){
    this->r = r;
    this->g = g; 
    this->b = b;
    this->a = a;
}

void jpl::_graphics::_engine::_text::TextRender::render() const{
    if(this->font == nullptr){
        throw jpl::_exception::IllegalStateException("No Font has been set, yet");
    }
    if(jpl::_graphics::_engine::_text::TextRender::PROGRAM_SHADERS == nullptr){
        throw jpl::_exception::IllegalStateException("Static TextRender's ProgramShader instance is nullptr");
    }
    jpl::_graphics::_engine::_text::TextRender::PAINTER->pushData(jpl::_graphics::_mesh::QUAD, GL_STATIC_DRAW);
    glUseProgram(jpl::_graphics::_engine::_text::TextRender::PROGRAM_SHADERS->getProgramIndex());
    glActiveTexture(GL_TEXTURE0);
    this->font->getTexture()->bind();

    float x = this->startX;
    float y = this->startY;

    for(size_t i = 0; i < this->text.size(); i++){
        char cr = this->text.at(i);
        float r = cr/this->font->getCharsPerWidth();//row(height by top side)
        float c = cr%this->font->getCharsPerWidth();//col(width by right side)
        r *= this->offsetTexY;    //With c and r coords the rendered texture begins from top-right corner
        c *= this->offsetTexX;
        
        float* buffer = new float[20]{
            x,y, 0.0f, c, r+this->offsetTexY,  //Bottom-left
            x+this->offsetX, y, 0.0f, c+this->offsetTexX,r+this->offsetTexY,    //Bottom-right
            x+this->offsetX, y+this->offsetY, 0.0f, c+this->offsetTexX, r,       //Top-right
            x, y+this->offsetY, 0.0f, c, r     //Top-left
        };

        glBindBuffer(GL_ARRAY_BUFFER, jpl::_graphics::_engine::_text::TextRender::PAINTER->getVBO());
        glBufferData(GL_ARRAY_BUFFER, 20*sizeof(float), buffer, GL_STATIC_DRAW);
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
        glUniform4f(this->locColors, this->r, this->g, this->b, this->a);
        jpl::_graphics::_engine::drawMesh(this->PAINTER, jpl::_graphics::_mesh::QUAD);
        x += this->offsetX;
        if(x >= this->startX+w1){
            x = this->startX;
            y -= this->offsetY;   //new line
        }
    }
    
}

void jpl::_graphics::_engine::_text::TextRender::initializeProgramShaders(
        jpl::_graphics::_shaders::ProgramManager* manager,
        const std::string &vertexShaderFileName,
        const std::string &fragmentShaderFileName,
        jpl::_graphics::_engine::Painter* painter
){
    std::fstream* file = new std::fstream;
    jpl::_utils::_files::getInternalFile("shaders/"+vertexShaderFileName, std::fstream::in | std::fstream::binary, &file);
    jpl::_graphics::_shaders::Shader* vertexShader = new jpl::_graphics::_shaders::Shader(file);
    vertexShader->probeShader(GL_VERTEX_SHADER);
    delete file;
    file = new std::fstream;
    jpl::_utils::_files::getInternalFile("shaders/"+fragmentShaderFileName, std::fstream::in | std::fstream::binary, &file);
    jpl::_graphics::_shaders::Shader* fragmentShader = new jpl::_graphics::_shaders::Shader(file);
    fragmentShader->probeShader(GL_FRAGMENT_SHADER);
    delete file;
    jpl::_graphics::_shaders::ProgramShaders* programShaders = new jpl::_graphics::_shaders::ProgramShaders();
    programShaders->addNewShader(vertexShader);
    programShaders->addNewShader(fragmentShader);
    manager->addProgramShaders(programShaders);
    glLinkProgram(programShaders->getProgramIndex());

    this->PROGRAM_SHADERS = programShaders;
    this->PAINTER = painter;
    glUseProgram(programShaders->getProgramIndex());
    this->locColors = glGetUniformLocation(programShaders->getProgramIndex(), "colors");
}