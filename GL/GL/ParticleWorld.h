#pragma once
#include "Particle.h"
#include "ParticleForceRegistry.h"
#include "ParticleContactResolver.h"
#include "ParticleContactGenerator.h"

class ParticleWorld
{
	struct ParticleRegistration {
		Particle* particle;
		ParticleRegistration* next;
	};
	ParticleForceRegistry registry;
	ParticleContactResolver resolver;

	struct ContactGenRegistration {
		ParticleContactGenerator *gen;
		ContactGenRegistration*next;
	};
	ContactGenRegistration* firstContactGen;

	ParticleContact* contacts;
	bool calculateIterations;
	unsigned maxContacts;

public:
	ParticleRegistration* firstParticle;
	ParticleWorld(unsigned maxContacts, unsigned iterations = 0);
	void startFrame();
	unsigned generateContacts();
	void integrate(float duration);
	void runPhysics(float duration);

};

