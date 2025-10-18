/**
 * A mesh is a set of vertices, which compose a shape
 */

#ifndef MESH_GRAPHICS_JPL
#define MESH_GRAPHICS_JPL

#include <jpl/exception/runtime/IllegalArgumentException.hpp>
#include "../shapes/Triangle.hpp"
#include "../shapes/Quad.hpp"
#include "../shapes/Cube.hpp"

namespace jpl{
    namespace _graphics{
        namespace _mesh{

            class Mesh{

                protected:

                    const float* vertices;
                    const unsigned int sizeVertices;
                    const unsigned int offsetTexture;

                    const unsigned int* indices;
                    const unsigned int sizeIndices;

                    //It should be passed as stride to glVertexAttribPointer
                    unsigned int coordsPerPoint; 

                    const bool normalized;

                public:

                    Mesh(const float* vertices, unsigned int sizeVertices, unsigned int offsetTexture, const unsigned int* indices, unsigned int sizeIndices, unsigned int coordsPerPoint, bool normalized);

                    ~Mesh();

                    bool isNormalized() const noexcept{
                        return this->normalized;
                    }

                    unsigned int getSizeVertices() const noexcept{
                        return this->sizeVertices;
                    }

                    const float* getVertices() const noexcept{
                        return this->vertices;
                    }

                    unsigned int getOffsetTexture() const noexcept{
                        return this->offsetTexture;
                    }

                    unsigned int getSizeIndices() const noexcept{
                        return this->sizeIndices;
                    }

                    const unsigned int* getIndices() const noexcept{
                        return this->indices;
                    }

                    unsigned int getCoordsPerPoint() const noexcept{
                        return this->coordsPerPoint;
                    }
            };

            extern const Mesh* const TRIANGLE;
            extern const Mesh* const QUAD;
            extern const Mesh* const CUBE;
        }
    }
}

#endif