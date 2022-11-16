#pragma once
#include "ParticleContact.h"
class ParticleContactResolver
{
protected:
	unsigned iterations;

	unsigned iterationsUsed;

public:
	ParticleContactResolver(unsigned iterations);

	void setIterations(unsigned iterations);

	void resolveContacts(ParticleContact * contactArray, unsigned numContacts, float duration);
};

