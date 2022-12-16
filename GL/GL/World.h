#pragma once
#include <vector>
#include "RigidBody.h"
#include "Particle.h"
class World
{
public:
	typedef std::vector<RigidBody*> RigidBodies; 
	void startFrame();
	void integrate(float duration);
	void runPhysics(float duration, std::vector<Particle*> particles);
	RigidBodies bodies;

	//Simple Collision Detection
	void CollisionDetection(std::vector<Particle*> particles);
	float checkContacts(Particle* a);
	void render(GLuint shaderProgram);
};

