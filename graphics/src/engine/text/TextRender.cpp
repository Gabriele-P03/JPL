#include "TextRender.h"
#include <algorithm>
#include <cmath>
#include <cstddef>

// ---------------------------------------------------------------- init

bool TextRender::init(const std::string& vertPath, const std::string& fragPath) {
    if (!shader_.loadFromFiles(vertPath, fragPath)) return false;

    uScreenLoc_ = shader_.uniform("uScreen");
    uAtlasLoc_  = shader_.uniform("uAtlas");

    // Quad unitario come triangle strip
    const float quad[] = { 0,0,  1,0,  0,1,  1,1 };

    glGenVertexArrays(1, &vao_);
    glGenBuffers(1, &quadVbo_);
    glGenBuffers(1, &instVbo_);

    glBindVertexArray(vao_);

    glBindBuffer(GL_ARRAY_BUFFER, quadVbo_);
    glBufferData(GL_ARRAY_BUFFER, sizeof quad, quad, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);

    glBindBuffer(GL_ARRAY_BUFFER, instVbo_);
    const GLsizei stride = sizeof(GlyphInstance);
    auto attr = [&](GLuint loc, GLint n, size_t off) {
        glEnableVertexAttribArray(loc);
        glVertexAttribPointer(loc, n, GL_FLOAT, GL_FALSE, stride, (void*)off);
        glVertexAttribDivisor(loc, 1);   // avanza una volta per istanza
    };
    attr(1, 2, offsetof(GlyphInstance, pos));
    attr(2, 2, offsetof(GlyphInstance, size));
    attr(3, 4, offsetof(GlyphInstance, uv));
    attr(4, 4, offsetof(GlyphInstance, color));

    glBindVertexArray(0);

    instCapacity_ = 1024;
    instances_.reserve(instCapacity_);
    glBindBuffer(GL_ARRAY_BUFFER, instVbo_);
    glBufferData(GL_ARRAY_BUFFER, instCapacity_ * sizeof(GlyphInstance), nullptr, GL_STREAM_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    return true;
}

void TextRender::shutdown() {
    if (instVbo_) glDeleteBuffers(1, &instVbo_);
    if (quadVbo_) glDeleteBuffers(1, &quadVbo_);
    if (vao_)     glDeleteVertexArrays(1, &vao_);
    shader_.destroy();
    instVbo_ = quadVbo_ = vao_ = 0;
}

// ---------------------------------------------------------------- helpers

const Glyph& TextRender::glyphFor(unsigned char c) const {
    const Glyph& g = font_->glyphs[c];
    return g.valid ? g : font_->glyphs[(unsigned char)'?'];
}

// ---------------------------------------------------------------- accodamento

void TextRender::draw(std::string_view s, float x, float y, float scale, Color c) {
    if (!font_) return;
    const float lh = font_->lineHeight * scale;
    float cx = x, cy = y;

    for (unsigned char ch : s) {
        if (ch == '\n') { cx = x; cy += lh; continue; }
        const Glyph& g = glyphFor(ch);
        if (g.w > 0 && g.h > 0 && ch != ' ') {
            GlyphInstance gi;
            gi.pos[0]  = std::round(cx + g.xoff * scale);   // round: evita bordi sfocati
            gi.pos[1]  = std::round(cy + g.yoff * scale);
            gi.size[0] = g.w * scale;
            gi.size[1] = g.h * scale;
            gi.uv[0] = g.u0; gi.uv[1] = g.v0; gi.uv[2] = g.u1; gi.uv[3] = g.v1;
            gi.color[0] = c.r; gi.color[1] = c.g; gi.color[2] = c.b; gi.color[3] = c.a;
            instances_.push_back(gi);
        }
        cx += g.advance * scale;
    }
}

Vec2 TextRender::measure(std::string_view s, float scale) const {
    if (!font_) return {};
    float w = 0, maxW = 0;
    int lines = 1;
    for (unsigned char ch : s) {
        if (ch == '\n') { maxW = std::max(maxW, w); w = 0; ++lines; continue; }
        w += glyphFor(ch).advance * scale;
    }
    maxW = std::max(maxW, w);
    return { maxW, lines * font_->lineHeight * scale };
}

// ---------------------------------------------------------------- layout / wrap

TextLayout TextRender::layout(std::string_view s, float maxWidth, float scale) const {
    TextLayout L;
    L.scale = scale;
    if (!font_) return L;

    constexpr size_t npos = (size_t)-1;
    size_t start = 0, i = 0, lastSpace = npos;
    float  w = 0, wAtSpace = 0;

    auto push = [&](size_t b, size_t e, float width) {
        L.lines.push_back({ b, e, width });
    };

    while (i < s.size()) {
        unsigned char c = s[i];

        if (c == '\n') {
            push(start, i, w);
            start = i = i + 1;
            w = 0; lastSpace = npos;
            continue;
        }

        float adv = glyphFor(c).advance * scale;

        // Overflow (gli spazi non provocano a capo: restano a fine riga, invisibili)
        if (maxWidth > 0 && c != ' ' && w + adv > maxWidth && i > start) {
            if (lastSpace != npos) {          // spezza all'ultimo spazio
                push(start, lastSpace, wAtSpace);
                start = i = lastSpace + 1;    // salta lo spazio e riparti da li'
            } else {                          // parola piu' lunga della riga: spezza il carattere
                push(start, i, w);
                start = i;
            }
            w = 0; lastSpace = npos;
            continue;
        }

        if (c == ' ') { lastSpace = i; wAtSpace = w; }
        w += adv;
        ++i;
    }
    push(start, s.size(), w);                 // ultima riga

    L.height = L.lines.size() * font_->lineHeight * scale;
    return L;
}

void TextRender::drawLayout(std::string_view s, const TextLayout& L,
                            const Rect& area, float scrollY, Color c) {
    if (!font_) return;
    const float lh = font_->lineHeight * L.scale;

    for (size_t k = 0; k < L.lines.size(); ++k) {
        float y = area.y + k * lh - scrollY;
        if (y + lh < area.y)      continue;   // sopra l'area
        if (y > area.y + area.h)  break;      // sotto l'area: le successive sono ancora piu' giu'
        const auto& ln = L.lines[k];
        draw(s.substr(ln.begin, ln.end - ln.begin), area.x, y, L.scale, c);
    }
}

// ---------------------------------------------------------------- flush

void TextRender::flush(const Rect* clip) {
    if (instances_.empty() || !font_) { instances_.clear(); return; }

    // Capacita' crescente (raddoppio)
    while (instCapacity_ < instances_.size()) instCapacity_ *= 2;

    shader_.use();
    glUniform2f(uScreenLoc_, (float)screenW_, (float)screenH_);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, font_->texture);
    glUniform1i(uAtlasLoc_, 0);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glDisable(GL_DEPTH_TEST);

    if (clip) {
        // GL: origine dello scissor in basso a sinistra -> inverti la y
        GLint   sx = (GLint)std::floor(clip->x);
        GLint   sy = (GLint)std::floor(screenH_ - (clip->y + clip->h));
        GLsizei sw = (GLsizei)std::ceil(clip->w);
        GLsizei sh = (GLsizei)std::ceil(clip->h);
        glEnable(GL_SCISSOR_TEST);
        glScissor(sx, sy, sw, sh);
    }

    glBindVertexArray(vao_);
    glBindBuffer(GL_ARRAY_BUFFER, instVbo_);

    // Orphaning: nuovo storage, il vecchio resta vivo finche' la GPU lo legge
    glBufferData(GL_ARRAY_BUFFER, instCapacity_ * sizeof(GlyphInstance), nullptr, GL_STREAM_DRAW);
    glBufferSubData(GL_ARRAY_BUFFER, 0, instances_.size() * sizeof(GlyphInstance), instances_.data());

    glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, 4, (GLsizei)instances_.size());

    glBindVertexArray(0);
    if (clip) glDisable(GL_SCISSOR_TEST);

    instances_.clear();
}
