#include "Projectile.h"

Projectile::Projectile(Vector3 Pos, Vector3 Vel, Vector3 Acc, float Damp, float Mass, Vector4 Color)
	: Particle(Pos, Vel, Acc, Damp, Color, 5.0f, Mass) {
}
Projectile::~Projectile() {
	// El destructor de Particle se encarga de liberar renderItem
}

void Projectile::setMass(float m) {
	mass = m;
}

float Projectile::getMass() const {
	return mass;
}

void Projectile::setAcceleration(const Vector3& a) {
	acceleration = a;
}

void Projectile::setPosition(const Vector3& p) {
	position = physx::PxTransform(p.x, p.y, p.z);
}

void Projectile::setVelocity(const Vector3& v) {
	velocity = v;
}
