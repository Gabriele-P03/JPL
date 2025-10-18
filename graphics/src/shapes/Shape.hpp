/**
 * Base class for all base shapes available.
 * A shape is a set of vertices which may be passed to any OpenGL's vertx buffers 
 */

#ifndef SHAPE_GRAPHICS_JPL
#define SHAPE_GRAPHICS_JPL

namespace jpl{
    namespace _graphics{
        namespace _shapes{

            class Shape{

                public:

                    //coords for a point: x,y,z
                    static constexpr unsigned int COORDS_PER_POINT = 3; 

                    virtual ~Shape() = default; //For polymorphism
            };
        }
    }
}


#endif