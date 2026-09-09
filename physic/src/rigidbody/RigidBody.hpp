#ifndef RIGID_BODY_PHYSICS_JPL
#define RIGID_BODY_PHYSICS_JPL

#include "IRigidBody.hpp"


namespace jpl{
    namespace _physics{

        template <typename T>
        concept GlmVector = std::same_as<T, glm::vec2> || std::same_as<T, glm::vec3>;

        template <GlmVector T>
        class RigidBody final : public IRigidBody {

            public:
                using AngVelT  = typename PhysicsTraits<T>::AngularVelocity;
                using InertiaT = typename PhysicsTraits<T>::InertiaTensor;
                using RotT     = typename PhysicsTraits<T>::Orientation;

            private:
                RigidBodyType m_type;
                float m_mass = 1.0f;
                float m_inverseMass = 1.0f;
                InertiaT m_inertiaTensor{1.0f};
                InertiaT m_inverseInertiaTensor{1.0f};
                T m_position{0.0f};
                T m_linearVelocity{0.0f};
                T m_forceAccumulator{0.0f};
                RotT m_orientation{};
                AngVelT m_angularVelocity{0.0f};
                AngVelT m_torqueAccumulator{0.0f};
                

            public:

                RigidBody(RigidBodyType type = RigidBodyType::Dynamic) : m_type(type) {

                }

                PhysicDimension GetDimension() const override {
                    if constexpr (std::same_as<T, glm::vec2>) 
                        return PhysicDimension::Dim2D;
                    return PhysicDimension::Dim3D;
                }

                RigidBodyType GetType() const override { 
                    return m_type; 
                }

                glm::vec3 GetPosition3D() const override {
                    if constexpr (std::same_as<T, glm::vec2>) 
                        return glm::vec3(m_position.x, m_position.y, 0.0f);
                    else
                        return m_position;
                }
                void SetPosition3D(const glm::vec3& pos) override {
                    if constexpr (std::same_as<T, glm::vec2>) 
                        m_position = glm::vec2(pos.x, pos.y);
                    else 
                        m_position = pos;
                }

                glm::vec3 GetLinearVelocity3D() const override {
                    if constexpr (std::same_as<T, glm::vec2>) 
                        return glm::vec3(m_linearVelocity.x, m_linearVelocity.y, 0.0f);
                    else
                        return m_linearVelocity;
                }
                void SetLinearVelocity3D(const glm::vec3& vel) override {
                    if constexpr (std::same_as<T, glm::vec2>) 
                        m_linearVelocity = glm::vec2(vel.x, vel.y);
                    else 
                        m_linearVelocity = vel;
                }

                glm::vec3 GetAngularVelocity3D() const override {
                    if constexpr (std::same_as<T, glm::vec2>) 
                        return glm::vec3(0.0f, 0.0f, m_angularVelocity);
                    else
                        return m_angularVelocity;
                }
                void SetAngularVelocity3D(const glm::vec3& angVel) override {
                    if constexpr (std::same_as<T, glm::vec2>) 
                        m_angularVelocity = angVel.z;
                    else 
                        m_angularVelocity = angVel;
                }

                glm::mat3 GetInverseInertiaTensor3D() const override {
                    if constexpr (std::same_as<T, glm::vec2>)
                        return glm::mat3(
                            0.0f, 0.0f, 0.0f, 
                            0.0f, 0.0f, 0.0f,  
                            0.0f, 0.0f, m_inverseInertiaTensor
                        );
                    else 
                        return m_inverseInertiaTensor;
                }
                void SetInverseInertiaTensor3D(const glm::mat3& invInertia) override {
                    if constexpr (std::same_as<T, glm::vec2>)
                        m_inverseInertiaTensor = invInertia[2][2]; 
                    else 
                        m_inverseInertiaTensor = invInertia;
                }

                void ApplyForce3D(const glm::vec3& force) override {
                    if constexpr (std::same_as<T, glm::vec2>) 
                        m_forceAccumulator += glm::vec2(force.x, force.y);
                    else 
                        m_forceAccumulator += force;
                }

                float GetInverseMass() const override { 
                    return m_inverseMass; 
                }
                void SetInverseMass(float invMass) override { 
                    m_inverseMass = invMass; 
                    m_mass = (invMass > 0.0f) ? 1.0f / invMass : 0.0f; 
                }

                float GetMass() const override{
                    return m_mass;
                }
                void SetMass(float mass) override {
                    if (m_type == RigidBodyType::Static) {
                        m_mass = 0.0f;
                        m_inverseMass = 0.0f;
                        return;
                    }
                    m_mass = mass;
                    if (mass > 0.0f) 
                        m_inverseMass = 1.0f / mass;
                    else 
                        m_inverseMass = 0.0f; 
                }
                
                const T& GetPosition() const { 
                    return m_position; 
                }
                void SetPosition(const T& pos) { 
                    m_position = pos; 
                }

                void ApplyForce(const T& force) { 
                    m_forceAccumulator += force; 
                }

                
                void Integrate(float dt, const glm::vec3& gravity3D) override {
                    if (m_type == RigidBodyType::Static || dt <= 0.0f) return;
                    
                    T gravity;
                    if constexpr (std::same_as<T, glm::vec2>) 
                        gravity = glm::vec2(gravity3D.x, gravity3D.y);
                    else 
                        gravity = gravity3D;

                    T linearAcceleration = gravity + (m_forceAccumulator * m_inverseMass);
                    m_linearVelocity += linearAcceleration * dt;
                    m_position += m_linearVelocity * dt;
                    
                    if constexpr (std::same_as<T, glm::vec2>) {
                        float angularAcceleration = m_torqueAccumulator * m_inverseInertiaTensor;
                        m_angularVelocity += angularAcceleration * dt;
                        m_orientation += m_angularVelocity * dt;
                    } else {
                        glm::vec3 angularAcceleration = m_inverseInertiaTensor * m_torqueAccumulator;
                        m_angularVelocity += angularAcceleration * dt;
                        glm::quat qVelocity(0.0f, m_angularVelocity.x, m_angularVelocity.y, m_angularVelocity.z);
                        m_orientation += (qVelocity * m_orientation) * (dt * 0.5f);
                        m_orientation = glm::normalize(m_orientation);
                    }
                    m_forceAccumulator = T(0.0f);
                    m_torqueAccumulator = AngVelT(0.0f);
                }

                AABB GetWorldAABB() const override {
                    if (!m_collider) return AABB{};

                    glm::mat3 rotMatrix(1.0f);
                    if constexpr (std::same_as<T, glm::vec2>) {
                        // Genera matrice di rotazione 2D attorno all'asse Z, mappata in una mat3
                        float cosA = glm::cos(m_orientation);
                        float sinA = glm::sin(m_orientation);
                        rotMatrix[0] = glm::vec3(cosA, sinA, 0.0f);
                        rotMatrix[1] = glm::vec3(-sinA, cosA, 0.0f);
                        rotMatrix[2] = glm::vec3(0.0f, 0.0f, 1.0f);
                    } else {
                        // Converte direttamente il quaternione 3D in una matrice mat3 di GLM
                        rotMatrix = glm::mat3_cast(m_orientation);
                    }

                    // Richiama l'algoritmo geometrico del collider usando lo spazio unificato 3D
                    return m_collider->ComputeWorldAABB(GetPosition3D(), rotMatrix);
                }
        };
        using RigidBody2D = RigidBody<glm::vec2>;
        using RigidBody3D = RigidBody<glm::vec3>;

    }
}

#endif