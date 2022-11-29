#include "ParticleGravity.h"

// Assigns the gravity to the ParticleGravity class
ParticleGravity::ParticleGravity(const glm::vec3& gravity)
{
	this->gravity = gravity;
}

// Updates the force to apply gravity to the particle
void ParticleGravity::updateForce(Particle* particle, float duration)
{
	if (particle->getPosition().y >= -1.5f) {
		particle->addForce(gravity * particle->getMass()); // Apply the mass-scaled force to the particle
	}
	
}

// gets the gravity of the particle
glm::vec3 ParticleGravity::getGravity(Particle* particle)
{
	if (particle->getPosition().y >= -1.5f) { // Constrains the particle from falling to far into the ground
		return gravity * particle->getMass(); 
	}
}
