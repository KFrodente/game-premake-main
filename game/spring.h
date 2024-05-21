#pragma once

typedef struct dlSpring
{
	struct dlBody* body1;
	struct dlBody* body2;
	float restLength;
	float k;

	struct dlSpring* next;
	struct dlSpring* prev;
} dlSpring_t;

extern dlSpring_t* dlSprings;

dlSpring_t* CreateSpring(struct dlBody* body1, struct dlBody* body2, float restLength, float k);
void AddSpring(dlSpring_t* spring);
void DestroySpring(dlSpring_t* spring);
void DestroyAllBodies();

void ApplySpringForce(dlSpring_t* spring);