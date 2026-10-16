#include "GravityForce.h"

GravityForce::GravityForce(Vector3 gravity)
    : gravity(gravity)
{
}

void GravityForce::updateForce(Particle* particle, double dt)
{
    if (particle == nullptr || particle->getMass() <= 0.0f) {
        return;
    }

    Vector3 gravityForce(
        gravity.x * particle->getMass(),
        gravity.y * particle->getMass(),
        gravity.z * particle->getMass()
    );

    particle->applyForce(gravityForce);
}
