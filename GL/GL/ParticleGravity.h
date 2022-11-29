#pragma once
#include "ParticleForceGenerator.h"
class ParticleGravity: public ParticleForceGenerator
{
private:
	glm::vec3 gravity;

public: 
	ParticleGravity(const glm::vec3 &gravity);
	virtual void updateForce(Particle* particle, float duration);
	glm::vec3 getGravity(Particle* particle);
};

