#include "WindForce.h"
#include <cmath>

WindForce::WindForce(Vector3 windVelocity, Vector3 center, Vector3 halfSize, float k1, float k2)
    : windVelocity(windVelocity), center(center), halfSize(halfSize), k1(k1), k2(k2), active(true)
{
}

Vector3 WindForce::getWindVelocity(const Vector3& pos) const
{
    // Viento uniforme: la posición no influye
    return windVelocity;
}

bool WindForce::isInside(const Vector3& pos) const
{
    return std::abs(pos.x - center.x) <= halfSize.x &&
        std::abs(pos.y - center.y) <= halfSize.y &&
        std::abs(pos.z - center.z) <= halfSize.z;
}

void WindForce::updateForce(Particle* particle, double dt)
{
    if (!active || particle == nullptr || particle->getMass() <= 0.0f) return;

    const Vector3 pos = particle->getPosition();
    if (!isInside(pos)) return;

    // Diferencia de velocidad entre el aire y la partícula
    const Vector3 diff = getWindVelocity(pos) - particle->getVelocity();

    // Término laminar + término turbulento
    const Vector3 force = diff * k1 + diff * (k2 * diff.magnitude());

    particle->applyForce(force);
}