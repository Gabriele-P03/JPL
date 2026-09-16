/**
 * A button is a clickable 2D element which call an event once clicked.
 * It may render text inside of it or even texture.
 * 
 * TextRenderer is not freed via Button's destructor, it is up to you to deallocate it
 */

#ifndef BUTTON_GRAPHICS_JPL
#define BUTTON_GRAPHICS_JPL

#include "../text/TextRender.hpp"
#include "../interfaces/IClickable.hpp"
#include "../VAO.hpp"

namespace jpl{
    namespace _graphics{
        namespace _engine{
            namespace _button{

                class Button : public IClickable{

                    protected:
                        
                        _text::TextRender* textRender;
                        _texture::Texture* texture;

                    public:

                        Button(float x, float y, float w, float h, _texture::Texture* texture);

                        /**
                         * Sets new text for the Button's TextRender. VAO and Shader are not activated automatically
                         * @param text
                         * @throw IllegalStateException if textRender has not set yet
                         */
                        virtual void setText(const std::string &text);
                        /**
                         * @return current rendered text or empty string if textRender has not set yet
                         */
                        std::string getText() const noexcept{
                            return this->textRender->getText();
                        }
                        
                        void setTextRender(_text::TextRender* tr){
                            this->textRender = tr;
                        }
                        const _text::TextRender* getTextRender() const noexcept{
                            return this->textRender;
                        }
                        /**
                         * Render button.
                         * Please, ensure to have the right VBO already binded since this function calls glBufferSubData 
                         */
                        virtual void render(Painter* painter) override;


                        ~Button(){
                            delete this->texture;
                        }
                };
            }
        }
    }
}

#endif