/**
 * Single-line editable text field.
 *
 * It owns text, cursor, selection, scroll and focus; the shared stateless TextRender only
 * draws glyphs and rects. The background Texture (optional) is drawn through the Painter
 * and it is deleted by the destructor, like in Button.
 *
 * Frame order:
 *   1. renderBackground(painter) for every widget (sprites)
 *   2. renderText(tr)            for every widget (it flushes the TextRender by itself,
 *                                 with a scissor on the field, like ScrollableText)
 *
 * Events must be forwarded by the application (see onChar, onKey, onMouseDown...).
 * The blinking caret uses glfwGetTime(), so the field must be rendered every frame.
 */

#ifndef TEXTINPUT_GRAPHICS_JPL
#define TEXTINPUT_GRAPHICS_JPL

#include <functional>
#include <string>
#include "../text/TextRender.hpp"
#include "../interfaces/IClickable.hpp"   // as in Button.hpp: brings Painter / Texture
#include "../VAO.hpp"
#include <GLFW/glfw3.h>                    // after the GL loader

#include "engine/Painter.hpp"

namespace jpl{
    namespace _graphics{
        namespace _engine{
            namespace _input{

                class TextInput : public ITextEditable, public IClickable{

                    public:

                        using SubmitCallback = std::function<void(const std::string&)>;

                        // Appearance (public on purpose: tweak freely)
                        glm::vec4 textColor      = {1.0f, 1.0f, 1.0f, 1.0f};
                        glm::vec4 caretColor     = {1.0f, 1.0f, 1.0f, 1.0f};
                        glm::vec4 selectionColor = {0.25f, 0.45f, 0.90f, 0.55f};
                        float        scale          = 1.0f;
                        float        padding        = 4.0f;    // inner horizontal padding (px)
                        double       blinkPeriod    = 1.0;     // seconds: half visible, half hidden
                        size_t       maxBytes       = 256;     // max UTF-8 bytes of the content

                        SubmitCallback onSubmit;               // called on Enter

                        TextInput(float x, float y, float w, float h, _texture::Texture* background = nullptr);
                        TextInput(const TextInput&) = delete;
                        TextInput& operator=(const TextInput&) = delete;
                        virtual ~TextInput(){
                            delete this->background;
                        }

                        void setFocus(bool focus_) override;

                        void setText(const std::string &t) override;

                        bool contains(double px, double py) const noexcept;

                        /** glfwSetCharCallback: inserts the typed codepoint. */
                        void onChar(unsigned int codepoint);
                        /** glfwSetKeyCallback: arrows, Home/End, Backspace/Delete, Enter, Ctrl+A/C/X/V, Esc. */
                        void onKey(GLFWwindow* window, int key, int action, int mods);
                        /** Left mouse button pressed. Focuses the field if inside (and blurs it if outside).
                         *  @return true if the click was inside. */
                        bool onMouseDown(const _text::TextRender &tr, double mx, double my, bool shift);
                        void onMouseUp() noexcept{ this->dragging = false; }
                        /** Cursor moved while the left button is held: extends the selection. */
                        void onMouseDrag(const _text::TextRender &tr, double mx, double my);

                        // ---- rendering ----
                        virtual void renderBackground(Painter* painter);
                        virtual void renderText(_text::TextRender &tr);

                    protected:

                        glm::vec4        area;
                        _texture::Texture* background;

                        std::string text;
                        size_t cursor = 0;       // byte index, always on a UTF-8 character boundary
                        size_t anchor = 0;       // selection = [anchor, cursor)
                        float  scrollX = 0.0f;
                        bool   focused = false;
                        bool   dragging = false;
                        double caretResetTime = 0.0;

                        bool   hasSelection() const noexcept{ return cursor != anchor; }
                        size_t selBegin() const noexcept{ return cursor < anchor ? cursor : anchor; }
                        size_t selEnd()   const noexcept{ return cursor < anchor ? anchor : cursor; }

                        void resetBlink();
                        bool caretVisible() const;
                        void moveCursor(size_t pos, bool extendSelection);
                        bool deleteSelection();
                        void insert(std::string s);
                        float localX(double mx) const noexcept;
                };
            }
        }
    }
}

#endif
