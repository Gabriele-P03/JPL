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

                //template<typename...Types>
                class Button{

                    protected:
                        
                        _text::TextRender* textRender;
                        std::string text;

                        _texture::Texture* texture;
                        Painter* painter;

                        unsigned int posX, posY, width, height;

                        float r,g,b,a;
                        //Colors for the text
                        float tr, tg, tb, ta;

                        std::function<void()> clickFunction;

                    public:

                        Button(unsigned int posX, unsigned int posY, unsigned int width, unsigned int height)
                            : posX(posX), posY(posY), width(width), height(height){
                                this->texture = nullptr;
                        }

                        unsigned int getPosX() const noexcept{return this->posX;}
                        unsigned int getPosY() const noexcept{return this->posY;}
                        unsigned int getWidth() const noexcept{return this->width;}
                        unsigned int getHeight() const noexcept{return this->height;}
                        float getRed() const noexcept {return this->r;}
                        float getGreen() const noexcept {return this->g;}
                        float getBlue() const noexcept {return this->b;}
                        float getAlpha() const noexcept {return this->a;}
                        float getTextRed() const noexcept {return this->tr;}
                        float getTextGreen() const noexcept {return this->tg;}
                        float getTextBlue() const noexcept {return this->tb;}
                        float getTextAlpha() const noexcept {return this->ta;}

                        /**
                         * @param textRender
                         * @throw IllegalArgumentException if textRender is nullptr
                         */
                        void setTextRender(_text::TextRender* textRender);

                        /**
                         * @param text
                         * @throw IllegalStateException if textRender has not set yet
                         */
                        void setText(const std::string &text){
                            if(this->textRender == nullptr){
                                throw jpl::_exception::IllegalStateException("TextRender has not set, yet");
                            }
                            this->text = text;
                        }
                        /**
                         * @return current rendered text or empty string if textRender has not set yet
                         */
                        std::string getText() const noexcept{
                            if(this->textRender == nullptr){
                                return "";
                            }
                            return this->textRender->getText();
                        }

                        const _text::TextRender* getTextRender() const noexcept{
                            return this->textRender;
                        }

                        /**
                         * @param r red
                         * @param g green
                         * @param b blue
                         * @param a alpha
                         * @throw IllegalArgumentException if any of the four values are less than 0 or greater than 1                         
                         */
                        void setBackgroundColor(float r, float g, float b, float a);


                        /**
                         * @param painter
                         * @throw IllegalArgumentException if painter is nullptr
                         */
                        void setPainter(Painter* painter);

                        /**
                         * @param texture
                         * @throw IllegalArgumentException if texture is nullptr
                         * @throw IllegalStateException if painter has not been set, yet
                         */
                        void setBackgroundTexture(_texture::Texture* texture);

                        void click(){
                            this->clickFunction();
                        }
                        void setOnClick( std::function<void()> f){
                            this->clickFunction = std::move(f);
                        }

                        /**
                         * Render text
                         */
                        virtual void renderText() const;
                        /**
                         * Render background texture
                         */
                        virtual void renderTexture() const;
                        /**
                         * Render button. 
                         * This is the only function that should be invoked
                         */
                        virtual void render() const;
                };
            }
        }
    }
}

#endif