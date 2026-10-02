#pragma once
#include "Vector3D.h"
#include "Scene.h"
#include "RenderUtils.hpp"

class Particle
{
public:
	Particle(Vector3 Pos, Vector3 Vel, Vector3 Acc = Vector3(0.0f, 0.0f, 0.0f), float Damp = 0.98f, Vector4 Color = Vector4(1.0f, 0.0f, 0.0f, 1.0f));
	~Particle();

	void integrate(double t);

protected:
	physx:: PxTransform position;
	Vector3 velocity;
	Vector3 acceleration;
	float damping;
	RenderItem* renderItem;
};

