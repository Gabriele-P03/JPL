#include "TextInput.hpp"
#include <algorithm>
#include <cmath>
#include <string_view>

namespace jpl{
namespace _graphics{
namespace _engine{
namespace _input{

namespace {

size_t prevChar(const std::string &s, size_t i){
    if(i == 0)
        return 0;
    do { --i; } while(i > 0 && (static_cast<unsigned char>(s[i]) & 0xC0) == 0x80);
    return i;
}

size_t nextChar(const std::string &s, size_t i){
    if(i >= s.size())
        return s.size();
    do { ++i; } while(i < s.size() && (static_cast<unsigned char>(s[i]) & 0xC0) == 0x80);
    return i;
}

std::string encodeUtf8(unsigned int cp){
    std::string out;
    if(cp < 0x80){
        out += static_cast<char>(cp);
    }else if(cp < 0x800){
        out += static_cast<char>(0xC0 | (cp >> 6));
        out += static_cast<char>(0x80 | (cp & 0x3F));
    }else if(cp < 0x10000){
        out += static_cast<char>(0xE0 | (cp >> 12));
        out += static_cast<char>(0x80 | ((cp >> 6) & 0x3F));
        out += static_cast<char>(0x80 | (cp & 0x3F));
    }else{
        out += static_cast<char>(0xF0 | (cp >> 18));
        out += static_cast<char>(0x80 | ((cp >> 12) & 0x3F));
        out += static_cast<char>(0x80 | ((cp >> 6) & 0x3F));
        out += static_cast<char>(0x80 | (cp & 0x3F));
    }
    return out;
}

}

TextInput::TextInput(float x, float y, float w, float h, _texture::Texture* background) : IClickable(x,y,w,h), ITextEditable("") {
    this->area = {x, y, w, h};
    this->background = background;
}

void TextInput::setText(const std::string &t){
    this->text = t;
    this->cursor = this->anchor = this->text.size();
    this->resetBlink();
}

void TextInput::setFocus(bool f){
    this->focused = f;
    if(f){
        this->resetBlink();
    }else{
        this->dragging = false;
        this->anchor = this->cursor;     // collapse selection on blur
    }
}

bool TextInput::contains(double px, double py) const noexcept{
    return px >= this->area.x && px < this->area.x + this->area.z &&
           py >= this->area.y && py < this->area.y + this->area.w;
}

void TextInput::resetBlink(){
    this->caretResetTime = glfwGetTime();   // caret visible right after any action
}

bool TextInput::caretVisible() const{
    double t = glfwGetTime() - this->caretResetTime;
    return std::fmod(t, this->blinkPeriod) < this->blinkPeriod * 0.5;
}

void TextInput::moveCursor(size_t pos, bool extendSelection){
    this->cursor = pos;
    if(!extendSelection) this->anchor = pos;
    this->resetBlink();
}

bool TextInput::deleteSelection(){
    if(!this->hasSelection()) return false;
    size_t b = this->selBegin(), e = this->selEnd();
    this->text.erase(b, e - b);
    this->cursor = this->anchor = b;
    return true;
}

void TextInput::insert(std::string s){
    this->deleteSelection();
    // Do not exceed maxBytes, and never cut a UTF-8 character in half
    size_t room = this->maxBytes > this->text.size() ? this->maxBytes - this->text.size() : 0;
    if(s.size() > room){
        size_t n = room;
        while(n > 0 && (static_cast<unsigned char>(s[n]) & 0xC0) == 0x80) --n;
        s.resize(n);
    }
    if(s.empty()) return;

    this->text.insert(this->cursor, s);
    this->cursor += s.size();
    this->anchor = this->cursor;
    this->resetBlink();
}

float TextInput::localX(double mx) const noexcept{
    return static_cast<float>(mx) - (this->area.x + this->padding) + this->scrollX;
}

void TextInput::onChar(unsigned int cp){
    if(!this->focused) return;
    if(cp < 32 || cp == 127) return;          // control characters
    this->insert(encodeUtf8(cp));
}

void TextInput::onKey(GLFWwindow* window, int key, int action, int mods){
    if(!this->focused) return;
    if(action != GLFW_PRESS && action != GLFW_REPEAT) return;   // REPEAT: tenere premuto

    const bool shift = (mods & GLFW_MOD_SHIFT) != 0;
    const bool ctrl  = (mods & (GLFW_MOD_CONTROL | GLFW_MOD_SUPER)) != 0;   // Super = Cmd su macOS

    switch(key){
        case GLFW_KEY_LEFT:
            if(!shift && this->hasSelection()) this->moveCursor(this->selBegin(), false);
            else this->moveCursor(prevChar(this->text, this->cursor), shift);
            break;

        case GLFW_KEY_RIGHT:
            if(!shift && this->hasSelection()) this->moveCursor(this->selEnd(), false);
            else this->moveCursor(nextChar(this->text, this->cursor), shift);
            break;

        case GLFW_KEY_HOME: this->moveCursor(0, shift); break;
        case GLFW_KEY_END:  this->moveCursor(this->text.size(), shift); break;

        case GLFW_KEY_BACKSPACE:
            if(!this->deleteSelection() && this->cursor > 0){
                size_t p = prevChar(this->text, this->cursor);
                this->text.erase(p, this->cursor - p);
                this->cursor = this->anchor = p;
            }
            this->resetBlink();
            break;

        case GLFW_KEY_DELETE:
            if(!this->deleteSelection() && this->cursor < this->text.size()){
                size_t n = nextChar(this->text, this->cursor);
                this->text.erase(this->cursor, n - this->cursor);
            }
            this->resetBlink();
            break;

        case GLFW_KEY_ENTER:
        case GLFW_KEY_KP_ENTER:
            if(this->onSubmit) this->onSubmit(this->text);
            break;

        case GLFW_KEY_ESCAPE:
            this->setFocus(false);
            break;

        case GLFW_KEY_A:
            if(ctrl){ this->anchor = 0; this->cursor = this->text.size(); this->resetBlink(); }
            break;

        case GLFW_KEY_C:
            if(ctrl && this->hasSelection()){
                std::string sel = this->text.substr(this->selBegin(), this->selEnd() - this->selBegin());
                glfwSetClipboardString(window, sel.c_str());
            }
            break;

        case GLFW_KEY_X:
            if(ctrl && this->hasSelection()){
                std::string sel = this->text.substr(this->selBegin(), this->selEnd() - this->selBegin());
                glfwSetClipboardString(window, sel.c_str());
                this->deleteSelection();
                this->resetBlink();
            }
            break;

        case GLFW_KEY_V:
            if(ctrl){
                const char* clip = glfwGetClipboardString(window);
                if(clip){
                    std::string s;
                    for(const char* p = clip; *p; ++p){
                        unsigned char c = static_cast<unsigned char>(*p);
                        if(c >= 32 && c != 127) s += *p;   // drops newlines and control chars
                    }
                    this->insert(s);
                }
            }
            break;

        default: break;
    }
}

bool TextInput::onMouseDown(const _text::TextRender &tr, double mx, double my, bool shift){
    if(!this->contains(mx, my)){
        this->setFocus(false);
        return false;
    }
    if(!this->focused) this->setFocus(true);
    this->dragging = true;
    this->moveCursor(tr.indexAtX(this->text, this->localX(mx), this->scale), shift);
    return true;
}

void TextInput::onMouseDrag(const _text::TextRender &tr, double mx, double my){
    (void)my;
    if(!this->focused || !this->dragging) return;
    this->moveCursor(tr.indexAtX(this->text, this->localX(mx), this->scale), true);
}

// ---------------------------------------------------------------- rendering

void TextInput::renderBackground(Painter* painter){
    if(this->background != nullptr){
        painter->render(this->background, this->area.x, this->area.y, this->area.z, this->area.w);
    }
}

void TextInput::renderText(_text::TextRender &tr){
    const std::string_view sv(this->text);
    const float viewW  = this->area.w - 2.0f * this->padding;
    const float caretW = std::max(1.0f, std::round(this->scale));
    const float lineH  = tr.measure("", this->scale).y;

    // Keep the caret inside the visible area
    const float caretX = tr.measure(sv.substr(0, this->cursor), this->scale).x;
    const float totalW = tr.measure(sv, this->scale).x;
    if(totalW + caretW <= viewW){
        this->scrollX = 0.0f;
    }else{
        if(caretX - this->scrollX > viewW - caretW) this->scrollX = caretX - (viewW - caretW);
        if(caretX - this->scrollX < 0.0f)           this->scrollX = caretX;
        this->scrollX = std::clamp(this->scrollX, 0.0f, totalW + caretW - viewW);
    }

    const float textX = this->area.x + this->padding - this->scrollX;
    const float topY  = this->area.y + this->area.w * 0.5f - tr.capCenterOffset(this->scale);

    // Whatever is already queued must not be clipped by this field's scissor
    tr.flush();

    // 1. selection (under the text)
    if(this->hasSelection()){
        float xa = tr.measure(sv.substr(0, this->selBegin()), this->scale).x;
        float xb = tr.measure(sv.substr(0, this->selEnd()),   this->scale).x;
        tr.fillRect({textX + xa, topY, xb - xa, lineH}, this->selectionColor);
    }

    // 2. text
    tr.draw(sv, textX, topY, this->scale, this->textColor);

    // 3. blinking caret (over the text)
    if(this->focused && this->caretVisible()){
        tr.fillRect({std::round(textX + caretX), topY, caretW, lineH}, this->caretColor);
    }

    glm::vec4 clip = {this->area.x + this->padding, this->area.y, viewW, this->area.w};
    tr.flush(&clip);
}

}}}}
