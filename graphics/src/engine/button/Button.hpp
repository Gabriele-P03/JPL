/**
 * A button is a clickable 2D element which call an event once clicked.
 * It may render text inside of it or even texture.
 * 
 * TextRenderer is not freed via Button's destructor, it is up to you to deallocate it
 */

#ifndef BUTTON_GRAPHICS_JPL
#define BUTTON_GRAPHICS_JPL

#include "../text/TextRender.hpp"
#include <functional>
#include "../VAO.hpp"

namespace jpl{
    namespace _graphics{
        namespace _engine{
            namespace _button{

                class Button{

                    protected:
                        
                        _text::TextRender* textRender;

                        _texture::Texture* texture;

                        unsigned int programIndex, projectionLocation;
                        float x, y, w, h;

                        _shaders::ProgramShaders* psTextRenderer;
                        VAO* vaoTextRenderer;

                        static constexpr unsigned int indices[6] = {
                            0,1,2,
                            0,1,3
                        };

                        std::function<void()> clickFunction;

                    public:

                        Button(unsigned int programIndex, float x, float y, float w, float h, _text::TextRender* textRender, _texture::Texture* texture);

                        float getX() const noexcept{return this->x;}
                        float getY() const noexcept{return this->y;}
                        float getW() const noexcept{return this->w;}
                        float getH() const noexcept{return this->h;}
                        virtual void setPos(float x, float y, float w, float h);

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

                        void setVAOTextRenderer(VAO* vao){
                            this->vaoTextRenderer = vao;
                        }
                        VAO* getVAOTextRenderer() const noexcept{
                            return this->vaoTextRenderer;
                        }

                        ~Button(){
                            delete this->texture;
                        }
                };
            }
        }
    }
}

#endif