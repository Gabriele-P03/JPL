#include "FontLoader.h"

#define STB_TRUETYPE_IMPLEMENTATION
#include <stb_truetype.h>

#include <cstdio>
#include <fstream>
#include <iterator>
#include <vector>
#include <jpl/utils/FilesUtils.hpp>

void loadFontTTF(jpl::_graphics::_engine::_text::Font& out, const std::string &path, float pixelHeight, int atlasSize) {

    std::fstream file;
    std::fstream* file_ptr = &file;
    jpl::_utils::_files::getLocalFile(path, std::ios_base::binary, &file_ptr);
    std::vector<unsigned char> ttf((std::istreambuf_iterator<char>(file)),
                                    std::istreambuf_iterator<char>());

    // Rasterize glyphs into an 8-bit bitmap (1 byte per pixel, top-left origin)
    constexpr int kFirst = 32, kCount = 96;
    std::vector<unsigned char> bitmap(atlasSize * atlasSize);
    stbtt_bakedchar baked[kCount];

    int res = stbtt_BakeFontBitmap(ttf.data(), 0, pixelHeight,
                                   bitmap.data(), atlasSize, atlasSize,
                                   kFirst, kCount, baked);
    if (res <= 0) {   // <= 0: non tutti i glifi stanno nell'atlas
        std::string msg = std::format("FontLoader: atlas {}x{} troppo piccolo per '{}' a {}px",atlasSize, atlasSize, path.c_str(), std::to_string(pixelHeight).c_str());
        throw jpl::_exception::RuntimeException(msg);
    }

    for (int y = atlasSize - 2; y < atlasSize; ++y)
        for (int x = atlasSize - 2; x < atlasSize; ++x)
            bitmap[y * atlasSize + x] = 255;
    out.whiteU = (atlasSize - 1) / float(atlasSize);
    out.whiteV = (atlasSize - 1) / float(atlasSize);

    // stb_truetype gives offsets relative to the baseline, but Glyph need them relative to the top of the line
    stbtt_fontinfo info;
    stbtt_InitFont(&info, ttf.data(), stbtt_GetFontOffsetForIndex(ttf.data(), 0));
    float scale = stbtt_ScaleForPixelHeight(&info, pixelHeight);
    int asc, desc, gap;
    stbtt_GetFontVMetrics(&info, &asc, &desc, &gap);
    float ascent = asc * scale;

    out.lineHeight = (asc - desc + gap) * scale;

    // 4. Filling glyphs
    const float inv = 1.0f / atlasSize;
    for (int i = 0; i < kCount; ++i) {
        const stbtt_bakedchar& b = baked[i];
        jpl::_graphics::_engine::_text::Glyph& g = out.glyphs[kFirst + i];
        g.u0 = b.x0 * inv;  g.v0 = b.y0 * inv;
        g.u1 = b.x1 * inv;  g.v1 = b.y1 * inv;
        g.w  = float(b.x1 - b.x0);
        g.h  = float(b.y1 - b.y0);
        g.xoff    = b.xoff;
        g.yoff    = ascent + b.yoff;   // baseline -> alto della riga
        g.advance = b.xadvance;
        g.valid   = true;              // lo spazio ha w = h = 0: draw() lo salta ma avanza
    }

    // Load atlas as single-channel texture
    glGenTextures(1, &out.texture);
    glBindTexture(GL_TEXTURE_2D, out.texture);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);   // righe da 1 byte: serve per GL_RED
    glTexImage2D(GL_TEXTURE_2D, 0, GL_R8, atlasSize, atlasSize, 0,
                 GL_RED, GL_UNSIGNED_BYTE, bitmap.data());
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
}

void destroyFont(jpl::_graphics::_engine::_text::Font& f) {
    if (f.texture)
        glDeleteTextures(1, &f.texture);
    f.texture = 0;
}
