#include "ParticleSpring.h"

ParticleSpring::ParticleSpring(Particle* other, float springConstant, float restLength)
{
	this->other = other;
	this->springConstant = springConstant;
	this->restLength = restLength;
}

void ParticleSpring::updateForce(Particle* particle, float duration)
{
	// Calculate the vector of the spring.
	glm::vec3 force = particle->getPosition();

	//particle->setPosition(&force);

	force -= other->getPosition();
	// Calculate the magnitude of the force.
	float magnitude = glm::length(force);
	magnitude = abs(magnitude - restLength);
	magnitude *= springConstant;
	// Calculate the final force and apply it.
	force = glm::normalize(force);
	force *= -magnitude;
	particle->addForce(force);
}
