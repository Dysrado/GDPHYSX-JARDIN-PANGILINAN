#pragma once
#include "ParticleLink.h"
#include "ParticleContact.h"
class ParticleRod : public ParticleLink
{
public:
	float length;
	float currentLength() const;
	virtual unsigned fillContact(ParticleContact* contact, unsigned limit) const;
};

