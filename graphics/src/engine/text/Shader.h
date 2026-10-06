#pragma once
#include <glad/glad.h>   // cambia con il tuo loader
#include <string>

// Programma GLSL caricato da file. Riutilizzabile anche per gli sprite 2D.
class Shader {
public:
    bool loadFromFiles(const std::string& vertPath, const std::string& fragPath);
    void destroy();

    void  use() const { glUseProgram(id_); }
    GLint uniform(const char* name) const { return glGetUniformLocation(id_, name); }
    GLuint id() const { return id_; }

private:
    GLuint id_ = 0;
};
