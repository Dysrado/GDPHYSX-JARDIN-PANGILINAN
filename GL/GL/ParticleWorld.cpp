#include "ParticleWorld.h"

ParticleWorld::ParticleWorld(unsigned maxContacts, unsigned iterations):resolver(iterations), maxContacts(maxContacts)
{
	contacts = new ParticleContact[maxContacts];
	calculateIterations = (iterations == 0);
}

void ParticleWorld::startFrame()
{
	// Edited =============================================================
	/*ParticleRegistration* reg = firstParticle;
	while (reg) {
		reg->particle->clearAccum();
		reg = reg->next;
	}*/

	for (Particles::iterator p = particles.begin(); p != particles.end(); p++) {
		(*p)->clearAccum();
	}
	//for (int i = 0; i < particles.size(); i++) {
	//	particles[i]->clearAccum();
	//}
}

unsigned ParticleWorld::generateContacts()
{
	unsigned limit = maxContacts;
	ParticleContact* nextContact = contacts;
	for (ContactGenerators::iterator g = contactGenerators.begin(); g != contactGenerators.end(); g++) {
		unsigned used = (*g)->addContact(nextContact, limit);
		limit -= used;
		nextContact += used;
		if (limit <= 0)
			break;
	}
	return maxContacts - limit;
}

void ParticleWorld::integrate(float duration) {
	// Edited =============================================================
	/*ParticleRegistration* reg = firstParticle;
	while (reg) {
		reg->particle->integrate(duration);
		reg = reg->next;
	}*/
	for (Particles::iterator p = particles.begin(); p != particles.end(); p++) {
		(*p)->integrate(duration);
	}
	/*for (int i = 0; i < particles.size(); i++) {
		particles[i]->integrate(duration);
	}*/
}

void ParticleWorld::runPhysics(float duration) {
	registry.updateForces(duration);
	integrate(duration);
	unsigned usedContacts = generateContacts();
	if (calculateIterations) resolver.setIterations(usedContacts * 2); //IDK WHAT THIS DOES
	resolver.resolveContacts(contacts, usedContacts, duration);
}

//ContactGenRegistration ParticleWorld::getContactGenerator()
//{
//	return firstContactGen;
//}

void ParticleWorld::push_back(Particle* particle)
{
	particles.push_back(particle);
}


void ParticleWorld::render(GLuint shaderProgram)
{
	// Edited =============================================================
	/*ParticleRegistration* temp = firstParticle;
	if (temp != NULL) {
		temp->particle->render(shaderProgram);
	}*/
	for (Particles::iterator p = particles.begin(); p != particles.end(); p++) {
		(*p)->render(shaderProgram);
	}
	/*for (int i = 0; i < particles.size(); i++) {
		particles[i]->render(shaderProgram);
	}*/

}
