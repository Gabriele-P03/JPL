#pragma once
#include "Types.h"
#include "Font.h"
#include "Shader.h"
#include <string_view>
#include <vector>

// Risultato del wrap: indici nella stringa originale (non copia il testo).
struct TextLayout {
    struct Line { size_t begin, end; float width; };
    std::vector<Line> lines;
    float height = 0;
    float scale  = 1;
};

class TextRender {
public:
    // Percorsi dei file shader (relativi alla working directory o assoluti).
    bool init(const std::string& vertPath, const std::string& fragPath);
    void shutdown();

    void setFont(const Font* font) { font_ = font; }
    void setScreenSize(int w, int h) { screenW_ = w; screenH_ = h; }

    // --- accodamento (nessuna chiamata GL) ---
    // (x, y) = angolo alto-sinistro della prima riga, pixel, origine in alto a sinistra.
    void draw(std::string_view s, float x, float y, float scale, Color c);

    // Misura senza disegnare (gestisce '\n', non fa wrap).
    Vec2 measure(std::string_view s, float scale) const;

    // --- layout con a capo automatico ---
    TextLayout layout(std::string_view s, float maxWidth, float scale) const;

    // Accoda solo le righe visibili in `area`, tenendo conto dello scroll.
    // Chiama poi flush(&area) per tagliare le righe parziali con lo scissor.
    void drawLayout(std::string_view s, const TextLayout& L,
                    const Rect& area, float scrollY, Color c);

    // --- upload + una sola draw call istanziata ---
    // clip != nullptr -> glScissor sul rettangolo (pixel, origine in alto).
    void flush(const Rect* clip = nullptr);

    size_t pendingGlyphs() const { return instances_.size(); }

private:
    struct GlyphInstance {            // 48 byte
        float pos[2];
        float size[2];
        float uv[4];                  // u0, v0, u1, v1
        float color[4];
    };

    const Glyph& glyphFor(unsigned char c) const;

    const Font* font_ = nullptr;
    int screenW_ = 1, screenH_ = 1;

    std::vector<GlyphInstance> instances_;

    Shader shader_;
    GLuint vao_ = 0, quadVbo_ = 0, instVbo_ = 0;
    GLint  uScreenLoc_ = -1, uAtlasLoc_ = -1;
    size_t instCapacity_ = 0;         // in istanze
};
