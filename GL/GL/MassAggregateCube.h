#pragma once

#include "Particle.h"
#include "ParticleWorld.h"
#include "ParticleLink.h"


class MassAggregateCube
{
	Particle* particleArray;
	ParticleRod* rods;

	public:
		MassAggregateCube(ParticleWorld* world);
		void pushToWorld(ParticleWorld* world);


	
};

