#pragma once
#include "Particle.h"
class ParticleContact
{
	friend class ParticleContactResolver; //To access protected class
public:
	Particle *particle[2]; //Particles involved in the contact


	float restitution; //normal restitution coefficient
	glm::vec3 contactNormal; //direction of the contact
	
	float penetration;//depth of penetration

	glm::vec3 particleMovement[2];

protected:
	void resolve(float duration); //Resolves the contact for velocity and interpenetration
	glm::vec3 calculateSeparatingVelocity(); //Calculates the separating velocity of the contact


private:
	void resolveVelocity(float duration); //Impulse calculations for the collision

	void resolveInterpenetration(float duration); //Interpenetration resolution of the contact
};


class ParticleContactGenerator
{
public:
	virtual unsigned addContact(ParticleContact* contact, unsigned limit) const = 0;

};

class ParticleContactResolver //Contact resolution routine for contacts. It is a singleton
{
protected:
	unsigned iterations; //max iterations

	unsigned iterationsUsed; //record of iterations used

public:
	ParticleContactResolver(unsigned iterations); //Constructor 

	void setIterations(unsigned iterations); //sets max iterations

	void resolveContacts(ParticleContact* contactArray, unsigned numContacts, float duration); //Resolves particle contact list for both penetration and velocity
};

