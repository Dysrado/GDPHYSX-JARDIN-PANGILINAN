#pragma once
#include "ParticleForceGenerator.h"
class ParticleAnchoredSpring :
    public ParticleForceGenerator
{
private:
    glm::vec3 *anchor;

    float springConstant;
    float restLength;

public:
    ParticleAnchoredSpring(glm::vec3* anchor, float springConstant, float restLength);
    void setAnchor(glm::vec3* position);
    virtual void updateForce(Particle* particle, float duration);
};

