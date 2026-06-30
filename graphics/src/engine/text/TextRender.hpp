/**
 * TextRender provides a way to render some text on the screen. 
 * 
 * If text exceeds available space, a scrollable view is implemented.
 * 
 * Although window coords begins from bottom-left corner up to the top-right, text renderer begins to write from top-left corner of its view 
 */

#ifndef TEXTRENDER_GRAPHICS_JPL
#define TEXTRENDER_GRAPHICS_JPL

#include <string>
#include "Font.hpp"
#include <jpl/exception/runtime/IllegalStateException.hpp>
#include "../../shaders/ProgramManager.hpp"
#include "../../shaders/Shader.hpp"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "../../Metrics.hpp"
#include <math.h>


namespace jpl{
    namespace _graphics{

        namespace _engine{

            namespace _text{
                class TextRender{

                    protected:

                        float x,y,w,h;
                        float r,g,b,a;
                        //It is updated by setText in order to prevent any ovewrflow during render call
                        unsigned int charsToRender;
                        //In NDC
                        float offsetTexX, offsetTexY;
                        float offsetX, offsetY;
                        unsigned int projectionLocation, colorsLocation;

                        unsigned int sizeFont;
                        std::string text;
                        unsigned int programShader;
                        const Font* font;

                    public:
                        /**
                         * @param ps programShader
                         * @param x
                         * @param y
                         * @param w
                         * @param h
                         */
                        TextRender(unsigned int ps, float x, float y, float w, float h);

                        void setFont(Font* font);

                        void setRGBA(float r, float g, float b, float a);
                        virtual void setText(const std::string &text);
                        void render() const;
                        void render(const std::string &text, float x, float y, float w, float h, float r, float g, float b, float a);

                        std::string getText() const noexcept{
                            return this->text;
                        }

                        void setDim(float x, float y, float w, float h) noexcept{
                            this->x = x;
                            this->y = y;
                            this->w = w;
                            this->h = h;
                        }

                        void setProgramShader(unsigned int ps) noexcept{
                            this->programShader = ps;
                        }
                        unsigned int getProgramShader() const noexcept{
                            return this->programShader;
                        }

                        void setFontSize(unsigned int fontSize) noexcept{
                            this->sizeFont = fontSize;
                        }
                        unsigned int getFontSize() const noexcept{
                            return this->sizeFont;
                        }

                        ~TextRender(){
                        }
                };
            }
        }
    }
}

#endif