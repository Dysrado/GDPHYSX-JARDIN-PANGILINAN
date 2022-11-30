#pragma once
#include "ParticleForceGenerator.h"
class ParticleSpring :
    public ParticleForceGenerator
{
    /** The particle at the other end of the spring. */
    Particle* other;
    /** Holds the spring constant. */
    float springConstant;
    /** Holds the rest length of the spring. */
    float restLength;

public:
    /** Creates a new spring with the given parameters. */
    ParticleSpring(Particle* other, float springConstant, float restLength);

    /** Applies the spring force to the given particle. */
    virtual void updateForce(Particle* particle, float duration);
};

