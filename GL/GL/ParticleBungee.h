#pragma once
#include "ParticleForceGenerator.h"
class ParticleBungee :
    public ParticleForceGenerator
{
private:
    Particle* other;

    float springConstant;
    float restLength;

public:
    ParticleBungee(Particle* other, float springConstant, float restLength);

    virtual void updateForce(Particle* particle, float duration);
};

