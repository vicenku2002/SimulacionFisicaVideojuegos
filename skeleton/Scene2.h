#pragma once

#include "Scene.h"
#include "RenderUtils.hpp"
#include <vector>
#include "Vector3D.h"
#include "Particle.h"
#include "ParticleGenerator.h"

using namespace std;

class Scene2 : public Scene {
public:
    explicit Scene2(std::string name) : Scene(std::move(name)), PG(nullptr) {}

    void init() override {
		PG = new ParticleGenerator(Vector3(0.0f, 10.0f, 0.0f), Vector3(0.0f, 0.0f, 0.0f), Vector3(0.0f, 0.0f, 0.0f), 0.98f, Vector4(1.0f, 0.0f, 0.0f, 1.0f), 10.0f, 5.0f, 100.0f, 1.0f);
        PG->setVelocityVariance(Vector3(2.0f, 1.0f, 2.0f));
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
       
    }

private:
	ParticleGenerator* PG;

};

