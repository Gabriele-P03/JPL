/**
 * Axis-Aligned Bounding Box
 * 
 */

#ifndef AABB_PHYSICS_JPL
#define AABB_PHYSICS_JPL

#include <glm/glm.hpp>
#include <algorithm>

namespace jpl{
    namespace _physics{

        struct AABB {
            glm::vec3 min;
            glm::vec3 max;

            AABB() : min(glm::vec3(0.0f)), max(glm::vec3(0.0f)) {}
            AABB(const glm::vec3& min, const glm::vec3& max) : min(min), max(max) {}

            bool Intersects(const AABB& other) const {
                return (min.x <= other.max.x && max.x >= other.min.x) &&
                       (min.y <= other.max.y && max.y >= other.min.y) &&
                       (min.z <= other.max.z && max.z >= other.min.z);
            }

            /**
             * Extends AABB including a new point
             */
            void Encapsulate(const glm::vec3& point) {
                min = glm::min(min, point);
                max = glm::max(max, point);
            }
        };

    }
}

#endif