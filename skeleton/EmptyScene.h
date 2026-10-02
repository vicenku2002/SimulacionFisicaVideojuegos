#pragma once

#include "Scene.h"
#include "RenderUtils.hpp"
#include <vector>
#include "Vector3D.h"
#include "Projectile.h"
#include <iostream>

using namespace std;

// Estructura para configurar un tipo de proyectil
struct ProjectileType {
	float velocity;
	float damping;
	float radius;
	Vector4 color;
};

class EmptyScene : public Scene {
public:
	explicit EmptyScene(std::string name) : Scene(std::move(name)) {}

	void init() override {
		//Creación de una esfera usando las utilidades de render existentes
		m_transform = physx::PxTransform(physx::PxVec3(0.0f, 10.0f, 0.0f));

		// Definir tipos de proyectiles con diferentes características
		// Tecla 'Q' - Rojo - velocidad baja
		m_projectileTypes.push_back({ 50.0f,  0.98f, 0.5f, Vector4(1.0f, 0.0f, 0.0f, 1.0f) });
		// Tecla 'F' - Verde - velocidad media
		m_projectileTypes.push_back({ 100.0f, 0.98f, 0.7f, Vector4(0.0f, 1.0f, 0.0f, 1.0f) });
		// Tecla 'E' - Azul - velocidad alta
		m_projectileTypes.push_back({ 150.0f, 0.98f, 0.3f, Vector4(0.0f, 0.0f, 1.0f, 1.0f) });

		retoC();
	}

	void update(double dt) override {

		// Integrar todos los proyectiles activos
		for (int i = 0; i < m_projectiles.size(); ++i) {
			if (m_projectiles[i]) {
				m_projectiles[i]->integrate(dt);
			}
		}
	}

	void keyPress(unsigned char key, const physx::PxTransform& camera) override {
		// Reinicio de escena
		if (key == 'r' || key == 'R') {
			m_transform.p = physx::PxVec3(0.0f, 10.0f, 0.0f); // Reset
			// Limpiar todos los proyectiles
			for (Projectile* p : m_projectiles) {
				if (p) delete p;
			}
			m_projectiles.clear();
			return;
		}

		// Disparar proyectil: teclas Q, F, E
		int typeIndex = -1;
		if (key == 'q' || key == 'Q') {
			typeIndex = 0;
		} else if (key == 'f' || key == 'F') {
			typeIndex = 1;
		} else if (key == 'e' || key == 'E') {
			typeIndex = 2;
		}

		if (typeIndex >= 0 && typeIndex < m_projectileTypes.size()) {
			fireProjectile(typeIndex);
			return;
		}
	}

	void cleanup() override {
		// Liberar todos los proyectiles
		for (Projectile* p : m_projectiles) {
			if (p) {
				delete p;
			}
		}
		m_projectiles.clear();

		for(RenderItem* item : m_renderItems)
		if (item) {
			item->release(); // Deregistra y destruye el item
			item = nullptr;
		}
	}

private:
	vector<physx::PxTransform> transforms;
	physx::PxTransform m_transform;
	vector<RenderItem*> m_renderItems;
	vector<Projectile*> m_projectiles; // Vector de proyectiles activos
	vector<ProjectileType> m_projectileTypes; // Configuración de tipos de proyectiles

	// Dispara un proyectil del tipo especificado
	void fireProjectile(int typeIndex) {
		if (typeIndex < 0 || typeIndex >= m_projectileTypes.size()) {
			return;
		}

		ProjectileType pType = m_projectileTypes[typeIndex];

		// Obtener la cámara
		Camera* pCamera = GetCamera();
		if (!pCamera) return;

		// Obtener posición y dirección de la cámara
		physx::PxVec3 camPos = pCamera->getEye();
		physx::PxVec3 camDir = pCamera->getDir();

		// Crear nuevo proyectil con configuración específica
		Vector3 initialPos(camPos.x, camPos.y, camPos.z);
		Vector3 initialVel = Vector3(camDir.x, camDir.y, camDir.z) * pType.velocity;
		Vector3 acceleration(0.0f, -9.8f, 0.0f); // Gravedad estándar

		Projectile* newProjectile = new Projectile(initialPos, initialVel, acceleration, pType.damping, 1.0f, pType.color);

		m_projectiles.push_back(newProjectile);
	}

	void retoB()
	{
		Vector3D D(0.0f, 0.0f, 1.0f); // Direccion de vision del enemigo

		// Esfera
		physx::PxShape* sphere = CreateShape(physx::PxSphereGeometry(1.0f));

		// Transformaciones
		transforms.push_back(physx::PxTransform(physx::PxVec3(2.0f, 0.0f, 3.0f)));
		transforms.push_back(physx::PxTransform(physx::PxVec3(-4.0f, 0.0f, 1.0f)));
		transforms.push_back(physx::PxTransform(physx::PxVec3(0.0f, 0.0f, 5.0f)));
		transforms.push_back(physx::PxTransform(physx::PxVec3(3.0f, 0.0f, 0.0f)));

		// Esfera del enemigo
		m_renderItems.push_back(new RenderItem(sphere, &physx::PxTransform(0.0f, 0.0f, 0.0f), Vector4(1.0f, 1.0f, 1.0f, 1.0f))); // Enemigo

		// Esferas de colores
		for (int i = transforms.size() - 4; i < transforms.size(); ++i)
		{
			// Producto escalar de la direccion de vista del enemigo con el objeto en el espacio
			float dotProd = D.dot(Vector3D(physx::PxVec3(transforms[i].p.x, transforms[i].p.y, transforms[i].p.z)));

			// Color dependiende de la posicion del objeto con respecto al enemigo (valor del producto escalar)
			Vector4 color;

			if (dotProd > 0) color = Vector4(0.0f, 1.0f, 0.0f, 1.0f);
			else if (dotProd < 0) color = Vector4(1.0f, 0.0f, 0.0f, 1.0f);
			else color = Vector4(1.0f, 1.0f, 0.0f, 1.0f);

			// RenderItem 
			m_renderItems.push_back(new RenderItem(sphere, &transforms[i], color));
		}
	}

	void retoC()
	{
		Vector3D A(-8.0f, 1.0f, -8.0f); // Punto A
		Vector3D B(8.0f, 8.0f, 8.0f); // Punto B

		// Esfera
		physx::PxShape* sphere = CreateShape(physx::PxSphereGeometry(2.0f));


		// Esferas de la trayectoria AB
		for (int i = 1; i <= 10; ++i)
		{
			// Producto escalar de la direccion de vista del enemigo con el objeto en el espacio
			Vector3D pos = A + ((B - A) * ( i / 10.0f));

			// Transformacion para colocar la esfera
			physx::PxTransform* currTrans = new physx::PxTransform(pos);
			transforms.push_back(*currTrans);

			// RenderItem 
			m_renderItems.push_back(new RenderItem(sphere, currTrans, Vector4(1.0f, 0.0f, 1.0f, 1.0f)));
		}
	}
};
