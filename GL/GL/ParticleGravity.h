#pragma once
#include "ParticleForceGenerator.h"
class ParticleGravity: public ParticleForceGenerator
{
private:
	glm::vec3 gravity;

public: 
	ParticleGravity(const glm::vec3 &gravity); // Constructor
	virtual void updateForce(Particle* particle, float duration); // Applies gravitational force to the particle
	glm::vec3 getGravity(Particle* particle); // Gets the particles gravity
};

