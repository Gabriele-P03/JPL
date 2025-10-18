#ifndef CUBE_GRAPHICS_JPL
#define CUBE_GRAPHICS_JPL

#include "Shape.hpp"

namespace jpl{
    namespace _graphics{
        namespace _shapes{

            class Cube : public Shape{

                public:

                    static constexpr unsigned short SIZE_TEXTURE = 2;
                    static constexpr unsigned short VALUES_PER_POINTS = Shape::COORDS_PER_POINT + SIZE_TEXTURE;
                    static constexpr unsigned int POINTS = 8;
                    static constexpr unsigned int SIZE = POINTS*VALUES_PER_POINTS; 
                    static constexpr float ORTHO_VERTICES[SIZE] = {
                        0.5f, 0.5f, -0.5f,       1.0f, 1.0f,     //Upper-right-behind
                        -0.5f, 0.5f, -0.5f,      0.0f, 1.0f,     //Upper-Left-behind
                        -0.5f, -0.5f, -0.5f,     0.0f, 0.0f,     //Bottom-Left-behind
                        0.5f, -0.5f, -0.5f,      1.0f, 0.0f,     //Bottom-Right-behind

                        0.5f, 0.5f, 0.5f,       1.0f, 1.0f,     //Upper-right-front
                        -0.5f, 0.5f, 0.5f,      0.0f, 1.0f,     //Upper-Left-front
                        -0.5f, -0.5f, 0.5f,     0.0f, 0.0f,     //Bottom-Left-front
                        0.5f, -0.5f, 0.5f,      1.0f, 0.0f      //Bottom-Right-front
                    };

                    static constexpr unsigned int SIZE_INDICES = 36;
                    static constexpr unsigned int ORTHO_INDICES[SIZE_INDICES] = {
                        0,1,2,0,3,2,    //Back
                        4,5,6,4,7,6,    //front
                        0,1,5,0,4,5,    //up
                        3,2,6,3,7,6,    //bottom
                        5,1,2,5,6,2,    //left
                        4,0,3,4,7,3     //right
                    };
            };
        }
    }
}

#endif