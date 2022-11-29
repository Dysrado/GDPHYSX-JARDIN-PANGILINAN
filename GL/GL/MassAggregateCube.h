#pragma once

#include "Particle.h"
#include "ParticleWorld.h"
#include "ParticleLink.h"
#include "ParticleForceGenerator.h"


class MassAggregateCube
{
	Particle* particleArray;
	ParticleRod* rods;

	public:
		MassAggregateCube(ParticleWorld* world, ParticleForceGenerator* fg);
		void pushToWorld(ParticleWorld* world);


	
};

