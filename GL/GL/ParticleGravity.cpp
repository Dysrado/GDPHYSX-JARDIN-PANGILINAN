#include "ParticleGravity.h"

ParticleGravity::ParticleGravity(const glm::vec3& gravity)
{
	this->gravity = gravity;
}

void ParticleGravity::updateForce(Particle* particle, float duration)
{
	if (particle->getPosition().y >= -1.5f) {
		particle->addForce(gravity * particle->getMass());
	}
	
}

glm::vec3 ParticleGravity::getGravity(Particle* particle)
{
	if (particle->getPosition().y >= -1.5f) {
		return gravity * particle->getMass();
	}
}
