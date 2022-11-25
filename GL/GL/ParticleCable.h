#pragma once
#include "ParticleLink.h"

class ParticleCable: public ParticleLink
{
public:
	float maxLength;

	float restitution;

	virtual unsigned fillContact(ParticleContact *contact, unsigned limit) const;
};

