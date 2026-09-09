#ifndef IRIGID_BODY_PHYSICS_JPL
#define IRIGID_BODY_PHYSICS_JPL

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <concepts>
#include <memory>
#include "../collider/AABB.hpp"
#include "../collider/ICollider.hpp"

namespace jpl{
    namespace _physics{

        enum class RigidBodyType {
            Static, //A static rigid body is not affected by forces or torques and does not move.
            Kinematic, //A kinematic rigid body is not affected by forces or torques but can be moved by the user.
            Dynamic //A dynamic rigid body is affected by forces and torques and can move.
        };

        enum class PhysicDimension{
            Dim2D,
            Dim3D
        };


        template<typename T> struct PhysicsTraits;
        template<> struct PhysicsTraits<glm::vec2> {
            using AngularVelocity = float;
            using InertiaTensor   = float;
            using Orientation     = float;
        };
        template<> struct PhysicsTraits<glm::vec3> {
            using AngularVelocity = glm::vec3;
            using InertiaTensor   = glm::mat3;
            using Orientation     = glm::quat;
        };

        class IRigidBody {
        public:
            std::shared_ptr<ICollider> m_collider;

            virtual ~IRigidBody() = default;

            virtual PhysicDimension GetDimension() const = 0;
            virtual RigidBodyType GetType() const = 0;

            virtual glm::vec3 GetPosition3D() const = 0;
            virtual void SetPosition3D(const glm::vec3& pos) = 0;
            
            virtual glm::vec3 GetLinearVelocity3D() const = 0;
            virtual void SetLinearVelocity3D(const glm::vec3& vel) = 0;

            virtual glm::vec3 GetAngularVelocity3D() const = 0;
            virtual void SetAngularVelocity3D(const glm::vec3& angVel) = 0;

            virtual glm::mat3 GetInverseInertiaTensor3D() const = 0;
            virtual void SetInverseInertiaTensor3D(const glm::mat3& invInertia) = 0;

            virtual float GetMass() const = 0;
            virtual void SetMass(float mass) = 0;          
            virtual float GetInverseMass() const = 0;
            virtual void SetInverseMass(float invMass) = 0;

            virtual void ApplyForce3D(const glm::vec3& force) = 0;

            virtual void Integrate(float dt, const glm::vec3& gravity3D) = 0;

            virtual AABB GetWorldAABB() const = 0;

        };
    }

}

#endif