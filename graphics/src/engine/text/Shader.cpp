#include "Shader.h"
#include <fstream>
#include <sstream>
#include <cstdio>

namespace {

bool readFile(const std::string& path, std::string& out) {
    std::ifstream f(path, std::ios::binary);
    if (!f) {
        std::fprintf(stderr, "Shader: impossibile aprire '%s'\n", path.c_str());
        return false;
    }
    std::stringstream ss;
    ss << f.rdbuf();
    out = ss.str();
    return true;
}

GLuint compile(GLenum type, const std::string& src, const std::string& path) {
    GLuint sh = glCreateShader(type);
    const char* p = src.c_str();
    glShaderSource(sh, 1, &p, nullptr);
    glCompileShader(sh);

    GLint ok = 0;
    glGetShaderiv(sh, GL_COMPILE_STATUS, &ok);
    if (!ok) {
        char log[2048];
        glGetShaderInfoLog(sh, sizeof log, nullptr, log);
        std::fprintf(stderr, "Shader: errore di compilazione in '%s':\n%s\n", path.c_str(), log);
        glDeleteShader(sh);
        return 0;
    }
    return sh;
}

} // namespace

bool Shader::loadFromFiles(const std::string& vertPath, const std::string& fragPath) {
    std::string vsSrc, fsSrc;
    if (!readFile(vertPath, vsSrc) || !readFile(fragPath, fsSrc)) return false;

    GLuint vs = compile(GL_VERTEX_SHADER,   vsSrc, vertPath);
    GLuint fs = compile(GL_FRAGMENT_SHADER, fsSrc, fragPath);
    if (!vs || !fs) {
        if (vs) glDeleteShader(vs);
        if (fs) glDeleteShader(fs);
        return false;
    }

    GLuint prog = glCreateProgram();
    glAttachShader(prog, vs);
    glAttachShader(prog, fs);
    glLinkProgram(prog);
    glDeleteShader(vs);
    glDeleteShader(fs);

    GLint ok = 0;
    glGetProgramiv(prog, GL_LINK_STATUS, &ok);
    if (!ok) {
        char log[2048];
        glGetProgramInfoLog(prog, sizeof log, nullptr, log);
        std::fprintf(stderr, "Shader: errore di link (%s + %s):\n%s\n",
                     vertPath.c_str(), fragPath.c_str(), log);
        glDeleteProgram(prog);
        return false;
    }

    destroy();       // in caso di reload a caldo
    id_ = prog;
    return true;
}

void Shader::destroy() {
    if (id_) glDeleteProgram(id_);
    id_ = 0;
}
