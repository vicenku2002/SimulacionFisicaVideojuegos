#pragma once

#include "Scene.h"
#include "RenderUtils.hpp"
#include <vector>
#include <iostream>
#include "ParticleGenerator.h"
#include "GravityForce.h"
#include "WindForce.h"

// viento aplicado a partículas de distintas masas
class Scene6 : public Scene {
public:
    explicit Scene6(std::string name) : Scene(std::move(name)) {}

    void init() override {
        gravity = new GravityForce(Vector3(0.0f, -9.81f, 0.0f));

        // Viento hacia +x en una franja entre y = 5 e y = 35
        wind = new WindForce(Vector3(20.0f, 0.0f, 0.0f),     // velocidad del viento
            Vector3(0.0f, 20.0f, 0.0f),     // centro de la caja
            Vector3(40.0f, 15.0f, 30.0f),   // semitamaño de la caja
            1.0f, 0.0f);                    // k1, k2

        m_forces.push_back(gravity);
        m_forces.push_back(wind);

        const Vector3 vel(0.0f, 0.0f, 0.0f);
        const Vector3 variance(0.5f, 0.2f, 0.5f);
        const Vector3 noAcc(0.0f, 0.0f, 0.0f);
        const float damping = 0.98f;
        const float rate = 3.0f;
        const float lifeTime = 8.0f;
        const float radius = 150.0f;

        // Blanco: masa 0.5 (muy ligera, el viento la arrastra mucho)
        ParticleGenerator* light = new ParticleGenerator(Vector3(-20.0f, 45.0f, -15.0f), vel, noAcc, damping,
            Vector4(1.0f, 1.0f, 1.0f, 1.0f), rate, lifeTime, radius, 0.5f);

        // Cian: masa 2
        ParticleGenerator* medium = new ParticleGenerator(Vector3(-20.0f, 45.0f, 0.0f), vel, noAcc, damping,
            Vector4(0.0f, 1.0f, 1.0f, 1.0f), rate, lifeTime, radius, 2.0f);

        // Naranja: masa 10 (pesada, apenas se desvía)
        ParticleGenerator* heavy = new ParticleGenerator(Vector3(-20.0f, 45.0f, 15.0f), vel, noAcc, damping,
            Vector4(1.0f, 0.5f, 0.0f, 1.0f), rate, lifeTime, radius, 10.0f);

        for (ParticleGenerator* pg : { light, medium, heavy }) {
            pg->setVelocityVariance(variance);
            pg->addForceGenerator(gravity);
            pg->addForceGenerator(wind);
            m_generators.push_back(pg);
        }
    }

    void update(double dt) override {
        for (ForceGenerator* f : m_forces) f->update(dt);
        for (ParticleGenerator* pg : m_generators) pg->update(dt);
    }

    void keyPress(unsigned char key, const physx::PxTransform& camera) override {
        switch (toupper(key)) {
        case 'R': // Borrar partículas
            for (ParticleGenerator* pg : m_generators) pg->clear();
            break;
        case 'G': // Activar/desactivar el viento
            wind->setActive(!wind->isActive());
            std::cout << "Viento " << (wind->isActive() ? "activado" : "desactivado") << "\n";
            break;
        case 'T': // Alternar entre flujo laminar (k2 = 0) y turbulento (k2 = 0.1)
            wind->setK2(wind->getK2() > 0.0f ? 0.0f : 0.1f);
            std::cout << "k2 = " << wind->getK2() << "\n";
            break;
        }
    }

    void cleanup() override {
        for (ParticleGenerator* pg : m_generators) delete pg;
        m_generators.clear();

        for (ForceGenerator* f : m_forces) delete f;
        m_forces.clear();
        gravity = nullptr;
        wind = nullptr;
    }

private:
    std::vector<ParticleGenerator*> m_generators;
    std::vector<ForceGenerator*> m_forces;
    GravityForce* gravity = nullptr;
    WindForce* wind = nullptr;
};