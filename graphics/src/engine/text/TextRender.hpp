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

                        float x,y;
                        float width,height;

                        const Font* font;

                        static _shaders::ProgramShaders* PROGRAM_SHADERS;
                        static Painter* PAINTER;

                    public:
                        TextRender(float x, float y, float width, float height);

                        void setFont(const Font* font){
                            if(font == nullptr){
                                throw jpl::_exception::IllegalArgumentException("Font is nullptr");
                            }
                            this->font = font;
                        }

                        void setText(const std::string &text) noexcept{
                            this->text = text;
                        }

                        void render() const;

                        std::string getText() const noexcept{
                            return this->text;
                        }

                        static void setProgramShaders(_shaders::ProgramShaders* ps){
                            PROGRAM_SHADERS = ps;
                        }

                        static const _shaders::ProgramShaders* const getShader(){
                            return const_cast<const _shaders::ProgramShaders* const>(PROGRAM_SHADERS);
                        }

                        static void initializeProgramShaders(
                            _shaders::ProgramManager* manager,
                            const std::string &vertexShaderFileName,
                            const std::string &fragmentShaderFileName
                        );  
                };
            }
        }
    }
}

#endif