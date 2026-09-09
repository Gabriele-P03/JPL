#ifndef NARROWPHASE_PHYSICS_JPL
#define NARROWPHASE_PHYSICS_JPL

#include "ContactMainfold.hpp"
#include "../collider/BoxCollider.hpp"
#include "Projection.hpp"
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/norm.hpp>

namespace jpl{
    namespace _physics{

        class Narrowphase {
        
            private:
                using CollisionFunc = ContactManifold(*)(IRigidBody*, IRigidBody*, const ICollider*, const ICollider*);
                
                static ContactManifold SphereVsSphere(IRigidBody* bodyA, IRigidBody* bodyB, const ICollider* colA, const ICollider* colB) {
                    ContactManifold m;
                    auto* sA = static_cast<const SphereCollider*>(colA);
                    auto* sB = static_cast<const SphereCollider*>(colB);

                    glm::vec3 posA = bodyA->GetPosition3D();
                    glm::vec3 posB = bodyB->GetPosition3D();

                    glm::vec3 delta = posB - posA;
                    float distance = glm::length(delta);
                    float radiusSum = sA->GetRadius() + sB->GetRadius();

                    if (distance < radiusSum) {
                        m.hasCollision = true;
                        m.penetration = radiusSum - distance;
                        m.normal = (distance > 0.0f) ? delta / distance : glm::vec3(0.0f, 1.0f, 0.0f);
                        m.contactPoint = posA + (m.normal * sA->GetRadius());
                    }
                    return m;
                }

                static ContactManifold CircleVsCircle(IRigidBody* bodyA, IRigidBody* bodyB, const ICollider* colA, const ICollider* colB) {
                    ContactManifold m;
                    auto* cA = static_cast<const CircleCollider*>(colA);
                    auto* cB = static_cast<const CircleCollider*>(colB);

                    glm::vec3 posA = bodyA->GetPosition3D();
                    glm::vec3 posB = bodyB->GetPosition3D();

                    // Lavoriamo in 3D ma sappiamo che Z è 0.0f
                    glm::vec3 delta = posB - posA;
                    float distance = glm::length(delta);
                    float radiusSum = cA->GetRadius() + cB->GetRadius();

                    if (distance < radiusSum) {
                        m.hasCollision = true;
                        m.penetration = radiusSum - distance;
                        m.normal = (distance > 0.0f) ? delta / distance : glm::vec3(1.0f, 0.0f, 0.0f);
                        m.contactPoint = posA + (m.normal * cA->GetRadius());
                    }
                    
                    return m;
                }

                static ContactManifold SphereVsCircle(IRigidBody* bodyA, IRigidBody* bodyB, const ICollider* colA, const ICollider* colB) {
                    ContactManifold m;
                    auto* sphere = static_cast<const SphereCollider*>(colA);
                    auto* circle = static_cast<const CircleCollider*>(colB);

                    glm::vec3 posSphere = bodyA->GetPosition3D();
                    glm::vec3 posCircle = bodyB->GetPosition3D();

                    glm::vec3 closestPointOnPlane = posSphere;
                    closestPointOnPlane.z = 0.0f; 

                    glm::vec3 delta2D = closestPointOnPlane - posCircle;
                    float dist2D = glm::length(delta2D);

                    if (dist2D <= circle->GetRadius()) {
                        float distZ = glm::abs(posSphere.z);
                        float radiusSphere = sphere->GetRadius();

                        if (distZ < radiusSphere) {
                            m.hasCollision = true;
                            m.bodyA = bodyA; m.bodyB = bodyB;
                            m.penetration = radiusSphere - distZ;
                            m.normal = glm::vec3(0.0f, 0.0f, (posSphere.z > 0.0f) ? -1.0f : 1.0f);
                            m.contactPoint = closestPointOnPlane; 
                        }
                    }
                    return m;
                }

                static std::vector<glm::vec3> GetBoxVertices(const IRigidBody* body, const ICollider* collider) {
                    std::vector<glm::vec3> vertices(8);
                    glm::vec3 halfExtents{0.0f};

                    if (collider->GetType() == ColliderType::Box2D) {
                        auto* box2D = static_cast<const BoxCollider2D*>(collider);
                        halfExtents = glm::vec3(box2D->GetHalfExtents(), 0.0f); 
                    } else {
                        auto* box3D = static_cast<const BoxCollider*>(collider);
                        halfExtents = box3D->GetHalfExtents();
                    }

                    glm::vec3 pos = body->GetPosition3D();
                    glm::mat3 rot = body->GetInverseInertiaTensor3D();

                    int idx = 0;
                    for (float x : {-halfExtents.x, halfExtents.x}) {
                        for (float y : {-halfExtents.y, halfExtents.y}) {
                            for (float z : {-halfExtents.z, halfExtents.z}) {
                                vertices[idx++] = pos + (rot * glm::vec3(x, y, z));
                            }
                        }
                    }
                    return vertices;
                }

                static std::vector<glm::vec3> GetBoxAxes(const IRigidBody* body) {
                    glm::mat3 rot = body->GetInverseInertiaTensor3D(); 
                    std::vector<glm::vec3> axes = {
                        glm::normalize(rot[0]),
                        glm::normalize(rot[1]), 
                        glm::normalize(rot[2])  
                    };
                    return axes;
                }

                static Projection ProjectPolygon(const std::vector<glm::vec3>& vertices, const glm::vec3& axis) {
                    float min = glm::dot(vertices[0], axis);
                    float max = min;
                    for (size_t i = 1; i < vertices.size(); ++i) {
                        float projection = glm::dot(vertices[i], axis);
                        if (projection < min) min = projection;
                        if (projection > max) max = projection;
                    }
                    return Projection{ min, max };
                }

                static ContactManifold BoxVsBox(IRigidBody* bodyA, IRigidBody* bodyB, const ICollider* colA, const ICollider* colB) {
                    ContactManifold m;
                    m.bodyA = bodyA;
                    m.bodyB = bodyB;

                    std::vector<glm::vec3> verticesA = GetBoxVertices(bodyA, colA);
                    std::vector<glm::vec3> verticesB = GetBoxVertices(bodyB, colB);

                    std::vector<glm::vec3> axesToTest;
                    
                    auto axesA = GetBoxAxes(bodyA);
                    auto axesB = GetBoxAxes(bodyB);
                    axesToTest.insert(axesToTest.end(), axesA.begin(), axesA.end());
                    axesToTest.insert(axesToTest.end(), axesB.begin(), axesB.end());

                    if (colA->GetType() == ColliderType::Box3D && colB->GetType() == ColliderType::Box3D) {
                        for (const auto& a : axesA) {
                            for (const auto& b : axesB) {
                                glm::vec3 crossAxis = glm::cross(a, b);
                                if (glm::length2(crossAxis) > 0.001f) {
                                    axesToTest.push_back(glm::normalize(crossAxis));
                                }
                            }
                        }
                    }

                    float minOverlap = std::numeric_limits<float>::max();
                    glm::vec3 smallestAxis{0.0f};

                    for (const auto& axis : axesToTest) {
                        if (glm::length2(axis) < 0.001f) continue;

                        Projection pA = ProjectPolygon(verticesA, axis);
                        Projection pB = ProjectPolygon(verticesB, axis);

                        if (!pA.Overlaps(pB)) {
                            return m; 
                        } else {
                            float overlap = pA.GetOverlap(pB);
                            if (overlap < minOverlap) {
                                minOverlap = overlap;
                                smallestAxis = axis;
                            }
                        }
                    }

                    m.hasCollision = true;
                    m.penetration = minOverlap;

                    glm::vec3 dir = bodyB->GetPosition3D() - bodyA->GetPosition3D();
                    if (glm::dot(smallestAxis, dir) < 0.0f) {
                        smallestAxis = -smallestAxis;
                    }
                    m.normal = smallestAxis;

                    glm::vec3 deepestVertex = verticesB[0];
                    float minDot = glm::dot(verticesB[0], -m.normal);
                    for (const auto& v : verticesB) {
                        float d = glm::dot(v, -m.normal);
                        if (d < minDot) {
                            minDot = d;
                            deepestVertex = v;
                        }
                    }
                    m.contactPoint = deepestVertex;

                    return m;
                }

                static ContactManifold BoxVsSphere(IRigidBody* bodyA, IRigidBody* bodyB, const ICollider* colA, const ICollider* colB) {
                    ContactManifold m;
                    m.bodyA = bodyA;
                    m.bodyB = bodyB;

                    const ICollider* boxCol = (colA->GetType() == ColliderType::Box2D || colA->GetType() == ColliderType::Box3D) ? colA : colB;
                    const ICollider* sphereCol = (colA == boxCol) ? colB : colA;
                    IRigidBody* boxBody = (colA == boxCol) ? bodyA : bodyB;
                    IRigidBody* sphereBody = (colA == boxCol) ? bodyB : bodyA;

                    float radius = (sphereCol->GetType() == ColliderType::Circle) ? 
                                    static_cast<const CircleCollider*>(sphereCol)->GetRadius() : 
                                    static_cast<const SphereCollider*>(sphereCol)->GetRadius();

                    glm::vec3 halfExtents = (boxCol->GetType() == ColliderType::Box2D) ? 
                                            glm::vec3(static_cast<const BoxCollider2D*>(boxCol)->GetHalfExtents(), 0.0f) : 
                                            static_cast<const BoxCollider*>(boxCol)->GetHalfExtents();

                    glm::vec3 spherePos = sphereBody->GetPosition3D();
                    glm::vec3 boxPos = boxBody->GetPosition3D();
                    glm::mat3 boxRot = boxBody->GetInverseInertiaTensor3D(); // Matrice orientamento

                    glm::vec3 delta = spherePos - boxPos;
                    glm::vec3 localSpherePos = glm::transpose(boxRot) * delta;

                    glm::vec3 localClosestPoint = glm::clamp(localSpherePos, -halfExtents, halfExtents);

                    glm::vec3 localDelta = localSpherePos - localClosestPoint;
                    float distance = glm::length(localDelta);

                    if (distance < 0.001f) {
                        float minDst = halfExtents.x - glm::abs(localSpherePos.x);
                        glm::vec3 axis(1.0f, 0.0f, 0.0f);
                        if (localSpherePos.x < 0.0f) axis = -axis;

                        float dstY = halfExtents.y - glm::abs(localSpherePos.y);
                        if (dstY < minDst) {
                            minDst = dstY;
                            axis = glm::vec3(0.0f, 1.0f, 0.0f);
                            if (localSpherePos.y < 0.0f) axis = -axis;
                        }
                        if (boxCol->GetType() == ColliderType::Box3D) {
                            float dstZ = halfExtents.z - glm::abs(localSpherePos.z);
                            if (dstZ < minDst) {
                                minDst = dstZ;
                                axis = glm::vec3(0.0f, 0.0f, 1.0f);
                                if (localSpherePos.z < 0.0f) axis = -axis;
                            }
                        }
                        m.hasCollision = true;
                        m.penetration = radius + minDst;
                        m.normal = boxRot * axis;
                        m.contactPoint = boxPos + (boxRot * localSpherePos);
                        
                        if (boxBody == bodyB) m.normal = -m.normal;
                        return m;
                    }

                    if (distance < radius) {
                        m.hasCollision = true;
                        m.penetration = radius - distance;
                        
                        glm::vec3 worldClosestPoint = boxPos + (boxRot * localClosestPoint);
                        m.normal = glm::normalize(spherePos - worldClosestPoint);
                        m.contactPoint = worldClosestPoint;

                        if (boxBody == bodyB) m.normal = -m.normal;
                    }

                    return m;
                }

                static ContactManifold NoCollision(IRigidBody*, IRigidBody*, const ICollider*, const ICollider*) {
                    return ContactManifold{};
                }

                static inline CollisionFunc m_collisionMatrix[4][4] = {
                    // Circle,          Box2D,       Sphere,          Box3D
                    { CircleVsCircle,   BoxVsSphere, SphereVsCircle,  BoxVsSphere }, // Circle
                    { nullptr,          BoxVsBox,    BoxVsSphere,     BoxVsBox    }, // Box2D
                    { nullptr,          nullptr,     SphereVsSphere,  BoxVsSphere }, // Sphere
                    { nullptr,          nullptr,     nullptr,         BoxVsBox    }  // Box3D
                };

            public:
                    
                static ContactManifold CheckCollision(IRigidBody* bodyA, IRigidBody* bodyB, const ICollider* colA, const ICollider* colB) {
                    ContactManifold manifold;

                    auto typeA = static_cast<int>(colA->GetType());
                    auto typeB = static_cast<int>(colB->GetType());

                    if (typeA <= typeB) {
                        manifold = m_collisionMatrix[typeA][typeB](bodyA, bodyB, colA, colB);
                    } else {
                        manifold = m_collisionMatrix[typeB][typeA](bodyB, bodyA, colB, colA);
                        manifold.normal = -manifold.normal;
                    }
                    manifold.bodyA = bodyA;
                    manifold.bodyB = bodyB;
                    return manifold;
                }
        
        };
    }
}

#endif