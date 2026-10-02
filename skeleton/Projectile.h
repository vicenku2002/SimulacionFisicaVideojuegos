#pragma once
#include "Particle.h"

class Projectile : public Particle
{
public:
	Projectile(Vector3 Pos, Vector3 Vel, Vector3 Acc = Vector3(0.0f, 0.0f, 0.0f), float Damp = 0.98f, float Mass = 1.0f, Vector4 Color = Vector4(1.0f, 0.0f, 0.0f, 1.0f));
	~Projectile();

	// Setters/getters para controlar parámetros físicos del proyectil
	void setMass(float m);
	float getMass() const;
	void setAcceleration(const Vector3& a);
	void setPosition(const Vector3& p);
	void setVelocity(const Vector3& v);
	Vector3 getVelocity() const;
	Vector3 getPosition() const;

private:
	float mass; // masa simulada del proyectil
};
