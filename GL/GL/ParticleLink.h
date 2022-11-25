#pragma once
#include "Particle.h"
#include "ParticleContact.h"
class ParticleLink
{
public:
	Particle* paricle[2];

protected:
	float currentLenth() const;

public:
	virtual unsigned fillContact(ParticleContact* contact, unsigned limit) const = 0;
};

