#include "spring.h"
#include "body.h"

#include <stdlib.h>
#include <assert.h>
#include <string.h>

dlSpring_t* dlSprings = NULL;

dlSpring_t* CreateSpring(struct dlBody* body1, struct dlBody* body2, float restLength, float k)
{
	dlSpring_t* spring = (dlSpring_t*)malloc(sizeof(dlSpring_t));
	assert(spring);

	memset(spring, 0, sizeof(dlSpring_t));
	spring->body1 = body1;
	spring->body2 = body2;
	spring->restLength = restLength;
	spring->k = k;

	return spring;
}

void AddSpring(dlSpring_t* spring)
{
	assert(spring);

	spring->prev = NULL;
	spring->next = dlSprings;

	//If list is not empty, update 'prev' of existing head
	if (dlSprings) dlSprings->prev = spring;

	// set head of elements to new elemnt
	dlSprings = spring;
}

void DestroySpring(dlSpring_t* spring)
{
	assert(spring);

	if (spring->prev != NULL) spring->prev->next = spring->next;
	if (spring->next != NULL) spring->next->prev = spring->prev;

	if (spring == dlSprings) dlSprings = spring->next;

	free(spring);
}

void DestroyAllSprings()
{
	if (!dlSprings) return;
	dlSpring_t* spring = dlSprings;
	dlSpring_t* temp;

	while (spring)
	{
		temp = spring->next;
		free(spring);
		spring = temp;
	}
	dlSprings = NULL;
}



void ApplySpringForce(dlSpring_t* springs)
{
	for (dlSpring_t* spring = springs; spring; spring = spring->next)
	{
		Vector2 direction = Vector2Subtract(spring->body2->position, spring->body1->position);
		if (direction.x == 0 && direction.y == 0) continue;

		float length = Vector2Length(direction);
		float x = length - spring->restLength;
		float force = x * -spring->k;

		Vector2 ndirection = Vector2Normalize(direction);

		ApplyForce(spring->body2, Vector2Scale(ndirection, force), FM_FORCE);
		ApplyForce(spring->body1, Vector2Scale(ndirection, -force), FM_FORCE);
	}
}