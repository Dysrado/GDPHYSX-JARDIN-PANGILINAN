#include "ParticleContact.h"

void ParticleContact::resolve(float duration)
{
	resolveVelocity(duration);
	resolveInterpenetration(duration);
}

glm::vec3 ParticleContact::calculateSeparatingVelocity() const
{
	glm::vec3 relativeVelocity = particle[0]->getVelocity();
	if (particle[1]) {
		relativeVelocity -= particle[1]->getVelocity();
		return relativeVelocity * contactNormal;
	}
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

	particle[0]->setPosition(particle[0]->getPosition() + movePerIMass * particle[0]->getMass());

	if (particle[1]) {
		particle[1]->setPosition(particle[1]->getPosition() + movePerIMass * particle[1]->getMass());
	}
}
