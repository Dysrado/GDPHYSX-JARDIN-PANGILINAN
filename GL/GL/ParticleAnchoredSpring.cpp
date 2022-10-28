#include "ParticleAnchoredSpring.h"

ParticleAnchoredSpring::ParticleAnchoredSpring(glm::vec3* anchor, float springConstant, float restLength)
{
	this->anchor = anchor;
	this->springConstant = springConstant;
	this->restLength = restLength;
}

void ParticleAnchoredSpring::setAnchor(glm::vec3* position)
{
	this->anchor = position;
}

void ParticleAnchoredSpring::updateForce(Particle* particle, float duration)
{
	glm::vec3 force = particle->getPosition();
	force -= *anchor;

	float magnitude = sqrt((force.x * force.x) + (force.y * force.y) + (force.z * force.z));
	magnitude = abs(magnitude - restLength);
	magnitude *= springConstant;

	glm::normalize(force);
	force *= -magnitude;
	particle->addForce(force);

}
