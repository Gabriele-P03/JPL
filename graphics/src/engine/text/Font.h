#pragma once
#include <glad/glad.h>   // cambia con il tuo loader

// Un glifo dell'atlas. Coordinate in pixel, scalate poi da `scale`.
struct Glyph {
    float u0 = 0, v0 = 0, u1 = 0, v1 = 0; // UV nell'atlas (0..1)
    float w = 0, h = 0;                   // dimensione del quad
    float xoff = 0, yoff = 0;             // offset dall'angolo alto-sinistro della riga
    float advance = 0;                    // avanzamento del cursore
    bool  valid = false;
};

// Font bitmap: atlas a canale singolo (GL_RED) + tabella glifi (byte 0..255).
// Per UTF-8 completo sostituisci l'array con una unordered_map<uint32_t, Glyph>.
struct Font {
    GLuint texture    = 0;
    float  lineHeight = 0;
    Glyph  glyphs[256];
};
