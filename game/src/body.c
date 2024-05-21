#include "body.h"
#include "world.h"
#include "integrator.h"

void Step(dlBody* body, float timeStep)
{
	body->force = Vector2Add(body->force, Vector2Scale(Vector2Scale(dlGravity, body->gravityScale), body->mass));
	body->acceleration = Vector2Scale(body->force, body->inverseMass);

	SemiImplicitEuler(body, timeStep);

	//apply damping
	float damping = 1.0f / (1.0f + timeStep * body->damping);
	body->velocity = Vector2Scale(body->velocity, damping);

	if (body->dissolve)
	{
		body->mass -= body->mass * body->dissolvePercent;
		body->inverseMass = 1 / body->mass;
	}

	ClearForce(body);
}
