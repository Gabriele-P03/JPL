#ifndef SPHERE_COLLIDER_PHYSICS_JPL
#define SPHERE_COLLIDER_PHYSICS_JPL

#include "ICollider.hpp"

namespace jpl{
    namespace _physics{
        class SphereCollider final : public ICollider {
   
            private:
                float m_radius;

            public:
                SphereCollider(float radius) : m_radius(radius) {}
                ColliderType GetType() const override { return ColliderType::Sphere; }
                float GetRadius() const { return m_radius; }

                AABB ComputeLocalAABB() const override {
                    return AABB{ glm::vec3(-m_radius), glm::vec3(m_radius) };
                }

                AABB ComputeWorldAABB(const glm::vec3& position, const glm::mat3& rotationMatrix) const override {
                    AABB local = ComputeLocalAABB();
                    return AABB{ local.min + position, local.max + position };
                }

        };
    }
}

#endif