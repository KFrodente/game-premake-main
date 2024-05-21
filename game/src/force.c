#include "force.h"
#include "body.h"
#include "raymath.h"


void ApplyGravitation(dlBody* bodies, float strength)
{
	for (dlBody* body1 = bodies; body1; body1 = body1->next)
	{
		for (dlBody* body2 = bodies; body2; body2 = body2->next)
		{
			if (body1 == body2) continue;

			Vector2 direction = Vector2Subtract(body1->position, body2->position);
			float distance = Vector2Length(direction);

			distance = fmaxf(1, distance);
			float force = (body1->mass * body2->mass / (distance * distance)) * strength;

			direction = Vector2Scale(Vector2Normalize(direction), force);

			ApplyForce(body1, direction, FM_FORCE);
			ApplyForce(body2, Vector2Scale(direction, -1), FM_FORCE);
		}
	}
}