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
	};

	typedef std::vector<ParticleForceRegistration> Registry;
	Registry registrations;

public:
	void add(Particle *particle, ParticleForceGenerator *fg);
	void remove(Particle *particle, ParticleForceGenerator *fg);

	void clear();

	void updateForces(float duration); 
};

