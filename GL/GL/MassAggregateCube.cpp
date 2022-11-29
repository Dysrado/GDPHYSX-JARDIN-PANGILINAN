#include "MassAggregateCube.h"


MassAggregateCube::MassAggregateCube(ParticleWorld* world, ParticleForceGenerator* fg)
{
	glm::vec3 scale(1.25, 1.25, 1.25);

	particleArray = new Particle[8];

	for (int i = 0; i < 8; i++) {
		particleArray[i] = Particle();
	
	
	particleArray[0].initVariables(glm::vec3(0, 3, 5), scale, glm::vec3(0, 0, 0), 2); //F1
	particleArray[1].initVariables(glm::vec3(3, 3, 5), scale, glm::vec3(0, 0, 0), 2); //F2
	particleArray[2].initVariables(glm::vec3(3,0, 5), scale, glm::vec3(0, 0, 0), 2); //F4
	particleArray[3].initVariables(glm::vec3(0, 0, 5), scale, glm::vec3(0, 0, 0), 2); //F3
	particleArray[4].initVariables(glm::vec3(0, 3, 8), scale, glm::vec3(0, 0, 0), 2); //B1
	particleArray[5].initVariables(glm::vec3(3, 3, 8), scale, glm::vec3(0, 0, 0), 2); //B2
	particleArray[6].initVariables(glm::vec3(3, 0, 8), scale, glm::vec3(0, 0, 0), 2); //B4
	particleArray[7].initVariables(glm::vec3(0, 0, 8), scale, glm::vec3(0, 0, 0), 2); //B3
	

	
	for (int i = 0; i < 8; i++) {
		particleArray[i].init();
		
	}

	rods = new ParticleRod[12];
	//Horizontal Rods
	rods[0].particle[0] = &particleArray[0];
	rods[0].particle[1] = &particleArray[1];

	rods[2].particle[0] = &particleArray[2];
	rods[2].particle[1] = &particleArray[3];

	rods[4].particle[0] = &particleArray[4];
	rods[4].particle[1] = &particleArray[5];

	rods[6].particle[0] = &particleArray[6];
	rods[6].particle[1] = &particleArray[7];

	rods[8].particle[0] = &particleArray[0];
	rods[8].particle[1] = &particleArray[4];

	rods[9].particle[0] = &particleArray[1];
	rods[9].particle[1] = &particleArray[5];

	rods[10].particle[0] = &particleArray[2];
	rods[10].particle[1] = &particleArray[6];

	rods[11].particle[0] = &particleArray[3];
	rods[11].particle[1] = &particleArray[7];

	//Vertical Rods
	rods[1].particle[0] = &particleArray[2];
	rods[1].particle[1] = &particleArray[1];

	rods[3].particle[0] = &particleArray[3];
	rods[3].particle[1] = &particleArray[0];

	rods[5].particle[0] = &particleArray[6];
	rods[5].particle[1] = &particleArray[5];

	rods[7].particle[0] = &particleArray[7];
	rods[7].particle[1] = &particleArray[4];

	//Loop to assign length of rods
	for (int i = 0; i < 12; i++) {
		rods[i].length = 3.f;

	}

	//Pushback particles to world and apply gravity along with it
	for (int i = 0; i < 8; i++) 
	world->particles.push_back(&particleArray[i]);
	world->registry.add(&particleArray[i], fg);
	}
	
	
}


