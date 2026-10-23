#pragma once

#include "Particle.h"

class ForceGenerator
{
public:

    virtual ~ForceGenerator() = default;

    // Aplica la fuerza a una partícula (se llama una vez por partícula)
    virtual void updateForce(Particle* particle, double dt) = 0;

    // Actualiza el estado interno del generador (se llama una vez por frame).
    // Útil para fuerzas que dependen del tiempo, como las explosiones.
    virtual void update(double dt) {}
};