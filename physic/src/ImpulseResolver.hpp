#ifndef IMPULSE_RESOLVER_PHYSICS_JPL
#define IMPULSE_RESOLVER_PHYSICS_JPL

#include <glm/glm.hpp>
#include "narrowphase/Narrowphase.hpp"

namespace jpl{
    namespace _physics{

        class ImpulseResolver {
        public:

            static inline constexpr float POSITION_CORRECTION_SLOP = 0.01f;
            static inline constexpr float POSITION_CORRECTION_PERCENT = 0.2f;

            static void ResolveCollision(ContactManifold& manifold) {
                if (!manifold.hasCollision) return;

                IRigidBody* bodyA = manifold.bodyA;
                IRigidBody* bodyB = manifold.bodyB;

                float invMassA = bodyA->GetInverseMass();
                float invMassB = bodyB->GetInverseMass();
                float totalInvMass = invMassA + invMassB;

                if (totalInvMass == 0.0f) return;

                glm::vec3 rA = manifold.contactPoint - bodyA->GetPosition3D();
                glm::vec3 rB = manifold.contactPoint - bodyB->GetPosition3D();

                glm::vec3 velA = bodyA->GetLinearVelocity3D();
                glm::vec3 velB = bodyB->GetLinearVelocity3D();
                
                glm::vec3 angVelA = bodyA->GetAngularVelocity3D(); 
                glm::vec3 angVelB = bodyB->GetAngularVelocity3D();

                glm::vec3 contactVelA = velA + glm::cross(angVelA, rA);
                glm::vec3 contactVelB = velB + glm::cross(angVelB, rB);

                glm::vec3 relativeVelocity = contactVelB - contactVelA;
                float velAlongNormal = glm::dot(relativeVelocity, manifold.normal);

                if (velAlongNormal > 0.0f) {
                    PositionCorrection(manifold, invMassA, invMassB, totalInvMass);
                    return;
                }

                glm::vec3 angularComponentA = glm::cross(bodyA->GetInverseInertiaTensor3D() * glm::cross(rA, manifold.normal), rA);
                glm::vec3 angularComponentB = glm::cross(bodyB->GetInverseInertiaTensor3D() * glm::cross(rB, manifold.normal), rB);
                
                float angularFactor = glm::dot(angularComponentA + angularComponentB, manifold.normal);

                float restitution = 0.5f;

                float j = -(1.0f + restitution) * velAlongNormal;
                j /= (totalInvMass + angularFactor);

                glm::vec3 impulse = j * manifold.normal;

                bodyA->SetLinearVelocity3D(velA - (invMassA * impulse));
                bodyB->SetLinearVelocity3D(velB + (invMassB * impulse));

                bodyA->SetAngularVelocity3D(angVelA - (bodyA->GetInverseInertiaTensor3D() * glm::cross(rA, impulse)));
                bodyB->SetAngularVelocity3D(angVelB + (bodyB->GetInverseInertiaTensor3D() * glm::cross(rB, impulse)));

                contactVelA = bodyA->GetLinearVelocity3D() + glm::cross(bodyA->GetAngularVelocity3D(), rA);
                contactVelB = bodyB->GetLinearVelocity3D() + glm::cross(bodyB->GetAngularVelocity3D(), rB);
                relativeVelocity = contactVelB - contactVelA;

                glm::vec3 tangent = relativeVelocity - (glm::dot(relativeVelocity, manifold.normal) * manifold.normal);
                float tangentLength = glm::length(tangent);

                if (tangentLength > 0.0001f) {
                    tangent = glm::normalize(tangent);

                    glm::vec3 angularFrictionA = glm::cross(bodyA->GetInverseInertiaTensor3D() * glm::cross(rA, tangent), rA);
                    glm::vec3 angularFrictionB = glm::cross(bodyB->GetInverseInertiaTensor3D() * glm::cross(rB, tangent), rB);
                    float angularFrictionFactor = glm::dot(angularFrictionA + angularFrictionB, tangent);

                    float jt = -glm::dot(relativeVelocity, tangent);
                    jt /= (totalInvMass + angularFrictionFactor);

                    float frictionCoefficient = 0.3f;
                    float maxFriction = j * frictionCoefficient;
                    jt = glm::clamp(jt, -maxFriction, maxFriction);

                    glm::vec3 frictionImpulse = jt * tangent;

                    bodyA->SetLinearVelocity3D(bodyA->GetLinearVelocity3D() - (invMassA * frictionImpulse));
                    bodyB->SetLinearVelocity3D(bodyB->GetLinearVelocity3D() + (invMassB * frictionImpulse));

                    bodyA->SetAngularVelocity3D(bodyA->GetAngularVelocity3D() - (bodyA->GetInverseInertiaTensor3D() * glm::cross(rA, frictionImpulse)));
                    bodyB->SetAngularVelocity3D(bodyB->GetAngularVelocity3D() + (bodyB->GetInverseInertiaTensor3D() * glm::cross(rB, frictionImpulse)));
                }

                PositionCorrection(manifold, invMassA, invMassB, totalInvMass);
            }

            private:
                static void PositionCorrection(const ContactManifold& manifold, float invMassA, float invMassB, float totalInvMass) {
                    float correctionMagnitude = std::max(manifold.penetration - POSITION_CORRECTION_SLOP, 0.0f) / totalInvMass * POSITION_CORRECTION_PERCENT;
                    glm::vec3 correctionVector = correctionMagnitude * manifold.normal;

                    manifold.bodyA->SetPosition3D(manifold.bodyA->GetPosition3D() - (invMassA * correctionVector));
                    manifold.bodyB->SetPosition3D(manifold.bodyB->GetPosition3D() + (invMassB * correctionVector));
                }
        };
    }
}

#endif