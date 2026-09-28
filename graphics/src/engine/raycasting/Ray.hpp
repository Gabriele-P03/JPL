
#ifndef RAY_GRAPHICS_JPL
#define RAY_GRAPHICS_JPL
#include <glm/glm.hpp>

namespace jpl::_graphics::_engine::_raycasting {

    struct Ray {
        glm::vec3 origin;
        glm::vec3 direction;
    };
}

#endif