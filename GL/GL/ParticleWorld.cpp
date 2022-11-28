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

	for (int i = 0; i < firstParticle.size(); i++) {
		firstParticle[i]->particle->clearAccum();
	}
}

unsigned ParticleWorld::generateContacts()
{
	unsigned limit = maxContacts;
	ParticleContact* nextContact = contacts;
	ContactGenRegistration* reg = firstContactGen;
	while (reg) {
		unsigned used = reg->gen->addContact(nextContact, limit);
		limit -= used;
		nextContact += used;
		if (limit <= 0)
			break;
		reg = reg->next;
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
	for (int i = 0; i < firstParticle.size(); i++) {
		firstParticle[i]->particle->integrate(duration);
	}
}

void ParticleWorld::runPhysics(float duration) {
	registry.updateForces(duration);
	integrate(duration);
	unsigned usedContacts = generateContacts();
	if (calculateIterations) resolver.setIterations(usedContacts * 2); //IDK WHAT THIS DOES
	resolver.resolveContacts(contacts, usedContacts, duration);
}

ContactGenRegistration ParticleWorld::getContactGenerator()
{
	return firstContactGen;
}

void ParticleWorld::push_back(Particle* particle)
{
	// Edited =============================================================
	/*ParticleRegistration* temp = new ParticleRegistration();
	temp->particle = particle;
	temp->next = NULL;

	if (firstParticle == NULL) {
		firstParticle = temp;
	}
	else {
		ParticleRegistration* traverse = firstParticle;
		while (traverse->next != NULL) {
			traverse = traverse->next;
		}
		traverse->next = temp;

	}*/
	ParticleRegistration* temp = new ParticleRegistration();
	temp->particle = particle;

	firstParticle.push_back(temp);
}

void ParticleWorld::push_back_contact(ParticleContactGenerator* contactGen)
{
}

void ParticleWorld::render(GLuint shaderProgram)
{
	// Edited =============================================================
	/*ParticleRegistration* temp = firstParticle;
	if (temp != NULL) {
		temp->particle->render(shaderProgram);
	}*/
	
	for (int i = 0; i < firstParticle.size(); i++) {
		firstParticle[i]->particle->render(shaderProgram);
	}

}
