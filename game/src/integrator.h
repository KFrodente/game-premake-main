#pragma once
#include "body.h"

inline void ExplicitEuler(dlBody* body, float timestep)
{
	body->position = Vector2Add(body->position, Vector2Scale(body->velocity, timestep));
	body->velocity = Vector2Add(body->velocity, Vector2Scale(body->acceleration, timestep));
}


inline void SemiImplicitEuler(dlBody* body, float timestep)
{
	body->velocity = Vector2Add(body->velocity, Vector2Scale(body->acceleration, timestep));
	body->position = Vector2Add(body->position, Vector2Scale(body->velocity, timestep));
}