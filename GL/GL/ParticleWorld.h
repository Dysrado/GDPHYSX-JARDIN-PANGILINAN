#pragma once
#include <vector>
#include "Particle.h"
#include "ParticleForceRegistry.h"

#include "ParticleContact.h"

class ParticleWorld
{
	ParticleContactResolver resolver;
	ParticleContact* contacts;
	bool calculateIterations;
	unsigned maxContacts;

public:
typedef std::vector<ParticleContactGenerator*> ContactGenerators; 
	std::vector<ParticleContact*> contactList;
	float checkContacts(Particle* a, Particle* b);
	void checkCollision(float duration);
	typedef std::vector<Particle*> Particles;
	ContactGenerators contactGenerators;
	Particles particles;
	
	

	ParticleForceRegistry registry;
	ParticleWorld(unsigned maxContacts, unsigned iterations = 0);
	void startFrame();
	ContactGenerators& getContactGenerator();
	unsigned generateContacts();
	void integrate(float duration);
	void runPhysics(float duration);

	void render(GLuint shaderProgram);

};

