#pragma once
#include "ParticleContact.h"
class ParticleContactGenerator
{
	virtual unsigned addContact(ParticleContact* contact, unsigned limit) const = 0;
};

