#pragma once
#include "ParticleRod.h"
#include "Particle.h"
#include "ParticleWorld.h"
class MassAggregateCube
{
	Particle* particleArray[8];
	ParticleRod* rods[12];

	public:
		MassAggregateCube();
		void pushToWorld(ParticleWorld* world);


	
};

