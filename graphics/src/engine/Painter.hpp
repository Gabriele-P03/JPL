/**
 * Painter is a wrapper class which can be used to paint any 2D texture basing on a certain mesh.
 * 
 * You can push a new mesh via pushData(const Mesh*, GLenum); since it does not check what VAO, VBO and EBO have been lastly binded, it is up to you
 * to bind them before call this function.
 * 
 * Once you have pushed a new mesh, you are able to call render(Texture*, float, float, float, float) in order to render the texture
 */

#ifndef PAINTER_GRAPHICS_JPL
#define PAINTER_GRAPHICS_JPL

#include "../mesh/Mesh.hpp"
#include "../texture/Texture.hpp"
#include "../Metrics.hpp"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace jpl{
    namespace _graphics{
        namespace _engine{

            class Painter{

                protected:

                    //These fields below are used only to ensure new matrix pushing
                    _texture::Texture* lastTexture;
                    float x,y,w,h;

                    /**
                     * This fiels is set by pushData whereas mesh does contain indices.
                     * It is read by render function in order to call glDrawElements rather than glDrawArrays
                     */
                    unsigned int sizeIndices;

                    /**
                     * This fiels is set by pushData whereas mesh does not contain indices and represents how many float values per point.
                     * It is read by render function in order to call glDrawArrays rather than glDrawElements
                     */
                    unsigned int valuesPerPoint;

                public:
                    Painter();

                    virtual void pushData(const _mesh::Mesh* mesh, GLenum mode);

                    void setSizeIndices(unsigned int sizeIndices) noexcept{
                        this->sizeIndices = sizeIndices;
                    }
                    unsigned int getSizeIndices() const noexcept{
                        return this->sizeIndices;
                    }
                    void setValuesPerPoint(unsigned int valuesPerPoint) noexcept{
                        this->valuesPerPoint = valuesPerPoint;
                    }
                    unsigned int getValuesPerPoint() const noexcept{
                        return this->valuesPerPoint;
                    }

                    virtual void render(_texture::Texture* texture, float x, float y, float w, float h) const;
            };


        }
    }
}

#endif