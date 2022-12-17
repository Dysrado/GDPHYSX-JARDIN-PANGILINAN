#include "World.h"

/// <summary>
/// Removes the previous calculations of the rigidbodies
/// </summary>
void World::startFrame()
{
	for (RigidBodies::iterator b = bodies.begin(); b != bodies.end(); b++) { 
		// clears the accumulated data.
		(*b)->clearAccumulators();

		// calculates the data for the transform matrix.
		(*b)->calculateDerivedData();
	}
}

/// <summary>
/// Calls the integrate function of all of the rigidbodies.
/// </summary>
/// <param name="duration"> the time difference of each frame </param>
void World::integrate(float duration)
{
	for (RigidBodies::iterator b = bodies.begin(); b != bodies.end(); b++) 
	{// Integrate the body by the given duration.
		(*b)->integrate(duration);
	}
}

/// <summary>
/// Runs the physics of all rigidbodies
/// </summary>
/// <param name="duration"> the time difference of each frame </param>
/// <param name="particles"> the list of particles </param>
void World::runPhysics(float duration, std::vector<Particle*> particles)
{
	// Checks if there's a collision.
	CollisionDetection(particles);
	// Then integrate the objects.
	integrate(duration);

}

/// <summary>
/// Checks if any particle collides with the rigidbody.
/// </summary>
/// <param name="particles"> Get the list of particles in particle world </param>
void World::CollisionDetection(std::vector<Particle*> particles)
{
	for (int i = 0; i < particles.size(); i++) {
		float intersect = checkContacts(particles[i]);
		if (intersect < 1) {
			//Moves the cube.
			bodies[0]->addForce(glm::vec3(0, 0, 10000));
			bodies[0]->addTorque(glm::vec3(0, 0, 1000000000000));
			
			//Remove Particle.
			particles[i]->setActive(false);
			particles[i]->setPosition(glm::vec3(-30,-5,0));
		}
	}
	

}

/// <summary>
/// Calculates the distance of the particle and rigidbody and checks if they are colliding
/// </summary>
/// <param name="a">  </param>
/// <returns> By how much is the intersection </returns>
float World::checkContacts(Particle* a)
{
	return pow((a->getPosition().x - bodies[0]->transformMatrix.data[3]), 2) + pow((a->getPosition().y - bodies[0]->transformMatrix.data[7]), 2) + pow((a->getPosition().z - bodies[0]->transformMatrix.data[11]), 2);
}

/// <summary>
/// Renders the rigidbodies found in bodies.
/// </summary>
/// <param name="shaderProgram"> shader data for the rendering </param>
void World::render(GLuint shaderProgram)
{
	for (RigidBodies::iterator b = bodies.begin(); b != bodies.end(); b++) {
		(*b)->render(shaderProgram);
	}
	
}
