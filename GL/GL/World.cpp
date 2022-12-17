#include "World.h"

void World::startFrame()
{
	for (RigidBodies::iterator b = bodies.begin(); b != bodies.end(); b++) { 
		(*b)->clearAccumulators();
		(*b)->calculateDerivedData();
	}
}

void World::integrate(float duration)
{
	for (RigidBodies::iterator b = bodies.begin(); b != bodies.end(); b++) 
	{// Integrate the body by the given duration.
		(*b)->integrate(duration);
	}
}

void World::runPhysics(float duration, std::vector<Particle*> particles)
{
	//registry.updateForces(duration);
	// Then integrate the objects.
	CollisionDetection(particles);
	integrate(duration);

}

void World::CollisionDetection(std::vector<Particle*> particles)
{
	for (int i = 0; i < particles.size(); i++) {
		float intersect = checkContacts(particles[i]);
		if (intersect < 1) {
			//Move the cube
			bodies[0]->addForce(glm::vec3(0, 0, 10000));
			bodies[0]->addTorque(glm::vec3(0, 0, 1000000000000));
			
			//Remove Particle
			particles[i]->setActive(false);
			particles[i]->setPosition(glm::vec3(-30,-5,0));
			std::cout << "This is colliding" << std::endl;
		}
	}
	

}

float World::checkContacts(Particle* a)
{
	return pow((a->getPosition().x - bodies[0]->transformMatrix.data[3]), 2) + pow((a->getPosition().y - bodies[0]->transformMatrix.data[7]), 2) + pow((a->getPosition().z - bodies[0]->transformMatrix.data[11]), 2);
}

void World::render(GLuint shaderProgram)
{
	for (RigidBodies::iterator b = bodies.begin(); b != bodies.end(); b++) {
		(*b)->render(shaderProgram);
	}
	
}
