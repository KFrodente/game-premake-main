#pragma once
#include "raylib.h"
#include "raymath.h"

typedef enum dlBodyType
{
	BT_DYNAMIC = 0,
	BT_KINEMATIC = 1,
	BT_STATIC = 2
} dlBodyType;

typedef enum dlForceMode
{
	FM_FORCE,
	FM_IMPULSE,
	FM_VELOCITY
} dlForceMode;

typedef struct dlBody
{
	dlBodyType type;

	// acceleration -> velocity -> position
	Vector2 position;
	Vector2 velocity;
	Vector2 acceleration;
	Vector2 force;

	Vector3 HSV;

	bool dissolve;
	float dissolvePercent;

	float mass;
	float inverseMass; //1 / mass (static = 0)
	float gravityScale;
	float damping;

	float restitution;

	struct dlBody* next;
	struct dlBody* prev;
} dlBody;

inline void ApplyForce(dlBody* body, Vector2 force, dlForceMode forceMode)
{
	if (body->type != BT_DYNAMIC) return;

	switch (forceMode)
	{
	case FM_FORCE:
		body->force = Vector2Add(body->force, force);
		break;
	case FM_IMPULSE:
		//applies a sudden change in momentum
		body->velocity = Vector2Scale(force, body->inverseMass);
		break;
	case FM_VELOCITY:
		body->velocity = force;
		break;
	}

}

inline void ClearForce(dlBody* body)
{
	body->force = Vector2Zero();
}

void Step(dlBody* body, float timeStep);
