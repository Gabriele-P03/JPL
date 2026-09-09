#ifndef BOX_COLLIDER_PHYSICS_JPL
#define BOX_COLLIDER_PHYSICS_JPL

#include "ICollider.hpp"

namespace jpl{
    namespace _physics{
        class BoxCollider final : public ICollider {
   
            private:
                glm::vec3 m_halfExtents;

            public:
                BoxCollider(const glm::vec3& halfExtents) : m_halfExtents(halfExtents) {}
                ColliderType GetType() const override { return ColliderType::Box3D; }
                const glm::vec3& GetHalfExtents() const { return m_halfExtents; }

                AABB ComputeLocalAABB() const override {
                    return AABB{ -m_halfExtents, m_halfExtents };
                }

                AABB ComputeWorldAABB(const glm::vec3& position, const glm::mat3& rotationMatrix) const override {
                    AABB box;
                    // Generiamo gli 8 vertici del parallelepipedo 3D locale
                    glm::vec3 corners[8];
                    int idx = 0;
                    for (float x : {-m_halfExtents.x, m_halfExtents.x})
                        for (float y : {-m_halfExtents.y, m_halfExtents.y})
                            for (float z : {-m_halfExtents.z, m_halfExtents.z})
                                corners[idx++] = glm::vec3(x, y, z);

                    box.min = position + (rotationMatrix * corners[0]);
                    box.max = box.min;
                    for (int i = 1; i < 8; ++i) {
                        box.Encapsulate(position + (rotationMatrix * corners[i]));
                    }
                    return box;
                };
        };
    }
}

#endif