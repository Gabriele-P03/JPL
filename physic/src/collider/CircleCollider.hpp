/**
 * CircleCollider is the collider for a 2D circle AABB
 */

#ifndef CIRCLE_COLLIDER_PHYSICA_JPL
#define CIRCLE_COLLIDER_PHYSICA_JPL

#include "ICollider.hpp"

namespace jpl{
    namespace _physics{

        class CircleCollider final : public ICollider {
                        
            private:
                float m_radius;

            public:
                CircleCollider(float radius) : m_radius(radius) {}
                ColliderType GetType() const override { return ColliderType::Circle; }
                float GetRadius() const { return m_radius; }

                AABB ComputeLocalAABB() const override {
                    return AABB{ glm::vec3(-m_radius, -m_radius, 0.0f), glm::vec3(m_radius, m_radius, 0.0f) };
                }

                AABB ComputeWorldAABB(const glm::vec3& position, const glm::mat3& /*rotationMatrix*/) const override {
                    // Per il cerchio la rotazione non influenza l'AABB
                    AABB local = ComputeLocalAABB();
                    return AABB{ local.min + position, local.max + position };
                }
        };
    }
}

#endif