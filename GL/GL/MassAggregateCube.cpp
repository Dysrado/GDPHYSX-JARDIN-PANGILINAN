#include "MassAggregateCube.h"


MassAggregateCube::MassAggregateCube(ParticleWorld* world, ParticleForceGenerator* fg)
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
	
	//Horizontal
	rods[0].particle[0] = &particleArray[0];
	rods[0].particle[1] = &particleArray[1];
	//rods[0].length = abs(particleArray[0].getPosition().x - particleArray[1].getPosition().x);

	rods[1].particle[0] = &particleArray[1];
	rods[1].particle[1] = &particleArray[2];
	//rods[1].length = abs(particleArray[1].getPosition().y - particleArray[2].getPosition().y);

	rods[2].particle[0] = &particleArray[2];
	rods[2].particle[1] = &particleArray[3];
	//rods[2].length = abs(particleArray[2].getPosition().x - particleArray[3].getPosition().x);

	rods[3].particle[0] = &particleArray[3];
	rods[3].particle[1] = &particleArray[0];
	//rods[3].length = abs(particleArray[3].getPosition().y - particleArray[0].getPosition().y);

	rods[4].particle[0] = &particleArray[4];
	rods[4].particle[1] = &particleArray[5];
	//rods[4].length = abs(particleArray[4].getPosition().x - particleArray[5].getPosition().x);

	rods[5].particle[0] = &particleArray[5];
	rods[5].particle[1] = &particleArray[6];
	//rods[5].length = abs(particleArray[5].getPosition().y - particleArray[6].getPosition().y);

	rods[6].particle[0] = &particleArray[6];
	rods[6].particle[1] = &particleArray[7];
	//rods[6].length = abs(particleArray[6].getPosition().x - particleArray[7].getPosition().x);

	rods[7].particle[0] = &particleArray[7];
	rods[7].particle[1] = &particleArray[4];
	//rods[7].length = abs(particleArray[7].getPosition().y - particleArray[4].getPosition().y);

	rods[8].particle[0] = &particleArray[0];
	rods[8].particle[1] = &particleArray[4];
	//rods[8].length = abs(particleArray[0].getPosition().x - particleArray[4].getPosition().x);

	rods[9].particle[0] = &particleArray[1];
	rods[9].particle[1] = &particleArray[5];
	//rods[9].length = abs(particleArray[1].getPosition().x - particleArray[5].getPosition().x);

	rods[10].particle[0] = &particleArray[2];
	rods[10].particle[1] = &particleArray[6];
	//rods[10].length = abs(particleArray[2].getPosition().x - particleArray[6].getPosition().x);

	rods[11].particle[0] = &particleArray[3];
	rods[11].particle[1] = &particleArray[7];
	//rods[11].length = abs(particleArray[3].getPosition().x - particleArray[7].getPosition().x);
	
	for (int i = 0; i < 12; i++) {
		rods[i].length = 3.f;
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
	world->registry.add(&particleArray[i], fg);
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
