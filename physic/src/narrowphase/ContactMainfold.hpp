#ifndef CONTACT_MANIFOLD_PHYSICS_JPL
#define CONTACT_MANIFOLD_PHYSICS_JPL

#include <glm/glm.hpp>
#include "../collider/ICollider.hpp"
#include "../rigidbody/IRigidBody.hpp"

namespace jpl{
    namespace _physics{
        
        struct ContactManifold {
            IRigidBody* bodyA = nullptr;
            IRigidBody* bodyB = nullptr;
            
            glm::vec3 contactPoint{0.0f};
            glm::vec3 normal{0.0f};  // Collision Direction (from A to B)
            float penetration = 0.0f;
            bool hasCollision = false;
        };
    }
}

#endif