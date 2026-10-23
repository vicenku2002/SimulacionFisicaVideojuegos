#pragma once

#include "ForceGenerator.h"

// Generador de fuerzas de viento dentro de un volumen con forma de caja.
// F = k1 * (vViento - v) + k2 * |vViento - v| * (vViento - v)
class WindForce : public ForceGenerator
{
public:
    // windVelocity: velocidad del viento
    // center / halfSize: centro y semitamaño de la caja de influencia
    // k1: coeficiente de rozamiento lineal, k2: coeficiente turbulento
    WindForce(Vector3 windVelocity, Vector3 center, Vector3 halfSize, float k1 = 1.0f, float k2 = 0.0f);

    void updateForce(Particle* particle, double dt) override;

    void setWindVelocity(Vector3 v) { windVelocity = v; }
    void setK1(float k) { k1 = k; }
    void setK2(float k) { k2 = k; }
    float getK2() const { return k2; }
    void setActive(bool a) { active = a; }
    bool isActive() const { return active; }

protected:
    // Velocidad del viento en una posición (las clases hijas, como el torbellino, la redefinen)
    virtual Vector3 getWindVelocity(const Vector3& pos) const;

    // Indica si una posición está dentro del volumen de influencia
    virtual bool isInside(const Vector3& pos) const;

    Vector3 windVelocity;
    Vector3 center;
    Vector3 halfSize;
    float k1;
    float k2;
    bool active;
};
