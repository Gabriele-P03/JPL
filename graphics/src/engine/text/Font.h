#pragma once

#include <unordered_map>
#include <GL/glew.h>
#include "../../texture/Texture.hpp"

namespace jpl::_graphics::_engine::_text{
    struct Glyph {
        float u0 = 0, v0 = 0, u1 = 0, v1 = 0; // Atlas' UV
        float w = 0, h = 0;                   // Quad Dimension
        float xoff = 0, yoff = 0;             // offset dall'angolo alto-sinistro della riga
        float advance = 0;                    // cursor offset
        bool valid = false;
    };

    struct Font {
        GLuint texture;
        float  lineHeight = 0;
        std::unordered_map<uint32_t, Glyph> glyphs;
        float  whiteU = 0, whiteV = 0;  //used by fillRect
    };
}