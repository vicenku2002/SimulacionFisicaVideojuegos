#pragma once

#include "Scene.h"
#include "RenderUtils.hpp"
#include <vector>
#include "ParticleGenerator.h"
#include "GravityForce.h"

//  Comparación de la gravedad como generador de fuerzas
// frente a la gravedad como aceleración constante, con distintas masas.
class Scene5 : public Scene {
public:
    explicit Scene5(std::string name) : Scene(std::move(name)) {}

    void init() override {
        // Dos instancias distintas del generador gravitatorio
        gravityNormal = new GravityForce(Vector3(0.0f, -9.81f, 0.0f));
        gravityLow = new GravityForce(Vector3(0.0f, -5.0f, 0.0f));   // "flotabilidad"

        m_forces.push_back(gravityNormal);
        m_forces.push_back(gravityLow);

        // Parámetros comunes de emisión
        const Vector3 vel(0.0f, 20.0f, 0.0f);      // Lanzamiento hacia arriba
        const Vector3 variance(1.0f, 1.0f, 1.0f);
        const Vector3 noAcc(0.0f, 0.0f, 0.0f);
        const float damping = 0.98f;
        const float rate = 3.0f;
        const float lifeTime = 5.0f;
        const float radius = 100.0f;

        // Rojo: fuerza gravitatoria normal, masa 1
        ParticleGenerator* red = new ParticleGenerator(Vector3(-30.0f, 0.0f, 0.0f), vel, noAcc, damping,
            Vector4(1.0f, 0.0f, 0.0f, 1.0f), rate, lifeTime, radius, 1.0f);
        red->addForceGenerator(gravityNormal);

        // Amarillo: fuerza gravitatoria normal, masa 50 (debe caer igual que el rojo)
        ParticleGenerator* yellow = new ParticleGenerator(Vector3(-10.0f, 0.0f, 0.0f), vel, noAcc, damping,
            Vector4(1.0f, 1.0f, 0.0f, 1.0f), rate, lifeTime, radius, 50.0f);
        yellow->addForceGenerator(gravityNormal);

        // Azul: fuerza gravitatoria reducida, masa 1 (sube más y cae más despacio)
        ParticleGenerator* blue = new ParticleGenerator(Vector3(10.0f, 0.0f, 0.0f), vel, noAcc, damping,
            Vector4(0.0f, 0.0f, 1.0f, 1.0f), rate, lifeTime, radius, 1.0f);
        blue->addForceGenerator(gravityLow);

        // Verde: gravedad como aceleración constante, sin generadores de fuerza
        ParticleGenerator* green = new ParticleGenerator(Vector3(30.0f, 0.0f, 0.0f), vel,
            Vector3(0.0f, -9.81f, 0.0f), damping,
            Vector4(0.0f, 1.0f, 0.0f, 1.0f), rate, lifeTime, radius, 1.0f);

        for (ParticleGenerator* pg : { red, yellow, blue, green }) {
            pg->setVelocityVariance(variance);
            m_generators.push_back(pg);
        }
    }

    void update(double dt) override {
        // Primero se actualizan las fuerzas (una vez por frame)...
        for (ForceGenerator* f : m_forces) f->update(dt);

        // ...y luego los generadores de partículas
        for (ParticleGenerator* pg : m_generators) pg->update(dt);
    }

    void keyPress(unsigned char key, const physx::PxTransform& camera) override {
        // Reiniciar: borrar todas las partículas
        if (key == 'r' || key == 'R') {
            for (ParticleGenerator* pg : m_generators) pg->clear();
        }
    }

    void cleanup() override {
        for (ParticleGenerator* pg : m_generators) delete pg;   // El destructor llama a clear()
        m_generators.clear();

        for (ForceGenerator* f : m_forces) delete f;
        m_forces.clear();
        gravityNormal = nullptr;
        gravityLow = nullptr;
    }

private:
    std::vector<ParticleGenerator*> m_generators;
    std::vector<ForceGenerator*> m_forces;
    GravityForce* gravityNormal = nullptr;
    GravityForce* gravityLow = nullptr;
};