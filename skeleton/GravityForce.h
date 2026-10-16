#pragma once

#include "ForceGenerator.h"
#include "Vector3D.h"

class GravityForce : public ForceGenerator
{
public:
    explicit GravityForce(Vector3 gravity);

    void updateForce(Particle* particle, double dt) override;

private:
    Vector3 gravity;
};