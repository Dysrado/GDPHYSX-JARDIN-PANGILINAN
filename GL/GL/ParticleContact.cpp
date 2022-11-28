#include "ParticleContact.h"

void ParticleContact::resolve(float duration)
{
	resolveVelocity(duration);
	resolveInterpenetration(duration);
}

glm::vec3 ParticleContact::calculateSeparatingVelocity()
{
	glm::vec3 relativeVelocity = particle[0]->getVelocity();
	if (particle[1]) {
		relativeVelocity -= particle[1]->getVelocity();
		
	}
	return relativeVelocity * contactNormal;
}

void ParticleContact::resolveVelocity(float duration)
{
	glm::vec3 separatingVelocity = calculateSeparatingVelocity();

	if (separatingVelocity.x > 0 && separatingVelocity.y > 0 && separatingVelocity.z > 0) {
		return;
	}

	glm::vec3 newSepVelocity = -separatingVelocity * restitution;

	
	// not sure if forceAccum or Acceleration 
	glm::vec3 accCausedVelocity = particle[0]->getAcceleration();
	if (particle[1]) {
		accCausedVelocity -= particle[1]->getAcceleration();
	}

	glm::vec3 accCausedSepVelocity = accCausedVelocity * contactNormal * duration;

	if (accCausedSepVelocity.x < 0 && accCausedSepVelocity.y < 0 && accCausedSepVelocity.z < 0) {
		newSepVelocity += restitution * accCausedSepVelocity;
		if (newSepVelocity.x < 0 && newSepVelocity.y < 0 && newSepVelocity.z < 0) {
			newSepVelocity.x = 0;
			newSepVelocity.y = 0;
			newSepVelocity.z = 0;
		}
	}
	glm::vec3 deltaVelocity = newSepVelocity - separatingVelocity;

	float totalInverseMass = particle[0]->getMass();
	if (particle[1]) {
		totalInverseMass += particle[1]->getMass();
	}

	if (totalInverseMass <= 0) {
		return;
	}
	glm::vec3 impulse = deltaVelocity / totalInverseMass;

	glm::vec3 impulsePerIMass = contactNormal * impulse;

	particle[0]->setVelocity(particle[0]->getVelocity() + impulsePerIMass * particle[0]->getMass());

	if (particle[1]) {
		particle[1]->setVelocity(particle[1]->getVelocity() + impulsePerIMass * -particle[1]->getMass());
	}




}

void ParticleContact::resolveInterpenetration(float duration)
{
	if (penetration <= 0) {
		return;
	}

	float totalInverseMass = particle[0]->getMass();

	if (particle[1]) {
		totalInverseMass += particle[1]->getMass();
	}

	if (totalInverseMass <= 0) {
		return;
	}

	glm::vec3 movePerIMass = contactNormal * (penetration / totalInverseMass);

	particleMovement[0] = movePerIMass * particle[0]->getMass();
	if (particle[1]) {
		particleMovement[1] = movePerIMass * -particle[1]->getMass();

	}
	else {
		particleMovement[1] = glm::vec3(0,0,0);
	}


	particle[0]->setPosition(particle[0]->getPosition() +  particleMovement[0]);

	if (particle[1]) {
		particle[1]->setPosition(particle[1]->getPosition() + particleMovement[1]);
	}
	
}

void ParticleContactResolver::resolveContacts(ParticleContact* contactArray, unsigned numContacts, float duration)
{
	unsigned i;
	iterationsUsed = 0;
	while (iterationsUsed < iterations) {
		glm::vec3 max(0);
		unsigned maxIndex = numContacts;
		for (i = 0; i < numContacts; i++) {
			glm::vec3 sepVel = contactArray[i].calculateSeparatingVelocity();

			if ((sepVel.x < max.x && sepVel.y < max.y && sepVel.z < max.z) && (sepVel.x < 0 && sepVel.y < 0 && sepVel.z < 0) || contactArray[i].penetration > 0) {
				max = sepVel;
				maxIndex = i;
			}
		}

		if (maxIndex == numContacts) {
			break;
		}


		contactArray[maxIndex].resolve(duration);
		iterationsUsed++;
	}
}

void ParticleContactResolver::setIterations(unsigned iterations)
{
	ParticleContactResolver::iterations = iterations;
}


ParticleContactResolver::ParticleContactResolver(unsigned iterations):iterations(iterations)
{
}