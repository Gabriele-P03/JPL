/**
 * A button is a clickable 2D element which call an event once clicked.
 * It may render text inside of it or even texture.
 * 
 */

#ifndef BUTTON_GRAPHICS_JPL
#define BUTTON_GRAPHICS_JPL

#include "../text/TextRender.hpp"
#include <functional>

namespace jpl{
    namespace _graphics{
        namespace _engine{
            namespace _button{

                class Button{

                    protected:
                        
                        _text::TextRender* textRender;

                        _texture::Texture* texture;

                        unsigned int posX, posY, width, height;

                        static constexpr unsigned int indices[6] = {
                            0,1,2,
                            0,1,3
                        };

                        std::function<void()> clickFunction;

                    public:

                        Button(_text::TextRender* textRender, _texture::Texture* texture){
                                this->texture = texture;
                                this->textRender = textRender;
                        }

                        unsigned int getPosX() const noexcept{return this->posX;}
                        unsigned int getPosY() const noexcept{return this->posY;}
                        unsigned int getWidth() const noexcept{return this->width;}
                        unsigned int getHeight() const noexcept{return this->height;}
                        virtual void setPos(unsigned int posX, unsigned int posY, unsigned int width, unsigned int height);

                        /**
                         * If textRender instance is commonly used, you must call always setText before render; another option may be to use separates VBO per each text you want to render 
                         * @param text
                         * @throw IllegalStateException if textRender has not set yet
                         */
                        virtual void setText(const std::string &text){
                            this->textRender->setText(text);
                        }
                        /**
                         * @return current rendered text or empty string if textRender has not set yet
                         */
                        std::string getText() const noexcept{
                            return this->textRender->getText();
                        }

                        void settextRender(_text::TextRender* tr){
                            if(tr == nullptr){
                                throw jpl::_exception::IllegalArgumentException("text Render cannot be nullptr");
                            }
                            this->textRender = tr;
                        }
                        const _text::TextRender* getTextRender() const noexcept{
                            return this->textRender;
                        }

                        void click(){
                            this->clickFunction();
                        }
                        void setOnClick( std::function<void()> f){
                            this->clickFunction = std::move(f);
                        }
                        /**
                         * Render button
                         */
                        virtual void render() const;
                };
            }
        }
    }
}

#endif