/**
 * TextRender provides a way to render some text on the screen. 
 * Based on given shader, it may render either in a 3D or 2D context. 
 */

#ifndef TEXTRENDER_GRAPHICS_JPL
#define TEXTRENDER_GRAPHICS_JPL

#include <string>
#include "Font.hpp"
#include <jpl/exception/runtime/IllegalStateException.hpp>
#include "../../shaders/ProgramManager.hpp"
#include "../Painter.hpp"
#include "../../shaders/Shader.hpp"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "../../Metrics.hpp"


namespace jpl{
    namespace _graphics{

        namespace _engine{

            namespace _text{
                class TextRender{

                    protected:
                        std::string text;

                        float posX,posY;
                        float width,height;

                        float startX;
                        float offsetX;
                        float startY;
                        float offsetY;
                        float w1;
                        float h1;

                        /*
                            They contain offset per char in unit
                        */
                        float offsetTexX, offsetTexY;   

                        const Font* font;

                        _shaders::ProgramShaders* PROGRAM_SHADERS;
                        Painter* PAINTER;

                        float r,g,b,a;
                        int locColors;

                    public:
                        TextRender(float x, float y, float width, float height);

                        void setFont(const Font* font){
                            if(font == nullptr){
                                throw jpl::_exception::IllegalArgumentException("Font is nullptr");
                            }
                            this->font = font;
                            this->offsetTexX = 1.0f/font->getCharsPerWidth();
                            this->offsetTexY = 1.0f/font->getCharsPerHeight();
                        }

                        void setColors(float r, float g, float b, float a);

                        void setDim(float x, float y, float w, float h);

                        /**
                         * It must be called both when new dimension have been set and after object has been instanced
                         * Notice that this function needs to be called after have set a font 
                         */
                        void updateCoords();

                        void setText(const std::string &text) noexcept{
                            this->text = text;
                        }

                        void render() const;

                        std::string getText() const noexcept{
                            return this->text;
                        }

                        void setProgramShaders(_shaders::ProgramShaders* ps){
                            this->PROGRAM_SHADERS = ps;
                        }

                        const _shaders::ProgramShaders* const getShader(){
                            return const_cast<const _shaders::ProgramShaders* const>(PROGRAM_SHADERS);
                        }

                        void initializeProgramShaders(
                            _shaders::ProgramManager* manager,
                            const std::string &vertexShaderFileName,
                            const std::string &fragmentShaderFileName,
                            jpl::_graphics::_engine::Painter* painter
                        );  
                };
            }
        }
    }
}

#endif