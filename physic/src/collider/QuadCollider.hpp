/**
 * QuadCollider is the collider for 2D-Box (Quad) AABB
 */

#ifndef QUAD_COLLIDER_PHYSICS_JPL
#define QUAD_COLLIDER_PHYSICS_JPL

#include "ICollider.hpp"

namespace jpl{
    namespace _physics{
        class BoxCollider2D final : public ICollider {
   
            private:
                glm::vec2 m_halfExtents;

            public:
                BoxCollider2D(const glm::vec2& halfExtents) : m_halfExtents(halfExtents) {}
                ColliderType GetType() const override { return ColliderType::Box2D; }
                const glm::vec2& GetHalfExtents() const { return m_halfExtents; }

                AABB ComputeLocalAABB() const override {
                    return AABB{ glm::vec3(-m_halfExtents, 0.0f), glm::vec3(m_halfExtents, 0.0f) };
                }

                AABB ComputeWorldAABB(const glm::vec3& position, const glm::mat3& rotationMatrix) const override {
                    AABB box;
                    
                    glm::vec3 corners[4] = {
                        glm::vec3(-m_halfExtents.x, -m_halfExtents.y, 0.0f),
                        glm::vec3( m_halfExtents.x, -m_halfExtents.y, 0.0f),
                        glm::vec3( m_halfExtents.x,  m_halfExtents.y, 0.0f),
                        glm::vec3(-m_halfExtents.x,  m_halfExtents.y, 0.0f)
                    };

                    box.min = position + (rotationMatrix * corners[0]);
                    box.max = box.min;
                    for (int i = 1; i < 4; ++i) {
                        box.Encapsulate(position + (rotationMatrix * corners[i]));
                    }
                    return box;
                }

        };
    }
}

#endif