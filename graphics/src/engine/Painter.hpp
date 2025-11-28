#ifndef VERTEX_GRAPHICS_JPL
#define VERTEX_GRAPHICS_JPL

#include "../mesh/Mesh.hpp"
#include <GL/glew.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace jpl{
    namespace _graphics{
        namespace _engine{

            class Painter{

                protected:
                    unsigned int VBO;
                    unsigned int VAO;
                    unsigned int EBO;

                    unsigned int x, y, z;
                    bool posUpdated;
                    unsigned int w,h;
                    bool sizeUpdated;

                    /**
                     * Index of the uniform model matrix called modelM.
                     * If none modelM matrix has been found, it is set as -1
                     */
                    int locModelMatrix;

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

                    void setX(unsigned int x) noexcept{
                        this->x = x;
                        this->posUpdated = true;
                    }
                    void setY(unsigned int y) noexcept{
                        this->y = y;
                        this->posUpdated = true;
                    }
                    void setZ(unsigned int z) noexcept{
                        this->z = z;
                        this->posUpdated = true;
                    }
                    void setPos(unsigned int x, unsigned int y, unsigned int z) noexcept{
                        this->x = x;
                        this->y = y;
                        this->z = z;
                        this->posUpdated = true;
                    }
                    unsigned int getX() const noexcept {return this->x;}
                    unsigned int getY() const noexcept {return this->y;}
                    unsigned int getZ() const noexcept {return this->z;}

                    bool isPosUpdated() const noexcept{
                        return this->posUpdated;
                    }
                    bool isSizeUpdated() const noexcept{
                        return this->sizeUpdated;
                    }

                    void setSizeUpdated(bool sizeUpdated) noexcept{
                        this->sizeUpdated = sizeUpdated;
                    }
                    void setPosUpdated(bool posUpdated) noexcept{
                        this->posUpdated = posUpdated;
                    }
                    int getModelMatrixLocation() const noexcept{
                        return this->locModelMatrix;
                    } 
            };

            extern void drawMesh(Painter* vertex, const _mesh::Mesh* mesh);

        }
    }
}

#endif