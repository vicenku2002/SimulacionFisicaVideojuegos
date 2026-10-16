#pragma once
#include "Particle.h"
#include "ForceGenerator.h"
#include <vector>
#include <random>

class ParticleGenerator
{
public:
	ParticleGenerator(Vector3 Pos, Vector3 Vel, Vector3 Acc = Vector3(0.0f, 0.0f, 0.0f), 
		float Damp = 0.98f, Vector4 Color = Vector4(1.0f, 0.0f, 0.0f, 1.0f), 
		float EmissionRate = 10.0f, float LifeTime = 5.0f, float ActionRadius = 100.0f, float Mass = 1.0f);
	~ParticleGenerator();

	// Genera nuevas partículas y actualiza/elimina existentes cada frame
	void update(double deltaTime);

	// Actualiza la posición de la fuente
	void setPosition(Vector3 newPos);

	// Actualiza la velocidad media de emisión
	void setVelocity(Vector3 newVel);

	// Establece la desviación estándar (varianza) de la velocidad para cada componente
	// La distribución normal usará Vel como media y estos valores como desviación estándar
	void setVelocityVariance(Vector3 variance);

	// Agrega un generador de fuerzas que afectará a todas las partículas
	void addForceGenerator(ForceGenerator* forceGenerator);

	// Establece la masa de las partículas emitidas
	void setMass(float newMass);

	// Actualiza el radio de acción
	void setActionRadius(float newRadius);

	// Obtiene la lista de partículas
	std::vector<Particle*>& getParticles();

	// Obtiene la posición actual del generador
	Vector3 getPosition() const;

	// Limpia todas las partículas
	void clear();

protected:
	Vector3 position;					// Posición de la fuente
	Vector3 velocity;					// Velocidad media de emisión de partículas
	Vector3 velocityVariance;			// Desviación estándar para la distribución normal de velocidad
	Vector3 acceleration;				// Aceleración aplicada a las partículas
	float damping;						// Damping de las partículas
	Vector4 color;						// Color de las partículas
	float mass;							// Masa de las partículas emitidas
	float emissionRate;					// Partículas por segundo
	float lifeTime;						// Tiempo de vida de las partículas
	float actionRadius;					// Radio de acción máximo
	double accumulatedTime;				// Tiempo acumulado para generar partículas
	std::vector<Particle*> particles;	// Lista de partículas activas
	std::vector<ForceGenerator*> forceGenerators; // Lista de generadores de fuerzas aplicados a las partículas

	// Generador y distribuciones aleatorias
	std::mt19937 rng;
	std::normal_distribution<float> distX;
	std::normal_distribution<float> distY;
	std::normal_distribution<float> distZ;

	// Método auxiliar para calcular distancia entre dos puntos
	float calculateDistance(Vector3 p1, Vector3 p2) const;

	// Método auxiliar para generar velocidad aleatoria con distribución normal
	Vector3 generateRandomVelocity();
};
