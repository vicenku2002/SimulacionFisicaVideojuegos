#pragma once
#include "Vector3D.h"
#include "Scene.h"
#include "RenderUtils.hpp"

class Particle
{
public:
	Particle(Vector3 Pos, Vector3 Vel, Vector3 Acc = Vector3(0.0f, 0.0f, 0.0f), float Damp = 0.98f, Vector4 Color = Vector4(1.0f, 0.0f, 0.0f, 1.0f), float LifeTime = 5.0f, float Mass = 1.0f);
	~Particle();

	void integrate(double t);

	// Obtiene la edad actual de la partícula
	double getAge() const;

	// Obtiene el tiempo de vida máximo
	float getLifeTime() const;

	// Verifica si la partícula ha expirado
	bool isAlive() const;

	// Obtiene la posición actual
	Vector3 getPosition() const;

	// Obtiene la masa de la partícula
	float getMass() const;

	// Aplica una fuerza a la partícula (F = m*a, a = F/m)
	void applyForce(Vector3 force);

	// Limpia las fuerzas acumuladas
	void clearForces();

protected:
	physx:: PxTransform position;
	Vector3 velocity;
	Vector3 acceleration;
	Vector3 accumulatedForces;		// Fuerzas acumuladas
	float mass;						// Masa de la partícula
	float damping;
	float lifeTime;		// Tiempo de vida máximo en segundos
	double age;			// Edad actual de la partícula
	RenderItem* renderItem;
};

