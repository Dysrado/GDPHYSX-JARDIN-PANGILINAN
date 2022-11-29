#pragma once
#include "Particle.h"

class ParticleForceGenerator 
{
public:
    virtual void updateForce(Particle *particle, float duration) = 0; // Abstact function: used to update the particles force
};