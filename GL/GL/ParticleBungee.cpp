#include "ParticleBungee.h"

ParticleBungee::ParticleBungee(Particle* other, float springConstant, float restLength)
{
	this->other = other;
	this->springConstant = springConstant;
	this->restLength = restLength;
}

void ParticleBungee::updateForce(Particle* particle, float duration)
{
	glm::vec3 force = particle->getPosition();
	force -= other->getPosition();

	float magnitude = sqrt((force.x * force.x) + (force.y * force.y) + (force.z * force.z));
	if (magnitude <= restLength) {
		return;
	}

	magnitude = springConstant * (magnitude - restLength);

	glm::normalize(force);
	force *= -magnitude;
	particle->addForce(force);
}
