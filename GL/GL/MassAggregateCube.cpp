#include "MassAggregateCube.h"


MassAggregateCube::MassAggregateCube(ParticleWorld* world)
{
	glm::vec3 scale(1.25, 1.25, 1.25);

	/*for (int i = 0; i < 8;  i++) {
		particleArray[i] = new Particle();
		
	}*/
	particleArray = new Particle[8];

	for (int i = 0; i < 8; i++) {
		particleArray[i] = Particle();
	}
	/*Particle* temp = new Particle();
	temp->initVariables(glm::vec3(-3, -1, 5), scale, glm::vec3(0, 0, 0), 2);
	temp->init();
	world->particles.push_back(temp);*/

	

	//rods = new ParticleRod[12];
	//particleArray[0] = Particle();
	
	particleArray[0].initVariables(glm::vec3(-1, 0, 5), scale, glm::vec3(0, 0, 0), 2); //F1
	particleArray[1].initVariables(glm::vec3(-0, 0, 5), scale, glm::vec3(0, 0, 0), 2); //F2
	particleArray[2].initVariables(glm::vec3(-0, -1, 5), scale, glm::vec3(0, 0, 0), 2); //F4
	particleArray[3].initVariables(glm::vec3(-1, -1, 5), scale, glm::vec3(0, 0, 0), 2); //F3
	particleArray[4].initVariables(glm::vec3(-1, 0, 7.5), scale, glm::vec3(0, 0, 0), 2); //B1
	particleArray[5].initVariables(glm::vec3(-0, 0, 7.5), scale, glm::vec3(0, 0, 0), 2); //B2
	particleArray[6].initVariables(glm::vec3(-0, -1, 7.5), scale, glm::vec3(0, 0, 0), 2); //B4
	particleArray[7].initVariables(glm::vec3(-1, -1, 7.5), scale, glm::vec3(0, 0, 0), 2); //B3
	

	
	for (int i = 0; i < 8; i++) {
		particleArray[i].init();
		
	}

	rods = new ParticleRod[12];
	
	
	rods[0].particle[0] = &particleArray[0];
	rods[0].particle[1] = &particleArray[1];
	rods[0].length = glm::distance(particleArray[0].getPosition(), particleArray[1].getPosition());

	rods[1].particle[0] = &particleArray[1];
	rods[1].particle[1] = &particleArray[2];
	rods[1].length = glm::distance(particleArray[1].getPosition(), particleArray[2].getPosition());

	rods[2].particle[0] = &particleArray[2];
	rods[2].particle[1] = &particleArray[3];
	rods[2].length = glm::distance(particleArray[2].getPosition(), particleArray[3].getPosition());

	rods[3].particle[0] = &particleArray[3];
	rods[3].particle[1] = &particleArray[0];
	rods[3].length = glm::distance(particleArray[3].getPosition(), particleArray[0].getPosition());

	rods[4].particle[0] = &particleArray[4];
	rods[4].particle[1] = &particleArray[5];
	rods[4].length = glm::distance(particleArray[4].getPosition(), particleArray[5].getPosition());

	rods[5].particle[0] = &particleArray[5];
	rods[5].particle[1] = &particleArray[6];
	rods[5].length = glm::distance(particleArray[5].getPosition(), particleArray[6].getPosition());

	rods[6].particle[0] = &particleArray[6];
	rods[6].particle[1] = &particleArray[7];
	rods[6].length = glm::distance(particleArray[6].getPosition(), particleArray[7].getPosition());

	rods[7].particle[0] = &particleArray[7];
	rods[7].particle[1] = &particleArray[4];
	rods[7].length = glm::distance(particleArray[7].getPosition(), particleArray[4].getPosition());

	rods[8].particle[0] = &particleArray[0];
	rods[8].particle[1] = &particleArray[4];
	rods[8].length = glm::distance(particleArray[0].getPosition(), particleArray[4].getPosition());

	rods[9].particle[0] = &particleArray[1];
	rods[9].particle[1] = &particleArray[5];
	rods[9].length = glm::distance(particleArray[1].getPosition(), particleArray[5].getPosition());

	rods[10].particle[0] = &particleArray[2];
	rods[10].particle[1] = &particleArray[6];
	rods[10].length = glm::distance(particleArray[2].getPosition(), particleArray[6].getPosition());

	rods[11].particle[0] = &particleArray[3];
	rods[11].particle[1] = &particleArray[7];
	rods[11].length = glm::distance(particleArray[3].getPosition(), particleArray[7].getPosition());

	for (int i = 0; i < 12; i++) {
		world->getContactGenerator().push_back(&rods[i]);
	}

		//world->particles.push_back(particleArray[i]);
	//particleArray[0] = new Particle();
	//world->particles.push_back(&particleArray[0]);
	//world->particles.push_back(rods[0].particle[0]);
	//world->particles.push_back(rods[0].particle[1]);
		//world->particles.push_back(&particleArray[0]);

	for (int i = 0; i < 8; i++) {
		//particleArray[i] = Particle();
		//std::cout << "Particle Array A: " << &particleArray[i] << std::endl;
		//world->push_back(&particleArray[i]);
	world->particles.push_back(&particleArray[i]);
	}
	//std::cout << "Position of First: " << particleArray[0].getPosition().x << std::endl;
	//world->push_back(&particleArray[0]);
	
}

void MassAggregateCube::pushToWorld(ParticleWorld* world)
{
	for (int i = 0; i < 8; i++) {
		world->particles.push_back(&particleArray[i]);
	}
}
