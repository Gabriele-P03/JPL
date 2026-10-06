#include "FontLoader.h"

#define STB_TRUETYPE_IMPLEMENTATION
#include "stb_truetype.h"   // https://github.com/nothings/stb

#include <cstdio>
#include <fstream>
#include <iterator>
#include <vector>

bool loadFontTTF(Font& out, const char* path, float pixelHeight, int atlasSize) {
    // 1. Leggi il file .ttf in memoria
    std::ifstream f(path, std::ios::binary);
    if (!f) {
        std::fprintf(stderr, "FontLoader: impossibile aprire '%s'\n", path);
        return false;
    }
    std::vector<unsigned char> ttf((std::istreambuf_iterator<char>(f)),
                                    std::istreambuf_iterator<char>());

    // 2. Rasterizza i glifi in un bitmap 8 bit (1 byte per pixel, origine in alto)
    constexpr int kFirst = 32, kCount = 96;
    std::vector<unsigned char> bitmap(atlasSize * atlasSize);
    stbtt_bakedchar baked[kCount];

    int res = stbtt_BakeFontBitmap(ttf.data(), 0, pixelHeight,
                                   bitmap.data(), atlasSize, atlasSize,
                                   kFirst, kCount, baked);
    if (res <= 0) {   // <= 0: non tutti i glifi stanno nell'atlas
        std::fprintf(stderr, "FontLoader: atlas %dx%d troppo piccolo per '%s' a %.0fpx\n",
                     atlasSize, atlasSize, path, pixelHeight);
        return false;
    }

    // 3. Metriche verticali (stb_truetype da' gli offset rispetto alla baseline,
    //    il nostro Glyph li vuole rispetto all'alto della riga)
    stbtt_fontinfo info;
    stbtt_InitFont(&info, ttf.data(), stbtt_GetFontOffsetForIndex(ttf.data(), 0));
    float scale = stbtt_ScaleForPixelHeight(&info, pixelHeight);
    int asc, desc, gap;
    stbtt_GetFontVMetrics(&info, &asc, &desc, &gap);
    float ascent = asc * scale;

    out.lineHeight = (asc - desc + gap) * scale;

    // 4. Riempi la tabella glifi
    const float inv = 1.0f / atlasSize;
    for (int i = 0; i < kCount; ++i) {
        const stbtt_bakedchar& b = baked[i];
        Glyph& g = out.glyphs[kFirst + i];
        g.u0 = b.x0 * inv;  g.v0 = b.y0 * inv;
        g.u1 = b.x1 * inv;  g.v1 = b.y1 * inv;
        g.w  = float(b.x1 - b.x0);
        g.h  = float(b.y1 - b.y0);
        g.xoff    = b.xoff;
        g.yoff    = ascent + b.yoff;   // baseline -> alto della riga
        g.advance = b.xadvance;
        g.valid   = true;              // lo spazio ha w = h = 0: draw() lo salta ma avanza
    }

    // 5. Carica l'atlas su GPU come texture a canale singolo
    glGenTextures(1, &out.texture);
    glBindTexture(GL_TEXTURE_2D, out.texture);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);   // righe da 1 byte: serve per GL_RED
    glTexImage2D(GL_TEXTURE_2D, 0, GL_R8, atlasSize, atlasSize, 0,
                 GL_RED, GL_UNSIGNED_BYTE, bitmap.data());
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    return true;
}

void destroyFont(Font& f) {
    if (f.texture) glDeleteTextures(1, &f.texture);
    f.texture = 0;
}
