/**
 * TextRender provides a way to render some text on the screen. 
 * 
 * EBO used within its context must be already loaded with indices on your own. Since this class uses glDrawElements but does not push
 * indices via glBufferData, undefined behaviour may occurr.
 * Also, calling setText vertices into VBO are edited, although that function does not bind any VBO before. It is up to you to bind it
 */

#ifndef TEXTRENDER_GRAPHICS_JPL
#define TEXTRENDER_GRAPHICS_JPL

#include "Font.hpp"
#include "../../shaders/ProgramManager.hpp"
#include "../VAO.hpp"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "../../Metrics.hpp"
#include "../interfaces/IClickable.hpp"
#include "../interfaces/ITextEditable.hpp"



namespace jpl{
    namespace _graphics{

        namespace _engine{

            namespace _text{
                class TextRender : public IClickable, public ITextEditable{

                    protected:

                        float r,g,b,a;
                        //It is updated by setText in order to prevent any ovewrflow during render call
                        unsigned int charsToRender;
                        //In NDC
                        float offsetTexX, offsetTexY;
                        float offsetX, offsetY;

                        float sizeFont;
                        const Font* font;

                        bool focused;
                        bool editable;
                        _shaders::ProgramShaders* ps;
                        VAO* vao;

                    public:
                        /**
                         * @param ps programShader
                         * @param x
                         * @param y
                         * @param w
                         * @param h
                         */
                        TextRender(float x, float y, float w, float h);

                        void setFont(Font* font);

                        void setRGBA(float r, float g, float b, float a);
                        
                        
                        virtual void setText(const std::string &text) override;

                        virtual void render(Painter* painter) override;
                        void render(const std::string &text, float x, float y, float w, float h, float r, float g, float b, float a);

                        void setVAOAndPS(VAO* vao, _shaders::ProgramShaders* ps) noexcept{
                            this->vao = vao;
                            this->ps = ps;
                        }

                        virtual void click() override{
                            if(this->editable)
                                this->setFocused(true);
                        }
                        void setFocused(bool focused) noexcept{
                            this->focused = focused;
                        }
                        bool isFocused() const noexcept{
                            return this->focused;
                        }

                        void setEditable(bool editable) noexcept{
                            this->editable = editable;
                        }
                        bool isEditable() const noexcept{
                            return this->editable;
                        }

                        void setFontSize(float fontSize) noexcept{
                            this->sizeFont = fontSize;
                        }
                        float getFontSize() const noexcept{
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