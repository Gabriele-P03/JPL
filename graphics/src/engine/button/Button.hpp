/**
 * A button is a clickable 2D element which call an event once clicked.
 * It may render text inside of it or even texture.
 *
 * The text is drawn through a shared, stateless TextRender passed to renderLabel().
 * The Button only stores its label (string, scale, color) and never owns a TextRender.
 * The Button owns its Texture and deletes it in the destructor.
 */

#ifndef BUTTON_GRAPHICS_JPL
#define BUTTON_GRAPHICS_JPL

#include <string>
#include "../text/TextRender.hpp"
#include "../interfaces/IClickable.hpp"
#include "../VAO.hpp"
#include "../Painter.hpp"

namespace jpl{
    namespace _graphics{
        namespace _engine{
            namespace _button{

                class Button : public IClickable{

                    protected:

                        _texture::Texture* texture;

                        std::string       label;
                        float             labelScale = 1.0f;
                        glm::vec4      labelColor = {1.0f, 1.0f, 1.0f, 1.0f};
                        glm::vec2       labelSize  = {0.0f, 0.0f};   // measure()'s cache
                        bool              labelDirty = true;

                    public:

                        Button(float x, float y, float w, float h, _texture::Texture* texture);

                        /**
                         * Sets the label. No GL calls are made here.
                         */
                        virtual void setText(const std::string &text);

                        const std::string& getText() const noexcept{
                            return this->label;
                        }

                        void setTextScale(float scale);
                        void setTextColor(const glm::vec4 &color) noexcept{
                            this->labelColor = color;   // il colore non influenza la misura
                        }

                        /**
                         * Renders only the button's texture (background).
                         * Please, ensure to have the right VBO already binded since this function calls glBufferSubData
                         */
                        void render(Painter* painter);

                        /**
                         * Queues the label (centered) into the shared TextRender.
                         * It does NOT flush: call tr.flush() once, after all the backgrounds
                         * that must stay under the text have been rendered.
                         */
                        virtual void renderLabel(_text::TextRender &tr);

                        virtual ~Button(){
                            delete this->texture;
                        }
                };
            }
        }
    }
}

#endif
