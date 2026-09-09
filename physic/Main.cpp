#include "src/rigidbody/RigidBody.hpp"
#include "src/collider/CircleCollider.hpp"
#include "src/collider/QuadCollider.hpp"
#include "src/collider/SphereCollider.hpp"
#include "src/collider/BoxCollider.hpp"
#include "src/narrowphase/Narrowphase.hpp"
#include "src/ImpulseResolver.hpp"
#include "src/narrowphase/ContactMainfold.hpp"
#include "src/narrowphase/Projection.hpp"
#include "src/narrowphase/SpatialHash.hpp"

#include <iostream>
#include <cassert>
#include <vector>
#include <memory>
#include <glm/glm.hpp>

// Struttura fittizia PhysicsWorld per orchestrare il test
class PhysicsWorldTest {
public:
    std::vector<jpl::_physics::IRigidBody*> allBodies;
    jpl::_physics::SpatialHash spatialHash{ 4.0f };

    jpl::_physics::IRigidBody* CreateBody2D(jpl::_physics::RigidBodyType type, glm::vec2 pos, float radius) {
        m_pool2D.push_back(new jpl::_physics::RigidBody2D(type));
        jpl::_physics::RigidBody2D* ptr = m_pool2D.back();
        ptr->SetPosition(pos);
        float mass = 1.0f; 
        ptr->SetMass(mass); // Questo internamente imposta m_mass e m_inverseMass (1.0f / mass)
        if (type == jpl::_physics::RigidBodyType::Dynamic) {
            // Calcolo momento d'inerzia per un cerchio pieno: I = 0.5 * mass * radius^2
            float inertia = 0.5f * mass * (radius * radius);
            // Impostiamo l'inerzia inversa float nativa del corpo 2D
            ptr->SetInverseInertiaTensor3D(glm::mat3(
                0.0f, 0.0f, 0.0f,
                0.0f, 0.0f, 0.0f,
                0.0f, 0.0f, 1.0f / inertia
            ));
        } else {
            // Se è statico, l'inerzia inversa deve essere zero su tutti gli assi
            ptr->SetInverseInertiaTensor3D(glm::mat3(0.0f));
        }
        jpl::_physics::IRigidBody* interfaceRB = static_cast<jpl::_physics::IRigidBody*>(ptr);
        interfaceRB->m_collider = std::make_shared<jpl::_physics::CircleCollider>(radius);
        allBodies.push_back(interfaceRB);
        return interfaceRB;
    }

    jpl::_physics::IRigidBody* CreateBody3D(jpl::_physics::RigidBodyType type, glm::vec3 pos, float radius) {
        m_pool3D.push_back(new jpl::_physics::RigidBody3D(type));
        jpl::_physics::RigidBody3D* ptr = m_pool3D.back();
        ptr->SetPosition(pos);
        float mass = 1.0f;
        ptr->SetMass(mass);

        if (type == jpl::_physics::RigidBodyType::Dynamic) {
            // Inerzia sfera: I = 0.4 * mass * radius^2
            float inertia = 0.4f * mass * (radius * radius);
            float invInertia = 1.0f / inertia;
            
            // Impostiamo la matrice 3x3 diagonale per la sfera
            ptr->SetInverseInertiaTensor3D(glm::mat3(
                invInertia, 0.0f,       0.0f,
                0.0f,       invInertia, 0.0f,
                0.0f,       0.0f,       invInertia
            ));
        } else {
            ptr->SetInverseInertiaTensor3D(glm::mat3(0.0f));
        }
        jpl::_physics::IRigidBody* interfaceRB = static_cast<jpl::_physics::IRigidBody*>(ptr);
        interfaceRB->m_collider = std::make_shared<jpl::_physics::SphereCollider>(radius);
        allBodies.push_back(interfaceRB);
        return interfaceRB;
    }

    void Step(float dt) {
        glm::vec3 gravity(0.0f, 0.0f, 0.0f); // Disattiviamo la gravità per i test di collisione controllati
        for (jpl::_physics::IRigidBody* body : allBodies) {
            body->Integrate(dt, gravity);
        }

        // Broadphase via Spatial Hash
        spatialHash.Clear();
        for (jpl::_physics::IRigidBody* body : allBodies) {
            spatialHash.Insert(body);
        }

        // Narrowphase & Resolution
        for (size_t i = 0; i < allBodies.size(); ++i) {
            jpl::_physics::IRigidBody* bodyA = allBodies[i];
            std::vector<jpl::_physics::IRigidBody*> candidates = spatialHash.GetCandidates(bodyA);

            for (jpl::_physics::IRigidBody* bodyB : candidates) {
                if (bodyA >= bodyB) continue;

                jpl::_physics::ContactManifold manifold = jpl::_physics::Narrowphase::CheckCollision(
                    bodyA, bodyB, bodyA->m_collider.get(), bodyB->m_collider.get()
                );

                if (manifold.hasCollision) {
                    jpl::_physics::ImpulseResolver::ResolveCollision(manifold);
                }
            }
        }
    }

private:
    std::vector<jpl::_physics::RigidBody2D*> m_pool2D;
    std::vector<jpl::_physics::RigidBody3D*> m_pool3D;
};

// =========================================================================
// RUNNER DEI TEST
// =========================================================================
int main() {
    std::cout << "--- INIZIO TEST MOTORE FISICO IBRIDO ---\n\n";

    // ---------------------------------------------------------------------
    // TEST 1: Collisione Nativa 2D (Cerchio vs Cerchio)
    // ---------------------------------------------------------------------
    {
        std::cout << "[Test 1] Verifica Collisione Nativa 2D... ";
        PhysicsWorldTest world;
        
        // Otteniamo i puntatori polimorfici
        jpl::_physics::IRigidBody* c1 = world.CreateBody2D(jpl::_physics::RigidBodyType::Dynamic, glm::vec2(0.0f, 0.0f), 1.0f);
        jpl::_physics::IRigidBody* c2 = world.CreateBody2D(jpl::_physics::RigidBodyType::Dynamic, glm::vec2(1.5f, 0.0f), 1.0f);
        
        // Usiamo esclusivamente l'API unificata SetLinearVelocity3D
        c1->SetLinearVelocity3D(glm::vec3(5.0f, 0.0f, 0.0f));
        c2->SetLinearVelocity3D(glm::vec3(-5.0f, 0.0f, 0.0f));

        world.Step(0.016f);

        // Verifichiamo il risultato leggendo sempre tramite l'API 3D unificata
        assert(c1->GetLinearVelocity3D().x < 0.0f);
        assert(c2->GetLinearVelocity3D().x > 0.0f);
        std::cout << "PASSATO (I corpi sono rimbalzati correttamente)\n";
    }

    // ---------------------------------------------------------------------
    // TEST 2: Isolamento Spaziale a Runtime (Oggetto 3D "Sopra" il piano 2D)
    // ---------------------------------------------------------------------
    {
        std::cout << "[Test 2] Verifica Isolamento Spaziale (Z disallineata)... ";
        PhysicsWorldTest world;

        // Un cerchio 2D a (0,0,0) nel mondo unificato
        world.CreateBody2D(jpl::_physics::RigidBodyType::Dynamic, glm::vec2(0.0f, 0.0f), 1.0f);
        
        // Una sfera 3D posizionata esattamente sopra il cerchio sulle coordinate X-Y, 
        // ma fluttuante nello spazio a Z = 10.0f.
        jpl::_physics::IRigidBody* s3D = world.CreateBody3D(jpl::_physics::RigidBodyType::Dynamic, glm::vec3(0.0f, 0.0f, 10.0f), 1.0f);
        s3D->SetLinearVelocity3D(glm::vec3(0.0f, 0.0f, -10.0f)); // Cade verso il basso lungo Z

        // Eseguiamo un passo. Lo Spatial Hash deve accorgersi che gli oggetti sono in celle Z diverse
        // e non deve nemmeno generare candidati di collisione.
        world.Step(0.016f);

        std::vector<jpl::_physics::IRigidBody*> candidates = world.spatialHash.GetCandidates(s3D);
        assert(candidates.empty()); // Non devono esserci candidati vicini lungo l'asse Z
        std::cout << "PASSATO (L'asse Z ha isolato correttamente i mondi)\n";
    }

    // ---------------------------------------------------------------------
    // TEST 3: Collisione Ibrida 2D vs 3D a Runtime
    // ---------------------------------------------------------------------
    {
        std::cout << "[Test 3] Verifica Collisione Ibrida 2D vs 3D... ";
        PhysicsWorldTest world;

        // Un cerchio 2D statico (es. un bumper o pavimento sul piano Z=0)
        jpl::_physics::IRigidBody* bumper2D = world.CreateBody2D(jpl::_physics::RigidBodyType::Static, glm::vec2(0.0f, 0.0f), 2.0f);
        bumper2D->SetInverseMass(0.0f); // Massa infinita

        // Una sfera 3D dinamica che arriva dalla profondità (Z = 0.5f) e penetra nel piano Z=0
        jpl::_physics::IRigidBody* ball3D = world.CreateBody3D(jpl::_physics::RigidBodyType::Dynamic, glm::vec3(0.0f, 0.0f, 0.5f), 1.0f);
        ball3D->SetLinearVelocity3D(glm::vec3(0.0f, 0.0f, -5.0f)); // Spinge contro il piano Z

        world.Step(0.016f);

        // L'Impulse Resolver deve aver respinto la sfera 3D lungo l'asse Z a causa del contatto col cerchio
        assert(ball3D->GetLinearVelocity3D().z > 0.0f);
        std::cout << "PASSATO (La sfera 3D è rimbalzata sul vincolo 2D)\n";
    }

    // ---------------------------------------------------------------------
    // TEST 4: Verifica Vincolo Rotazionale 2D (Momento Torcente)
    // ---------------------------------------------------------------------
    {
        std::cout << "[Test 4] Verifica Stabilità Angolare 2D... ";
        PhysicsWorldTest world;

        jpl::_physics::IRigidBody* body2D = world.CreateBody2D(jpl::_physics::RigidBodyType::Dynamic, glm::vec2(0.0f, 0.0f), 1.0f);
        
        // Simuliamo una spinta asimmetrica tridimensionale proveniente dall'Impulse Resolver
        // (es. un urto che genera una rotazione complessa in XYZ)
        body2D->SetAngularVelocity3D(glm::vec3(4.0f, 9.0f, 2.0f));

        // Verifichiamo che l'override dell'interfaccia abbia correttamente 
        // filtrato le componenti isolando solo l'asse Z nativo del 2D
        assert(body2D->GetAngularVelocity3D().x == 0.0f);
        assert(body2D->GetAngularVelocity3D().y == 0.0f);
        assert(body2D->GetAngularVelocity3D().z == 2.0f);
        std::cout << "PASSATO (Assi X e Y filtrati, asse Z preservato)\n";
    }

    std::cout << "\n--- TUTTI I TEST SONO STATI SUPERATI CON SUCCESSO! ---\n";
    return 0;
}
