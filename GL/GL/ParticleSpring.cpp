#include "ParticleSpring.h"
#include <math.h>

ParticleSpring::ParticleSpring(Particle* other, float springConstant, float restLength)
{
	this->other = other;
	this->springConstant = springConstant;
	this->restLength = restLength;
}

void ParticleSpring::updateForce(Particle* particle, float duration)
{
	glm::vec3 force = particle->getPosition();
	force -= other->getPosition();

	float magnitude = sqrt((force.x * force.x) + (force.y * force.y) + (force.z * force.z));
	magnitude = abs(magnitude - restLength);
	magnitude *= springConstant;

	glm::normalize(force);
	force *= -magnitude;
	particle->addForce(force);
}
