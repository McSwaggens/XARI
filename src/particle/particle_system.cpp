#include "particle_system.h"

ParticleSystem::ParticleSystem (unsigned int particleCount, float _lifeTime) :
		particles	(particleCount				),
		verticies	(sf::Points, particleCount	),
		lifeTime	(sf::seconds(_lifeTime)		) { }

void ParticleSystem::Tick ()
{
	for (unsigned int i = 0; i < particles.size (); i++)
	{
		Particle* particle = &particles[i];
		
		particle->lifeTime -= engine->DeltaTime ();
		
		if (particle->lifeTime <= 0)
		{
			this->ResetParticle (i);
		}
		
		verticies[i].position += particle->velocity * engine->DeltaTime ();
		
		float ratio = particle->lifeTime / lifeTime.asSeconds();
		verticies[i].color.a = static_cast<sf::Uint8>(ratio * 255);
		verticies[i].color.r = static_cast<sf::Uint8>(ratio * 255);
		verticies[i].color.g = 0;
		verticies[i].color.b = 255 - static_cast<sf::Uint8>(ratio * 255);
	}
}

void ParticleSystem::draw (sf::RenderTarget& target, sf::RenderStates states)
const
{
	states.texture = NULL;
	target.draw (verticies, states);
}

void ParticleSystem::ResetParticle (unsigned int particleIndex)
{
	float angle = (std::rand() % 360) * 3.14f / 180.f;
	float speed = (std::rand() % 150) + 50.f;
	
	particles[particleIndex].velocity = sf::Vector2f(std::cos(angle) * speed, std::sin(angle) * speed);
	particles[particleIndex].lifeTime = sf::milliseconds((std::rand() % 2000) + 1000).asSeconds();
	
	verticies[particleIndex].position = this->getPosition ();
}
