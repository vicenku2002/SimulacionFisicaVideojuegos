#include "ParticleGenerator.h"
#include <cmath>
#include <random>
#include <algorithm>

ParticleGenerator::ParticleGenerator(
    Vector3 Pos,
    Vector3 Vel,
    Vector3 Acc,
    float Damp,
    Vector4 Color,
    float EmissionRate,
    float LifeTime,
    float ActionRadius,
    float Mass
)
    : position(Pos),
      velocity(Vel),
      velocityVariance(0.0f, 0.0f, 0.0f),
      acceleration(Acc),
      gravity(0.0f, -10.0f, 0.0f),
      damping(Damp),
      color(Color),
      mass(Mass),
      emissionRate(EmissionRate),
      lifeTime(LifeTime),
      actionRadius(ActionRadius),
      accumulatedTime(0.0),
      rng(std::random_device{}()),
      distX(0.0f, 1.0f),
      distY(0.0f, 1.0f),
      distZ(0.0f, 1.0f)
{
}

ParticleGenerator::~ParticleGenerator()
{
    clear();
}

Vector3 ParticleGenerator::generateRandomVelocity()
{
    // Una desviación estándar de cero significa
    // que no se añade variación a ese eje.
    const float randomX =
        velocityVariance.x > 0.0f ? distX(rng) * velocityVariance.x : 0.0f;

    const float randomY =
        velocityVariance.y > 0.0f ? distY(rng) * velocityVariance.y : 0.0f;

    const float randomZ =
        velocityVariance.z > 0.0f ? distZ(rng) * velocityVariance.z : 0.0f;

    return Vector3(
        velocity.x + randomX,
        velocity.y + randomY,
        velocity.z + randomZ
    );
}

void ParticleGenerator::update(double deltaTime)
{
    if (deltaTime < 0.0) {
        return;
    }

    // Acumula el tiempo para la emisión.
    accumulatedTime += deltaTime;

    // Evita dividir entre cero o usar una tasa negativa.
    if (emissionRate > 0.0f) {
        const double particlesToEmit =
            static_cast<double>(emissionRate) * accumulatedTime;

        const int particleCount =
            static_cast<int>(particlesToEmit);

        for (int i = 0; i < particleCount; ++i) {
            Vector3 randomVel = generateRandomVelocity();

            Particle* newParticle = new Particle(
                position,
                randomVel,
                acceleration,
                damping,
                color,
                lifeTime,
                mass
            );

            particles.push_back(newParticle);
        }

        accumulatedTime -=
            static_cast<double>(particleCount) / emissionRate;
    }

    // Integra y elimina las partículas que hayan expirado
    // o salido del radio de acción.
    auto it = particles.begin();

    while (it != particles.end()) {
        Particle* particle = *it;

        Vector3 gravityForce(
            gravity.x * particle->getMass(),
            gravity.y * particle->getMass(),
            gravity.z * particle->getMass()
        );

        particle->applyForce(gravityForce);
        particle->integrate(deltaTime);

        if (!particle->isAlive()) {
            delete particle;
            it = particles.erase(it);
            continue;
        }

        const float distance =
            calculateDistance(position, particle->getPosition());

        if (distance > actionRadius) {
            delete particle;
            it = particles.erase(it);
            continue;
        }

        ++it;
    }
}

void ParticleGenerator::setPosition(Vector3 newPos)
{
    position = newPos;
}

void ParticleGenerator::setVelocity(Vector3 newVel)
{
    velocity = newVel;
}

void ParticleGenerator::setVelocityVariance(Vector3 variance)
{
    velocityVariance.x = (variance.x > 0.0f) ? variance.x : 0.0f;
    velocityVariance.y = (variance.y > 0.0f) ? variance.y : 0.0f;
    velocityVariance.z = (variance.z > 0.0f) ? variance.z : 0.0f;

    distX.param(std::normal_distribution<float>::param_type(0.0f, 1.0f));
    distY.param(std::normal_distribution<float>::param_type(0.0f, 1.0f));
    distZ.param(std::normal_distribution<float>::param_type(0.0f, 1.0f));
}

void ParticleGenerator::setGravity(Vector3 newGravity)
{
    gravity = newGravity;
}

void ParticleGenerator::setMass(float newMass)
{
    mass = newMass;
}

void ParticleGenerator::setActionRadius(float newRadius)
{
    actionRadius = newRadius;
}

std::vector<Particle*>& ParticleGenerator::getParticles()
{
    return particles;
}

Vector3 ParticleGenerator::getPosition() const
{
    return position;
}

void ParticleGenerator::clear()
{
    for (auto particle : particles) {
        delete particle;
    }

    particles.clear();
}

float ParticleGenerator::calculateDistance(
    Vector3 p1,
    Vector3 p2
) const
{
    const float dx = p2.x - p1.x;
    const float dy = p2.y - p1.y;
    const float dz = p2.z - p1.z;

    return std::sqrt(dx * dx + dy * dy + dz * dz);
}