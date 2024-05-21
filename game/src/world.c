#include "world.h"
//#include "body.h"
#include <stdlib.h>
#include <assert.h>
#include <string.h>

dlBody* dlBodies = NULL;
int dlBodyCount = 0;
Vector2 dlGravity;

dlBody* CreateBody(Vector2 position, float mass, dlBodyType bodyType)
{

	//Allocate memory for new Body
	dlBody* body = (dlBody*)malloc(sizeof(dlBody));
	//Check if allocation is successful
	assert(body);

	memset(body, 0, sizeof(dlBody));
	body->position = position;
	body->mass = mass;
	body->inverseMass = (bodyType == BT_DYNAMIC) ? 1 / mass : 0;
	body->type = bodyType;



	return body;
}

void AddBody(dlBody* body)
{
	assert(body);

	body->prev = NULL;
	body->next = dlBodies;
	if (dlBodies)
		dlBodies->prev = body;
	dlBodies = body;
	dlBodyCount++;
}


void DestroyBody(dlBody* body)
{
	//Assert if provided Body is not NULL
	assert(body);
	//If 'prev' is not NULL, set 'prev->next' to 'body->next'
	if (body->prev)
		body->prev->next = body->next;
		
	//If 'next' is not NULL, set 'next->prev' to 'body->prev'
	if (body->next)
		body->next->prev = body->prev;
	//If body is the head, update head to 'body->next'
	if (body == dlBodies)
		dlBodies = body->next;
	//Decrement body count
	dlBodyCount--;
	//Free the body
	free(body);
}