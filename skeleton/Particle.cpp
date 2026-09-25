#include "Particle.h"
#include <cmath>

Particle::Particle(Vector3 Pos, Vector3 Vel, Vector3 Acc, float Damp) {
	position = physx::PxTransform(Pos.x, Pos.y, Pos.z);
	velocity = Vel;
	acceleration = Acc;
	damping = Damp;
	// Crear un RenderItem para la partícula (por ejemplo, una esfera)
	physx::PxShape* shape = CreateShape(physx::PxSphereGeometry(1.0f)); // Radio de 1.0 unidades
	renderItem = new RenderItem(shape, &position, Vector4(1.0f, 0.0f, 0.0f, 1.0f)); // Color rojo
}

Particle::~Particle() {
	renderItem->release(); // Liberar el RenderItem
}

void Particle::integrate(double t) {

	// Integración explícita semimplícita por Euler con aceleración y damping:
	// v(t+dt) = (v(t) + a*dt) * damping^{dt}
	// p(t+dt) = p(t) + v(t+dt) * dt
	float dt = static_cast<float>(t);

	// Actualiza la velocidad con la aceleración
	velocity.x += acceleration.x * dt;
	velocity.y += acceleration.y * dt;
	velocity.z += acceleration.z * dt;

	// Aplica el damping exponencial (damping por segundo -> damping ^ dt)
	float dampFactor = std::pow(damping, dt);
	velocity.x *= dampFactor;
	velocity.y *= dampFactor;
	velocity.z *= dampFactor;

	// Actualiza la posición con la nueva velocidad
	position.p.x += velocity.x * dt;
	position.p.y += velocity.y * dt;
	position.p.z += velocity.z * dt;
}