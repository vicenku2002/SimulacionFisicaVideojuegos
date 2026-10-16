#pragma once

#include "Scene.h"
#include "RenderUtils.hpp"
#include <vector>
#include "Vector3D.h"
#include "Particle.h"
#include "ParticleGenerator.h"
#include "GravityForce.h"

using namespace std;

class Scene3 : public Scene {
public:
    explicit Scene3(std::string name) : Scene(std::move(name)), PG(nullptr) {}

    void init() override {

        gravityNormal = new GravityForce(Vector3(0.0f, -9.81f, 0.0f));

        PG = new ParticleGenerator(Vector3(0.0f, 10.0f, 0.0f), Vector3(0.0f, 0.0f, 0.0f), Vector3(0.0f, 0.0f, 0.0f), 0.98f, Vector4(1.0f, 1.0f, 1.0f, 1.0f), 20.0f, 5.0f, 100.0f, 0.01f);
        PG->setVelocityVariance(Vector3(10.0f, 1.0f, 10.0f));
        PG->addForceGenerator(gravityNormal);
    }

    void update(double dt) override {
        PG->update(dt);
    }

    void keyPress(unsigned char key, const physx::PxTransform& camera) override {

    }

    void cleanup() override {
        PG->clear();
        delete PG;
        PG = nullptr;

		delete gravityNormal;
		gravityNormal = nullptr;

    }

private:
    ParticleGenerator* PG;
    GravityForce* gravityNormal;

};

