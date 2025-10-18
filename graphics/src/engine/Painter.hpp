#ifndef VERTEX_GRAPHICS_JPL
#define VERTEX_GRAPHICS_JPL

#include "../mesh/Mesh.hpp"
#include <GL/glew.h>

namespace jpl{
    namespace _graphics{
        namespace _engine{

            class Painter{

                protected:
                    unsigned int VBO;
                    unsigned int VAO;
                    unsigned int EBO;

                public:
                    Painter();

                    virtual void bindBuffer() const noexcept;
                    virtual void pushData(const float* data, unsigned int sizeData, GLenum mode) const noexcept;
                    virtual void pushData(const _mesh::Mesh* mesh, GLenum mode) const noexcept;

                    unsigned int getVBO() const noexcept{
                        return this->VBO;
                    }
                    unsigned int getVAO() const noexcept {
                        return this->VAO;
                    }
                    unsigned int getEBO() const noexcept{
                        return this->EBO;
                    }
            };

            extern void drawMesh(Painter* vertex, const _mesh::Mesh* mesh);

        }
    }
}

#endif