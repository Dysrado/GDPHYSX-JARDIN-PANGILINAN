#include "MassAggregateCube.h"


MassAggregateCube::MassAggregateCube(ParticleWorld* world, ParticleForceGenerator* fg)
{
	glm::vec3 scale(1, 1, 1);

	particleArray = new Particle[8];

	for (int i = 0; i < 8; i++) {
		particleArray[i] = Particle();
	
		particleArray[0].initVariables(glm::vec3(0, 2, 3), scale, glm::vec3(0, 0, 0), 2); //F1
		particleArray[1].initVariables(glm::vec3(2, 2, 3), scale, glm::vec3(0, 0, 0), 2); //F2
		particleArray[2].initVariables(glm::vec3(2, 0, 3), scale, glm::vec3(0, 0, 0), 2); //F4
		particleArray[3].initVariables(glm::vec3(0, 0, 3), scale, glm::vec3(0, 0, 0), 2); //F3
		particleArray[4].initVariables(glm::vec3(0, 2, 5), scale, glm::vec3(0, 0, 0), 2); //B1
		particleArray[5].initVariables(glm::vec3(2, 2, 5), scale, glm::vec3(0, 0, 0), 2); //B2
		particleArray[6].initVariables(glm::vec3(2, 0, 5), scale, glm::vec3(0, 0, 0), 2); //B4
		particleArray[7].initVariables(glm::vec3(0, 0, 5), scale, glm::vec3(0, 0, 0), 2); //B3
	
		for (int i = 0; i < 8; i++) {
			particleArray[i].init();
		}

		float restLength = 2;
		float springConstant = 2;

		rods = new ParticleRod[12];
		//Horizontal Rods
		rods[0].particle[0] = &particleArray[0];
		rods[0].particle[1] = &particleArray[1];
		ParticleSpring* ps1 = new ParticleSpring(&particleArray[1], springConstant, restLength);
		world->registry.add(&particleArray[0], ps1);

		rods[2].particle[0] = &particleArray[2];
		rods[2].particle[1] = &particleArray[3];
		ParticleSpring* ps2 = new ParticleSpring(&particleArray[2], springConstant, restLength);
		world->registry.add(&particleArray[3], ps2);

		rods[4].particle[0] = &particleArray[4];
		rods[4].particle[1] = &particleArray[5];
		ParticleSpring* ps3 = new ParticleSpring(&particleArray[4], springConstant, restLength);
		world->registry.add(&particleArray[5], ps3);

		rods[6].particle[0] = &particleArray[6];
		rods[6].particle[1] = &particleArray[7];
		ParticleSpring* ps4 = new ParticleSpring(&particleArray[6], springConstant, restLength);
		world->registry.add(&particleArray[7], ps4);

		rods[8].particle[0] = &particleArray[0];
		rods[8].particle[1] = &particleArray[4];
		ParticleSpring* ps5 = new ParticleSpring(&particleArray[0], springConstant, restLength);
		world->registry.add(&particleArray[4], ps5);

		rods[9].particle[0] = &particleArray[1];
		rods[9].particle[1] = &particleArray[5];
		ParticleSpring* ps6 = new ParticleSpring(&particleArray[1], springConstant, restLength);
		world->registry.add(&particleArray[5], ps6);

		rods[10].particle[0] = &particleArray[2];
		rods[10].particle[1] = &particleArray[6];
		ParticleSpring* ps7 = new ParticleSpring(&particleArray[2], springConstant, restLength);
		world->registry.add(&particleArray[6], ps7);

		rods[11].particle[0] = &particleArray[3];
		rods[11].particle[1] = &particleArray[7];
		ParticleSpring* ps8 = new ParticleSpring(&particleArray[3], springConstant, restLength);
		world->registry.add(&particleArray[7], ps8);

		//Vertical Rods
		rods[1].particle[0] = &particleArray[2];
		rods[1].particle[1] = &particleArray[1];		
		ParticleSpring* ps9 = new ParticleSpring(&particleArray[2], springConstant, restLength);
		world->registry.add(&particleArray[1], ps9);

		rods[3].particle[0] = &particleArray[3];
		rods[3].particle[1] = &particleArray[0];
		ParticleSpring* ps10 = new ParticleSpring(&particleArray[3], springConstant, restLength);
		world->registry.add(&particleArray[0], ps10);

		rods[5].particle[0] = &particleArray[6];
		rods[5].particle[1] = &particleArray[5];
		ParticleSpring* ps11 = new ParticleSpring(&particleArray[6], springConstant, restLength);
		world->registry.add(&particleArray[5], ps11);

		rods[7].particle[0] = &particleArray[7];
		rods[7].particle[1] = &particleArray[4];
		ParticleSpring* ps12 = new ParticleSpring(&particleArray[7], springConstant, restLength);
		world->registry.add(&particleArray[4], ps12);

		//Loop to assign length of rods
		for (int i = 0; i < 12; i++) {
			rods[i].length = 3.f;
		}

		//Pushback particles to world and apply gravity along with it
		for (int i = 0; i < 8; i++) {
			world->particles.push_back(&particleArray[i]);
			//world->registry.add(&particleArray[i], fg);
		}
	}
}


