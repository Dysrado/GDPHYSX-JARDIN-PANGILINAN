#pragma once
#include "Particle.h"
#include "ParticleForceGenerator.h"
#include <vector>

class ParticleForceRegistry
{
protected:
	struct ParticleForceRegistration {
		Particle* particle;
		ParticleForceGenerator* fg;
	}; // A structure that contains the particle and the associated force generator

	typedef std::vector<ParticleForceRegistration> Registry;
	Registry registrations; // List of Registrations

public:
	void add(Particle *particle, ParticleForceGenerator *fg); // adds the particle and force generator to the list
	void remove(Particle *particle, ParticleForceGenerator *fg);

	void clear(); // removes all of the data from registrations

	void updateForces(float duration); // updates all of the particles with associated forces
};

