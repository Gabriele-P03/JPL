#pragma once

#include "TextRender.h"
#include <algorithm>
#include <string>

namespace jpl::_graphics::_engine::_text::TextRender {
    struct ScrollableText {

        private:
            void clampScroll() {
                float maxScroll = std::max(0.0f, layout_.height - area.h);
                scrollY = std::clamp(scrollY, 0.0f, maxScroll);
            }
            std::string text_;
            TextLayout  layout_;
            bool  dirty_ = true;
            float cachedWidth_ = -1, cachedScale_ = -1;

        public:
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
                tr.flush();
                tr.drawLayout(text_, layout_, area, scrollY, c);
                tr.flush(&area);
            }
    };
}