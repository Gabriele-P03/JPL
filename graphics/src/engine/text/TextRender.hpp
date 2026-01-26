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
                        std::string text;

                        bool centered;

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

                        /**
                         * Calculate the startX for the new line in case of centered rendering
                         * @param i index of the first char into text
                         * @param l amount of chars that can be rendered on the current line
                         */
                        virtual unsigned int calculateStartXCentered(unsigned int i, int &l) const noexcept;

                        /**
                         * Set if text may be edited
                         */
                        bool editable;
                        /**
                         * If editable is set, this is set if player clicked the textbox
                         */
                        bool focus;

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

                        void setDim(float x, float y, float w, float h);

                        /**
                         * It must be called both when new dimension have been set and after object has been instanced
                         * Notice that this function needs to be called after have set a font 
                         */
                        void updateCoords();

                        virtual void setText(const std::string &text);

                        void render() const;
                        void render(const std::string &text, unsigned int x, unsigned int y, unsigned int w, unsigned int h);

                        std::string getText() const noexcept{
                            return this->text;
                        }

                        void setCentered(bool v) noexcept{
                            this->centered = v;
                        }
                        const bool isCentered() const noexcept{
                            return this->centered;
                        }

                        void setEditable(bool editable) noexcept{
                            this->editable = editable;
                        }
                        bool isEditable() const noexcept{
                            return this->editable;
                        }
                        void setFocus(bool focus) noexcept{
                            this->focus = focus;
                        }
                        bool isFocus() const noexcept{
                            return this->focus;
                        }
                };
            }
        }
    }
}

#endif