#include "ParticleContact.h"
/*Resolves the contact for velocity and interpenetration*/
void ParticleContact::resolve(float duration)
{
	resolveVelocity(duration);
	resolveInterpenetration(duration);
	
	
}
/*Calculates the separating velocity of the contact*/
glm::vec3 ParticleContact::calculateSeparatingVelocity()
{
	glm::vec3 relativeVelocity = particle[0]->getVelocity();
	if (particle[1]) {
		relativeVelocity -= particle[1]->getVelocity();
		
	}
	return relativeVelocity * contactNormal;
}
/*Impulse calculations for the collision*/
void ParticleContact::resolveVelocity(float duration)
{

	//velocity in the direction of the contact
	glm::vec3 separatingVelocity = calculateSeparatingVelocity();
	
	//Check if needs to be resolved
	if (separatingVelocity.x > 0 && separatingVelocity.y > 0 && separatingVelocity.z > 0) {
		return;
	}

	//Calculate new Separating Velocity
	glm::vec3 newSepVelocity = -separatingVelocity * restitution;

	//Check velocity buildup from acceleration
	glm::vec3 accCausedVelocity = particle[0]->getAcceleration();
	if (particle[1]) {
		accCausedVelocity -= particle[1]->getAcceleration();
	}

	glm::vec3 accCausedSepVelocity = accCausedVelocity * contactNormal * duration;

	// Remove closing velocity due to acceleration from new separating velocity
	if (accCausedSepVelocity.x < 0 && accCausedSepVelocity.y < 0 && accCausedSepVelocity.z < 0) {
		newSepVelocity += restitution * accCausedSepVelocity;
		if (newSepVelocity.x < 0 && newSepVelocity.y < 0 && newSepVelocity.z < 0) {
			newSepVelocity.x = 0;
			newSepVelocity.y = 0;
			newSepVelocity.z = 0;
		}
	}
	glm::vec3 deltaVelocity = newSepVelocity - separatingVelocity;

	//Apply change in velocity in proportion to inverse mass
	float totalInverseMass = particle[0]->getMass();
	if (particle[1]) {
		totalInverseMass += particle[1]->getMass();
	}
	//if infinite mass return
	if (totalInverseMass <= 0) {
		
		return;
	}
	//Impulse Calculation
	glm::vec3 impulse = deltaVelocity / totalInverseMass;
	//Calculation of Impulse per Unit
	glm::vec3 impulsePerIMass = contactNormal * impulse;

	//Apply impulses
	particle[0]->setVelocity(particle[0]->getVelocity() + impulsePerIMass * particle[0]->getMass());

	if (particle[1]) {
		particle[1]->setVelocity(particle[1]->getVelocity() + impulsePerIMass * -particle[1]->getMass());
	}



}
/*Interpenetration resolution of the contact*/
void ParticleContact::resolveInterpenetration(float duration)
{
	//No penetration no need for resolution
	if (penetration <= 0) {
		return;
	}

	//Compute total inverse mass
	float totalInverseMass = particle[0]->getMass();

	if (particle[1]) {
		totalInverseMass += particle[1]->getMass();
	}
	//Exit if infinite mass
	if (totalInverseMass <= 0) {
		return;
	}

	//Calculate penetration resolution per unit
	glm::vec3 movePerIMass = contactNormal * (-penetration / totalInverseMass);

	//Calculate movement amounts
	particleMovement[0] = movePerIMass * particle[0]->getMass();
	if (particle[1]) {
		particleMovement[1] = movePerIMass * -particle[1]->getMass();

	}
	else {
		particleMovement[1] = glm::vec3(0,0,0);
	}

	//Apply the penetration resolution
	particle[0]->setPosition(particle[0]->getPosition() +  particleMovement[0]);

	if (particle[1]) {
		particle[1]->setPosition(particle[1]->getPosition() + particleMovement[1]);
	}
}

/*Resolves particle contact list for both penetration and velocity*/
void ParticleContactResolver::resolveContacts(ParticleContact* contactArray, unsigned numContacts, float duration)
{
	unsigned i;
	iterationsUsed = 0;
	while (iterationsUsed < iterations) {
		//contact with largest closing velocity
		glm::vec3 max(0);
		unsigned maxIndex = numContacts;
		for (i = 0; i < numContacts; i++) {
			glm::vec3 sepVel = contactArray[i].calculateSeparatingVelocity();

			if ((sepVel.x < max.x && sepVel.y < max.y && sepVel.z < max.z) && (sepVel.x < 0 && sepVel.y < 0 && sepVel.z < 0) || contactArray[i].penetration > 0) {
				max = sepVel;
				maxIndex = i;
			}
		}
		//check if there is something to resolve
		if (maxIndex == numContacts) {
			break;
		}

		//resolve contact
		contactArray[maxIndex].resolve(duration);
		iterationsUsed++;
	}
}
/*sets max iterations*/
void ParticleContactResolver::setIterations(unsigned iterations)
{
	ParticleContactResolver::iterations = iterations;
}

/*Constuctor*/
ParticleContactResolver::ParticleContactResolver(unsigned iterations):iterations(iterations)
{
}