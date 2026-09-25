#pragma once

#include <PxPhysicsAPI.h>

class Vector3D
{
public:
	float x, y, z;

	// Constructor por defecto
	Vector3D() : x(0), y(0), z(0) {}

	// Constructor por parametros
	Vector3D(float X, float Y, float Z) : x(X), y(Y), z(Z) {}

	// Constructor por conversion
	Vector3D(physx::PxVec3 vector) : x(vector.x), y(vector.y), z(vector.z) {}

	// Devuelve la magnitud del vector
	float magnitude() const { return sqrt(pow(x, 2) + pow(y, 2) + pow(z, 2)); }

	// Devuelve el vector normalizado
	Vector3D normalize() const 
	{
		const float currMagnitude = magnitude();

		return Vector3D(x / currMagnitude, y / currMagnitude, z / currMagnitude);
	}

	// Producto escalar con otro vector
	float dot(const Vector3D& otherV) const { return ((x * otherV.x) + (y * otherV.y) + (z * otherV.z)); }

	// Producto vectorial con otro vector
	Vector3D cross(const Vector3D& otherV) const { return Vector3D(y * otherV.z - z * otherV.y, x * otherV.z - z * otherV.x, x * otherV.y - y * otherV.x); }

	// Operador de asignacion
	void operator=(const Vector3D& otherV)  
	{ 
		x = otherV.x;
		y = otherV.y;
		z = otherV.z;
	}

	// Operador de suma
	Vector3D operator+(const Vector3D& otherV) const { return Vector3D(x + otherV.x, y + otherV.y, z * otherV.z); }

	// Operador de resta
	Vector3D operator-(const Vector3D& otherV) const { return Vector3D(x - otherV.x, y - otherV.y, z - otherV.z); }

	// Operador de multiplicacion por un escalar
	Vector3D operator*(const float scale) const { return Vector3D(x * scale, y * scale, z * scale); }

	// Operador de suma y guardado
	void operator+=(const Vector3D& otherV)  
	{ 
		x += otherV.x;
		y += otherV.y;
		z += otherV.z;
	}

	// Conversion del vector a vector de physx
	operator physx::PxVec3() const { return physx::PxVec3(x, y, z); }
};