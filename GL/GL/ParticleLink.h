#pragma once
#include "Particle.h"
#include "ParticleContact.h"
class ParticleLink : public ParticleContactGenerator
{
public:
	/**
	* Holds the pair of particles that are connected by this link.
	*/
	Particle* particle[2];

protected:
	/**
	* Returns the current length of the link.
	*/
	float currentLength() const;

public:
	/* adds the contact and its data such as the contact normal and penetration */
	virtual unsigned addContact(ParticleContact* contact, unsigned limit) const = 0;
};

/**
 * Rods link a pair of particles, generating a contact if they
 * stray too far apart or too close.
 */
class ParticleRod : public ParticleLink
{
public:
	/**
	* Holds the length of the rod.
	*/
	float length;
	/**
	* Fills the given contact structure with the contact needed
	* to keep the rod from extending or compressing.
	*/
	virtual unsigned addContact(ParticleContact* contact, unsigned limit) const;
};

