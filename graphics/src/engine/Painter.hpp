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

                    float x, y, z;
                    bool posUpdated;

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

                    void setX(float x) noexcept{
                        this->x = x;
                        this->posUpdated = true;
                    }
                    void setY(float y) noexcept{
                        this->y = y;
                        this->posUpdated = true;
                    }
                    void setZ(float z) noexcept{
                        this->z = z;
                        this->posUpdated = true;
                    }
                    void setPos(float x, float y, float z) noexcept{
                        this->x = x;
                        this->y = y;
                        this->z = z;
                        this->posUpdated = true;
                    }
                    float getX() const noexcept {return this->x;}
                    float getY() const noexcept {return this->y;}
                    float getZ() const noexcept {return this->z;}

                    bool isPosUpdated() const noexcept{
                        return this->posUpdated;
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