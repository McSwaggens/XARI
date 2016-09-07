#ifndef __PX_PARTICLE_H_
#define __PX_PARTICLE_H_

#include "engine.h"

struct PxParticle
{
	Vector2 position;
	Color color;
	
	PxParticle (Vector2 position, Color color)
	{
		this->position = position;
		this->color = color;
	}
};



#endif