#include "Particle.h"
#include <cmath>

Particle::Particle(Vector3 Pos, Vector3 Vel, Vector3 Acc, float Damp, Vector4 Color, float LifeTime, float Mass) {
	position = physx::PxTransform(Pos.x, Pos.y, Pos.z);
	velocity = Vel;
	acceleration = Acc;
	damping = Damp;
	lifeTime = LifeTime;
	mass = Mass;
	age = 0.0;
	accumulatedForces = Vector3(0.0f, 0.0f, 0.0f);
	// Crear un RenderItem para la partícula (por ejemplo, una esfera)
	physx::PxShape* shape = CreateShape(physx::PxSphereGeometry(1.0f)); // Radio de 1.0 unidades
	renderItem = new RenderItem(shape, &position, Color); // Color personalizable
}

Particle::~Particle() {
	if (renderItem) renderItem->release(); // Liberar el RenderItem
}

void Particle::integrate(double t) {

	// Integración explícita semiimplícita por Euler con aceleración, fuerzas y damping:
	// a_total = acceleration + (forces / mass)
	// v(t+dt) = (v(t) + a_total*dt) * damping^{dt}
	// p(t+dt) = p(t) + v(t+dt) * dt
	float dt = static_cast<float>(t);

	// Calcula la aceleración total (aceleración base + fuerzas/masa)
	Vector3 totalAcceleration(
		acceleration.x + accumulatedForces.x / mass,
		acceleration.y + accumulatedForces.y / mass,
		acceleration.z + accumulatedForces.z / mass
	);

	// Actualiza la velocidad con la aceleración total
	velocity.x += totalAcceleration.x * dt;
	velocity.y += totalAcceleration.y * dt;
	velocity.z += totalAcceleration.z * dt;

	// Aplica el damping exponencial (damping por segundo -> damping ^ dt)
	float dampFactor = std::pow(damping, dt);
	velocity.x *= dampFactor;
	velocity.y *= dampFactor;
	velocity.z *= dampFactor;

	// Actualiza la posición con la nueva velocidad
	position.p.x += velocity.x * dt;
	position.p.y += velocity.y * dt;
	position.p.z += velocity.z * dt;

	// Incrementa la edad de la partícula
	age += t;

	// Limpia las fuerzas acumuladas para el siguiente frame
	accumulatedForces = Vector3(0.0f, 0.0f, 0.0f);
}

double Particle::getAge() const {
	return age;
}

float Particle::getLifeTime() const {
	return lifeTime;
}

bool Particle::isAlive() const {
	return age < lifeTime;
}

Vector3 Particle::getPosition() const {
	return Vector3(position.p.x, position.p.y, position.p.z);
}

float Particle::getMass() const {
	return mass;
}

void Particle::applyForce(Vector3 force) {
	accumulatedForces.x += force.x;
	accumulatedForces.y += force.y;
	accumulatedForces.z += force.z;
}

void Particle::clearForces() {
	accumulatedForces = Vector3(0.0f, 0.0f, 0.0f);
}
