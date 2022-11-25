#pragma once
#include "Particle.h"
#include "ParticleContact.h"
class ParticleLink
{
public:
	Particle* particle[2];

protected:
	float currentLength() const;

public:
	virtual unsigned fillContact(ParticleContact* contact, unsigned limit) const = 0;
};

