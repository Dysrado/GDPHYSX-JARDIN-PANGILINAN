#pragma once

#include "Particle.h"
#include "ParticleWorld.h"
#include "ParticleLink.h"
#include "ParticleForceGenerator.h"
#include "ParticleSpring.h"


class MassAggregateCube
{
	//Particles to form the corners
	Particle* particleArray;
	//Rods to connect the corners
	ParticleRod* rods;

	public:
		MassAggregateCube(ParticleWorld* world, ParticleForceGenerator* fg);
};

