#include "ParticleForceRegistry.h"

// Assigns the force generator to the particle
void ParticleForceRegistry::add(Particle* particle, ParticleForceGenerator* fg)
{
	ParticleForceRegistration temp;
	temp.particle = particle;
	temp.fg = fg;
	registrations.push_back(temp);
}

void ParticleForceRegistry::remove(Particle* particle, ParticleForceGenerator* fg)
{
}

// Removes all of the force generations in force registrations
void ParticleForceRegistry::clear()
{
	registrations.clear();
}

// Updates the forces of all particles in the registration
void ParticleForceRegistry::updateForces(float duration)
{
	for (int i = 0; i < registrations.size(); i++) {
		registrations[i].fg->updateForce(registrations[i].particle, duration);
	}
}
