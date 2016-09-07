#ifndef __PARTICLE_SYSTEM_H_
#define __PARTICLE_SYSTEM_H_

#include "../engine.h"
#include "particle.h"
#include <cmath>

class ParticleSystem : public Actor
{
private:
	std::vector<Particle> particles;
	VertexArray verticies;
	sf::Time lifeTime;
public:
	ParticleSystem (unsigned int particleCount, float lifeTime);
	void Tick ();
	virtual void draw (sf::RenderTarget& target, sf::RenderStates states) const;
	void ResetParticle (unsigned int particleIndex);
	virtual void GetAliveParticles () { };
};

#endif
