#pragma once

#include "Font.h"
#include <string_view>
#include <vector>
#include <glm/glm.hpp>

#include "../interfaces/ITextEditable.hpp"
#include "../../shaders/ProgramShaders.hpp"

namespace jpl::_graphics::_engine::_text {
    struct TextLayout {
        struct Line { size_t begin, end; float width; };
        std::vector<Line> lines;
        float height = 0;
        float scale  = 1;
    };

    class TextRender : public _graphics::_engine::ITextEditable {
    public:
        TextRender() = default;
        void init(_graphics::_shaders::ProgramShaders* psText);
        void shutdown();

        void setFont(const Font* font) { font_ = font; }
        void setScreenSize(int w, int h) { screenW_ = w; screenH_ = h; }

        void draw(std::string_view s, float x, float y, float scale, glm::vec4 c);

        /**
         *  Measure without drawing (manages \n, do not wrap).
        */
        glm::vec2 measure(std::string_view s, float scale) const;

        TextLayout layout(std::string_view s, float maxWidth, float scale) const;

        void drawLayout(std::string_view s, const TextLayout& L,
                        const glm::vec4& area, float scrollY, glm::vec4 c);

        /**
         * After have called draw() all times you need, you have to call this
         * function to render them
         * @param clip
         */
        void flush(const glm::vec4* clip = nullptr);

        float capCenterOffset(float scale) const;

        size_t pendingGlyphs() const { return instances_.size(); }

        /**
         * @param s
         * @param x
         * @param scale
         * @return right-side of the nearest byte to the given x
         */
        size_t indexAtX(std::string_view s, float x, float scale) const;

        /**
         * Draw carter rect
         * @p x,y,w,h
         * @c r,g,b,a
        */
        void fillRect(const glm::vec4& p, const glm::vec4 &c);

    private:
        struct GlyphInstance {            // 48 byte
            float pos[2];
            float size[2];
            float uv[4];                  // u0, v0, u1, v1
            float color[4];
        };

        const Glyph& glyphFor(uint32_t c) const;

        const Font* font_ = nullptr;
        int screenW_ = 1, screenH_ = 1;

        std::vector<GlyphInstance> instances_;
        _graphics::_shaders::ProgramShaders* ps;
        GLuint vao_ = 0, quadVbo_ = 0, instVbo_ = 0;
        GLint  uScreenLoc_ = -1, uAtlasLoc_ = -1;
        size_t instCapacity_ = 0;
    };
}
