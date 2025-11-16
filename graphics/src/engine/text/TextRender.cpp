#include "TextRender.hpp"

jpl::_graphics::_shaders::ProgramShaders* jpl::_graphics::_engine::_text::TextRender::PROGRAM_SHADERS = nullptr;
jpl::_graphics::_engine::Painter* jpl::_graphics::_engine::_text::TextRender::PAINTER = nullptr;

jpl::_graphics::_engine::_text::TextRender::TextRender(float x, float y, float width, float height)
    : x(x), y(x), width(width), height(height){
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
    unsigned int w = this->font->getPixelWidthPerChar();
    unsigned int h = this->font->getPixelHeightPerChar();

    float startX = this->x/jpl::_graphics::_metrics::width;
    float x = jpl::_graphics::_metrics::XPosToScreenCoords(startX);
    float offsetX = ((float)this->font->getPixelWidthPerChar())/jpl::_graphics::_metrics::width;
    float startY = (this->y+this->height)/jpl::_graphics::_metrics::height;
    float y = jpl::_graphics::_metrics::YPosToScreenCoords(startY);
    float offsetY = ((float)this->font->getPixelHeightPerChar())/jpl::_graphics::_metrics::height;
    float w1 = this->width/jpl::_graphics::_metrics::width;
    float h1 = this->height/jpl::_graphics::_metrics::height;

    int locTranslate = glGetUniformLocation(jpl::_graphics::_engine::_text::TextRender::PROGRAM_SHADERS->getProgramIndex(), "translate");

    float offsetTexX = 1.0f/this->font->getCharsPerWidth();
    float offsetTexY = 1.0f/this->font->getCharsPerHeight();

    for(size_t i = 0; i < this->text.size(); i++){
        char cr = this->text.at(i);
        float r = cr/this->font->getCharsPerWidth();//row(height by top side)
        float c = cr%this->font->getCharsPerWidth();//col(width by right side)
        r *= offsetTexY;    //With c and r coords the rendered texture begins from top-right corner
        c *= offsetTexX;
        
        float* buffer = new float[20]{
            x,y, 0.0f, c, r+offsetTexY,  //Bottom-left
            x+offsetX, y, 0.0f, c+offsetTexX,r+offsetTexY,    //Bottom-right
            x+offsetX, y+offsetY, 0.0f, c+offsetTexX, r,       //Top-right
            x, y+offsetY, 0.0f, c, r     //Top-left
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
        jpl::_graphics::_engine::drawMesh(jpl::_graphics::_engine::_text::TextRender::PAINTER, jpl::_graphics::_mesh::QUAD);
        x += offsetX;
        if(x >= startX+w1){
            x = 0;
            y -= offsetY;
        }
    }
    
}

void jpl::_graphics::_engine::_text::TextRender::initializeProgramShaders(
        jpl::_graphics::_shaders::ProgramManager* manager,
        const std::string &vertexShaderFileName,
        const std::string &fragmentShaderFileName
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

    jpl::_graphics::_engine::_text::TextRender::PROGRAM_SHADERS = programShaders;

    jpl::_graphics::_engine::_text::TextRender::PAINTER = new jpl::_graphics::_engine::Painter();
    jpl::_graphics::_engine::_text::TextRender::PAINTER->pushData(jpl::_graphics::_mesh::QUAD, GL_STATIC_DRAW);
}