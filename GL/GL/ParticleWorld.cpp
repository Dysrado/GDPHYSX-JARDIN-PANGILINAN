#include "ParticleWorld.h"
/*Returns the distance from one end of Particle A to Particle B*/
float ParticleWorld::checkContacts(Particle* a, Particle* b)
{
	return pow((a->getPosition().x + b->getPosition().x), 2) + pow((a->getPosition().y + b->getPosition().y), 2) + pow((a->getPosition().z + b->getPosition().z), 2);
}

/*Runs through a list to check if any of the two particles are colliding*/
void ParticleWorld::checkCollision()
{
	for (int i = 0; i < contactList.size(); i++) {
		float intersect = checkContacts(contactList[i]->particle[0], contactList[i]->particle[1]);
		if (intersect < 10) {
			std::cout << "Collided";
		}
	}
	
}

/*Constructor for Particle World*/
ParticleWorld::ParticleWorld(unsigned maxContacts, unsigned iterations):resolver(iterations), maxContacts(maxContacts)
{
	contacts = new ParticleContact[maxContacts];
	calculateIterations = (iterations == 0);
}

/*Runs at the start of the game loop and clears force accumulators*/
void ParticleWorld::startFrame()
{
	for (Particles::iterator p = particles.begin(); p != particles.end(); p++) {
		(*p)->clearAccum();
	}
	
}
/*Returns the contactGenerators list*/
ParticleWorld::ContactGenerators& ParticleWorld::getContactGenerator()
{
	return contactGenerators;
}


/*Generates the contacts based from the contactGenerator List and checks if there are any contacts betweent the particle*/
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

/*Runs the movement for each particle*/
void ParticleWorld::integrate(float duration) {
	
	for (Particles::iterator p = particles.begin(); p != particles.end(); p++) {
		(*p)->integrate(duration);
	}
	
}

/*Deploys the physics simulation for the particle world*/
void ParticleWorld::runPhysics(float duration) {
	registry.updateForces(duration);
	integrate(duration);
	checkCollision();
	unsigned usedContacts = generateContacts();
	if (usedContacts) {
		if (calculateIterations) resolver.setIterations(usedContacts * 2); 
		resolver.resolveContacts(contacts, usedContacts, duration);
	}
}


/*Renders each particle onto the scene*/
void ParticleWorld::render(GLuint shaderProgram)
{

	for (Particles::iterator p = particles.begin(); p != particles.end(); p++) {
		(*p)->render(shaderProgram);
	}
	

}
