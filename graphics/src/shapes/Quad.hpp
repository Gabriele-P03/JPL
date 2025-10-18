#ifndef QUAD_GRAPHICS_JPL
#define QUAD_GRAPHICS_JPL

#include "Shape.hpp"

namespace jpl{
    namespace _graphics{
        namespace _shapes{

            class Quad : public Shape{

                public:

                    static constexpr unsigned short SIZE_TEXTURE = 2;
                    static constexpr unsigned short VALUES_PER_POINTS = Shape::COORDS_PER_POINT + SIZE_TEXTURE;
                    static constexpr unsigned int POINTS = 4;
                    static constexpr unsigned int SIZE = POINTS*VALUES_PER_POINTS; 
                    static constexpr float ORTHO_VERTICES[SIZE] = {
                        0.5f, 0.5f, 0.0f,       1.0f, 1.0f,     //Upper-right
                        -0.5f, 0.5f, 0.0f,      0.0f, 1.0f,     //Upper-Left
                        -0.5f, -0.5f, 0.0f,     0.0f, 0.0f,     //Bottom-Left
                        0.5f, -0.5f, 0.0f,      1.0f, 0.0f      //Bottom-Right
                    };

                    static constexpr unsigned int SIZE_INDICES = 6;
                    static constexpr unsigned int ORTHO_INDICES[SIZE_INDICES] = {
                        0,1,2,
                        0,3,2
                    };
            };
        }
    }
}

#endif