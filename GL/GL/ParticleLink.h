#pragma once
#include "Particle.h"
#include "ParticleContact.h"
class ParticleLink : public ParticleContactGenerator
{
public:
	Particle* particle[2];

protected:
	float currentLength() const;

public:
	virtual unsigned addContact(ParticleContact* contact, unsigned limit) const = 0;
};

class ParticleRod : public ParticleLink
{
public:
	float length;
	virtual unsigned addContact(ParticleContact* contact, unsigned limit) const;
};

