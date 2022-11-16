#pragma once
#include "Particle.h"
class ParticleContact
{
public:
	Particle *particle[2];

	float restitution;

	glm::vec3 contactNormal;

	void resolve(float duration);
	glm::vec3 calculateSeparatingVelocity() const;


private:
	void resolveVelocity(float duration);

	float penetration;
	void resolveInterpenetration(float duration);
};

