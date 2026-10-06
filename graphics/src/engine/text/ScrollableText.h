#pragma once
#include "TextRender.h"
#include <algorithm>
#include <string>

// Widget di testo scrollabile: possiede testo, scroll e cache del layout.
// Il TextRender (condiviso) resta senza stato.
struct ScrollableText {
    Rect  area;
    float scrollY = 0;

    void setText(std::string t) { text_ = std::move(t); dirty_ = true; scrollY = 0; }
    void scrollBy(float dy)     { scrollY += dy; clampScroll(); }

    void draw(TextRender& tr, float scale, Color c) {
        if (dirty_ || cachedWidth_ != area.w || cachedScale_ != scale) {
            layout_ = tr.layout(text_, area.w, scale);
            cachedWidth_ = area.w; cachedScale_ = scale; dirty_ = false;
            clampScroll();
        }
        tr.flush();   // scarica il testo gia' in coda, senza scissor
        tr.drawLayout(text_, layout_, area, scrollY, c);
        tr.flush(&area);
    }

private:
    void clampScroll() {
        float maxScroll = std::max(0.0f, layout_.height - area.h);
        scrollY = std::clamp(scrollY, 0.0f, maxScroll);
    }
    std::string text_;
    TextLayout  layout_;
    bool  dirty_ = true;
    float cachedWidth_ = -1, cachedScale_ = -1;
};
