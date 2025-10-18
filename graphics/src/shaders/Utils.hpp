/**
 * Utility functions about shaders program
 */

#ifndef SHADERS_UTILS_GRAPHICS_JPL
#define SHADERS_UTILS_GRAPHICS_JPL

//#include <GL/gl.h>

namespace jpl{
    namespace _graphics{
        namespace _shader{
            namespace _utils{

                /**
                 * There is a maximum number of vertex attributes we're allowed to declare limited by the hardware.
                 */
                inline int getMaxSupportedVertexAttr(){
                    int max = 0;
                    //glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &max);
                    return max;
                }
            }
            
        }
    }
}

#endif