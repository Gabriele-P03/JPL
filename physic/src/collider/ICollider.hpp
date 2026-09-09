#ifndef ICOLLIDER_PHYSICS_JPL
#define ICOLLIDER_PHYSICS_JPL

#include <glm/glm.hpp>
#include <concepts>
#include "aabb.hpp"

namespace jpl{
    namespace _physics {

        enum class ColliderType { Circle, Box2D, Sphere, Box3D };

        class ICollider {
        public:
            virtual ~ICollider() = default;

            virtual ColliderType GetType() const = 0;
            
            /**
             * Compute AABB in the local space of the object
             */
            virtual AABB ComputeLocalAABB() const = 0;

            /**
             * Compute AABB in the world space
             */
            virtual AABB ComputeWorldAABB(const glm::vec3& position, const glm::mat3& rotationMatrix) const = 0;
        };
    }
}

#endif