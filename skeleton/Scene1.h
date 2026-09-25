#pragma once
#pragma once

#include "Scene.h"
#include "RenderUtils.hpp"
#include <vector>
#include "Vector3D.h"
#include "Particle.h"

using namespace std;

class Scene1 : public Scene {
public:
    explicit Scene1(std::string name) : Scene(std::move(name)), m_particle(nullptr) {}

    void init() override {
        // Ejemplo: Creación de una esfera usando las utilidades de render existentes
        //physx::PxShape* shape = CreateShape(physx::PxSphereGeometry(2.0f));
        m_transform = physx::PxTransform(physx::PxVec3(0.0f, 10.0f, 0.0f));

		// Crear la partícula para esta escena: (posición inicial, velocidad inicial, aceleración, damping)
        m_particle = new Particle(Vector3(0.0f, 2.0f, 0.0f), Vector3(1.0f, 0.0f, 0.0f), Vector3(2.0f, 0.0f, 0.0f));
    }

    void update(double dt) override {
        // Lógica/Integración del alumno (por ejemplo, movimiento simple)
        //m_transform.p.y -= static_cast<float>(9.8 * dt);
        if (m_particle) {
            m_particle->integrate(dt);
        }
    }

    void keyPress(unsigned char key, const physx::PxTransform& camera) override {
        if (key == 'r' || key == 'R') {
            m_transform.p = physx::PxVec3(0.0f, 10.0f, 0.0f); // Reset
        }
    }

    void cleanup() override {
        // Liberar la partícula si existe
        if (m_particle) {
            delete m_particle;
            m_particle = nullptr;
        }

        for (RenderItem* item : m_renderItems)
            if (item) {
                item->release(); // Deregistra y destruye el item
                item = nullptr;
            }
    }

private:
    vector<physx::PxTransform> transforms;
    physx::PxTransform m_transform;
    vector<RenderItem*> m_renderItems;
    Particle* m_particle;
   
};
