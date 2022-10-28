#include "ParticleForceRegistry.h"

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

void ParticleForceRegistry::clear()
{
	registrations.clear();
}

void ParticleForceRegistry::updateForces(float duration)
{
	for (int i = 0; i < registrations.size(); i++) {
		registrations[i].fg->updateForce(registrations[i].particle, duration);
	}
}
