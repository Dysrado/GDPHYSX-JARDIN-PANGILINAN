#pragma once
#include <vector>
#include "Particle.h"
#include "ParticleForceRegistry.h"
#include "ParticleContactResolver.h"
#include "ParticleContactGenerator.h"

class ParticleWorld
{
	ParticleContactResolver resolver;

	typedef std::vector<ParticleContactGenerator*> ContactGenerators;

	ContactGenerators contactGenerators;

	ParticleContact* contacts;
	bool calculateIterations;
	unsigned maxContacts;

public:
	typedef std::vector<Particle*> Particles;
	Particles particles;
	ParticleForceRegistry registry;
	ParticleWorld(unsigned maxContacts, unsigned iterations = 0);
	void startFrame();
	unsigned generateContacts();
	void integrate(float duration);
	void runPhysics(float duration);
	//ContactGenRegistration* getContactGenerator();
	void push_back(Particle *particle);
	void render(GLuint shaderProgram);

};

