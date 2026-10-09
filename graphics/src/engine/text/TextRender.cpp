#include "TextRender.hpp"
#include "../../Metrics.hpp"
#include <algorithm>
#include <cmath>
#include <cstddef>

void jpl::_graphics::_engine::_text::TextRender::init(_graphics::_shaders::ProgramShaders* psText) {
    this->ps = psText;
    this->ps->use();
    uScreenLoc_ = glGetUniformLocation(psText->getProgramIndex(), "uScreen");
    uAtlasLoc_  = glGetUniformLocation(psText->getProgramIndex(),"uAtlas");

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
}

void jpl::_graphics::_engine::_text::TextRender::shutdown() {
    if (instVbo_)
        glDeleteBuffers(1, &instVbo_);
    if (quadVbo_)
        glDeleteBuffers(1, &quadVbo_);
    if (vao_)
        glDeleteVertexArrays(1, &vao_);
    instVbo_ = quadVbo_ = vao_ = 0;
}


const jpl::_graphics::_engine::_text::Glyph& jpl::_graphics::_engine::_text::TextRender::glyphFor(const uint32_t c) const {
    const Glyph& g = font_->glyphs.at(c);
    return g.valid ? g : font_->glyphs.at('?');
}


void jpl::_graphics::_engine::_text::TextRender::draw(std::string_view s, float x, float y, float scale, glm::vec4 c) {
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
            gi.color[0] = c.x; gi.color[1] = c.y; gi.color[2] = c.z; gi.color[3] = c.w;
            instances_.push_back(gi);
        }
        cx += g.advance * scale;
    }
}

glm::vec2 jpl::_graphics::_engine::_text::TextRender::measure(std::string_view s, float scale) const {
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


jpl::_graphics::_engine::_text::TextLayout jpl::_graphics::_engine::_text::TextRender::layout(std::string_view s, float maxWidth, float scale) const {
    TextLayout L;
    L.scale = scale;
    if (!font_)
        return L;

    constexpr size_t npos = (size_t)-1;
    size_t start = 0, i = 0, lastSpace = npos;
    float  w = 0, wAtSpace = 0;

    auto push = [&](size_t b, size_t e, float width) {L.lines.push_back({ b, e, width });};

    while (i < s.size()) {
        unsigned char c = s[i];
        if (c == '\n') {
            push(start, i, w);
            start = i = i + 1;
            w = 0; lastSpace = npos;
            continue;
        }
        float adv = glyphFor(c).advance * scale;
        // Overflow (spaces does not cause new-line: kept at the end of the line as transparent)
        if (maxWidth > 0 && c != ' ' && w + adv > maxWidth && i > start) {
            if (lastSpace != npos) {
                push(start, lastSpace, wAtSpace);
                start = i = lastSpace + 1;
            } else {
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
    push(start, s.size(), w);

    L.height = L.lines.size() * font_->lineHeight * scale;
    return L;
}

void jpl::_graphics::_engine::_text::TextRender::drawLayout(std::string_view s, const TextLayout& L,
                                        const glm::vec4& area, float scrollY, glm::vec4 c) {
    if (!font_) return;
    const float lh = font_->lineHeight * L.scale;

    for (size_t k = 0; k < L.lines.size(); ++k) {
        float y = area.y + k * lh - scrollY;
        if (y + lh < area.y)      continue;
        if (y > area.y + area.w)  break;
        const auto& ln = L.lines[k];
        draw(s.substr(ln.begin, ln.end - ln.begin), area.x, y, L.scale, c);
    }
}


void jpl::_graphics::_engine::_text::TextRender::flush(const glm::vec4* clip) {
    if (instances_.empty() || !font_) { instances_.clear(); return; }

    while (instCapacity_ < instances_.size()) instCapacity_ *= 2;

    this->ps->use();
    glUniform2f(uScreenLoc_, (float)jpl::_graphics::_metrics::width, (float)jpl::_graphics::_metrics::height);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, font_->texture);
    glUniform1i(uAtlasLoc_, 0);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glDisable(GL_DEPTH_TEST);

    if (clip) {
        // GL: origine dello scissor in basso a sinistra -> inverti la y
        GLint   sx = (GLint)std::floor(clip->x);
        GLint   sy = (GLint)std::floor(clip->y);
        GLsizei sw = (GLsizei)std::ceil(clip->z);
        GLsizei sh = (GLsizei)std::ceil(clip->w);
        glEnable(GL_SCISSOR_TEST);
        glScissor(sx, sy, sw, sh);
    }

    glBindVertexArray(vao_);
    glBindBuffer(GL_ARRAY_BUFFER, instVbo_);

    // Orphaning: nuovo storage, il vecchio resta vivo finche' la GPU lo legge
    glBufferData(GL_ARRAY_BUFFER, instCapacity_ * sizeof(GlyphInstance), nullptr, GL_STREAM_DRAW);
    glBufferSubData(GL_ARRAY_BUFFER, 0, instances_.size() * sizeof(GlyphInstance), instances_.data());

    glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, 4, static_cast<GLsizei>(instances_.size()));

    glBindVertexArray(0);
    if (clip) glDisable(GL_SCISSOR_TEST);

    instances_.clear();
}

float jpl::_graphics::_engine::_text::TextRender::capCenterOffset(float scale) const {
    if (!font_)
        return 0.0f;
    const Glyph& H = font_->glyphs.at('H');
    if (!H.valid)
        return font_->lineHeight * 0.5f * scale;   // fallback: centro della riga
    return (H.yoff + H.h * 0.5f) * scale;
}

void jpl::_graphics::_engine::_text::TextRender::fillRect(const glm::vec4& r, const glm::vec4& c) {
    if (!font_ || r.z <= 0 || r.w <= 0) return;
    GlyphInstance gi;
    gi.pos[0]  = r.x;  gi.pos[1]  = r.y;
    gi.size[0] = r.z;  gi.size[1] = r.w;
    gi.uv[0] = gi.uv[2] = font_->whiteU;
    gi.uv[1] = gi.uv[3] = font_->whiteV;
    gi.color[0] = c.x; gi.color[1] = c.y; gi.color[2] = c.z; gi.color[3] = c.w;
    instances_.push_back(gi);
}

//This function is declared inside this source file only
uint32_t decodeUtf8(std::string_view s, size_t& i) {
    unsigned char c = (unsigned char)s[i];
    int len; uint32_t cp;
    if      (c < 0x80)           { ++i; return c; }
    if ((c & 0xE0) == 0xC0) { len = 2; cp = c & 0x1F; }
    else if ((c & 0xF0) == 0xE0) { len = 3; cp = c & 0x0F; }
    else if ((c & 0xF8) == 0xF0) { len = 4; cp = c & 0x07; }
    else                         { ++i; return 0xFFFD; }

    if (i + len > s.size()) { ++i; return 0xFFFD; }
    for (int k = 1; k < len; ++k) {
        unsigned char cc = (unsigned char)s[i + k];
        if ((cc & 0xC0) != 0x80) { ++i; return 0xFFFD; }
        cp = (cp << 6) | (cc & 0x3F);
    }
    i += len;
    return cp;
}

size_t jpl::_graphics::_engine::_text::TextRender::indexAtX(std::string_view s, float x, float scale) const {
    if (!font_ || x <= 0) return 0;
    float w = 0;
    size_t i = 0;
    while (i < s.size()) {
        size_t next = i;
        uint32_t cp = decodeUtf8(s, next);
        float adv = glyphFor(cp).advance * scale;
        if (x < w + adv * 0.5f) return i;
        w += adv;
        i = next;
    }
    return s.size();
}
