#include "ParticleGravity.h"

void ParticleGravity::updateForce(Particle* particle, float duration)
{
	particle->addForce(gravity * particle->getMass());
}
