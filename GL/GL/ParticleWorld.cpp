#include "ParticleWorld.h"

ParticleWorld::ParticleWorld(unsigned maxContacts, unsigned iterations)
{
}

void ParticleWorld::startFrame()
{
	particleRegistration* reg = firstParticle;
	while (reg) {
		reg->particle->clearAccum();
		reg = reg->next;
	}
}

unsigned ParticleWorld::generateContacts()
{
	unsigned limit = maxContacts;
	ParticleContact* nextContact = contacts;
	ContactGenRegistration* reg = firstContactGen;
	while (reg) {
		//unsigned used = reg->gen
		limit -= used;
		nextContact += used;
		if (limit <= 0)
			break;
		reg = reg->next;
	}
	return maxContacts - limit;
}

void ParticleWorld::integrate(float duration) {
	ParticleRegistration* reg = firstParticle;
	while (reg) {
		reg->particle->integrate(duration);
		reg = reg->next;
	}
}

void ParticleWorld::runPhysics(float duration) {
	registry.updateForces(duration);
	integrate(duration);
	unsigned usedContacts = generateContacts();
	if (calculateIterations) resolver.setIterations(usedContacts * 2); //IDK WHAT THIS DOES
	resolver.resolveContacts(contacts, usedContacts, duration);
}
