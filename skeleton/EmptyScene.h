#pragma once

#include "Scene.h"
#include "RenderUtils.hpp"
#include <vector>
#include "Vector3D.h"

using namespace std;

class EmptyScene : public Scene {
public:
    explicit EmptyScene(std::string name) : Scene(std::move(name)) {}

    void init() override {
        // Ejemplo: Creación de una esfera usando las utilidades de render existentes
        //physx::PxShape* shape = CreateShape(physx::PxSphereGeometry(2.0f));
        m_transform = physx::PxTransform(physx::PxVec3(0.0f, 10.0f, 0.0f));

        retoC();
    }

    void update(double dt) override {
        // Lógica/Integración del alumno (por ejemplo, movimiento simple)
        //m_transform.p.y -= static_cast<float>(9.8 * dt);
    }

    void keyPress(unsigned char key, const physx::PxTransform& camera) override {
        if (key == 'r' || key == 'R') {
            m_transform.p = physx::PxVec3(0.0f, 10.0f, 0.0f); // Reset
        }
    }

    void cleanup() override {
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