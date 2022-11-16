#pragma once
#include "Particle.h"
class ParticleContact
{
public:
	Particle *particle[2];

	float restitution;
	glm::vec3 contactNormal;
	float penetration;

	void resolve(float duration);
	glm::vec3 calculateSeparatingVelocity() const;


private:
	void resolveVelocity(float duration);

	void resolveInterpenetration(float duration);
};

