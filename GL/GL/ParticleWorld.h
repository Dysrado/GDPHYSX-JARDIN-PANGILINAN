#pragma once
#include <vector>
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
	std::vector<ParticleRegistration*> firstParticle;
	// Edited =============================================================
	//ParticleRegistration* firstParticle;
	ParticleForceRegistry registry;
	ParticleWorld(unsigned maxContacts, unsigned iterations = 0);
	void startFrame();
	unsigned generateContacts();
	void integrate(float duration);
	void runPhysics(float duration);
	ContactGenRegistration* getContactGenerator();
	void push_back(Particle *particle);
	void push_back_contact(ParticleContactGenerator *contactGen);
	void render(GLuint shaderProgram);

};

