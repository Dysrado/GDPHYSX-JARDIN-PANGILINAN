#include "ParticleWorld.h"

float ParticleWorld::checkContacts(Particle* a, Particle* b)
{
	return pow((a->getPosition().x + b->getPosition().x), 2) + pow((a->getPosition().y + b->getPosition().y), 2) + pow((a->getPosition().z + b->getPosition().z), 2);
}

void ParticleWorld::checkCollision()
{
	//std::cout << "Running..\n";
	//std::cout << "contactSize: " << contactList.size() << std::endl;
	for (int i = 0; i < contactList.size(); i++) {
		float intersect = checkContacts(contactList[i]->particle[0], contactList[i]->particle[1]);
		std::cout << "Particle Location " << contactList[i]->particle[0]->getPosition().x << std::endl;;
		if (intersect < 10) {
			std::cout << "Collided";
		}
	}
	
}

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

ParticleWorld::ContactGenerators& ParticleWorld::getContactGenerator()
{
	return contactGenerators;
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
	checkCollision();
	unsigned usedContacts = generateContacts();
	if (usedContacts) {
		if (calculateIterations) resolver.setIterations(usedContacts * 2); 
		resolver.resolveContacts(contacts, usedContacts, duration);
	}
	std::cout << "RESET" << std::endl;

	
}


//void ParticleWorld::push_back(Particle* particle)
//{
//	particles.push_back(particle);
//}


void ParticleWorld::render(GLuint shaderProgram)
{
	// Edited =============================================================
	/*ParticleRegistration* temp = firstParticle;
	if (temp != NULL) {
		temp->particle->render(shaderProgram);
	}*/
	for (Particles::iterator p = particles.begin(); p != particles.end(); p++) {
		//std::cout << "Particle Array B " << (*p) << " " << (*p)->getPosition().x << " " << (*p)->getPosition().y << " " << (*p)->getPosition().z << std::endl;
		(*p)->render(shaderProgram);
	}
	/*for (int i = 0; i < particles.size(); i++) {
		particles[i]->render(shaderProgram);
	}*/

}
