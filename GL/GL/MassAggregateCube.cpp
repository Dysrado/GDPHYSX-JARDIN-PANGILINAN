#include "MassAggregateCube.h"

MassAggregateCube::MassAggregateCube()
{
	glm::vec3 scale(0.5, 0.5, 0.5);

	for (int i = 0; i < 8;  i++) {
		particleArray[i] = new Particle();
	}
	particleArray[0]->initVariables(glm::vec3(1, 1, 5), scale, glm::vec3(0, 0, 0), 2);
	particleArray[1]->initVariables(glm::vec3(0, 1, 5), scale, glm::vec3(0, 0, 0), 2);
	particleArray[2]->initVariables(glm::vec3(0, 0, 5), scale, glm::vec3(0, 0, 0), 2);
	particleArray[3]->initVariables(glm::vec3(1, 0, 5), scale, glm::vec3(0, 0, 0), 2);
	particleArray[4]->initVariables(glm::vec3(1, 1, 7.5), scale, glm::vec3(0, 0, 0), 2);
	particleArray[5]->initVariables(glm::vec3(0, 1, 7.5), scale, glm::vec3(0, 0, 0), 2);
	particleArray[6]->initVariables(glm::vec3(0, 0, 7.5), scale, glm::vec3(0, 0, 0), 2);
	particleArray[7]->initVariables(glm::vec3(1, 0, 7.5), scale, glm::vec3(0, 0, 0), 2);
	
	for (int i = 0; i < 8; i++) {
		particleArray[i]->init();
	}
	
}

void MassAggregateCube::pushToWorld(ParticleWorld* world)
{
	for (int i = 0; i < 8; i++) {
		world->push_back(particleArray[i]);
	}
}
