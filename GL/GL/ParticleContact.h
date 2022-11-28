#pragma once
#include "Particle.h"
class ParticleContact
{
	friend class ParticleContactResolver;
public:
	Particle *particle[2];

	float restitution;
	glm::vec3 contactNormal;
	float penetration;

protected:
	void resolve(float duration);
	glm::vec3 calculateSeparatingVelocity();


private:
	void resolveVelocity(float duration);

	void resolveInterpenetration(float duration);
};


class ParticleContactGenerator
{
public:
	virtual unsigned addContact(ParticleContact* contact, unsigned limit) const = 0;

};

class ParticleContactResolver
{
protected:
	unsigned iterations;

	unsigned iterationsUsed;

public:
	ParticleContactResolver(unsigned iterations);

	void setIterations(unsigned iterations);

	void resolveContacts(ParticleContact* contactArray, unsigned numContacts, float duration);
};

