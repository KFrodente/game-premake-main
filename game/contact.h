#pragma once
#include "raylib.h"

typedef struct dlContact
{
	struct dlBody* body1;
	struct dlBody* body2;

	float restitution;
	float depth;
	Vector2 normal;

	struct dlContact* next;
} dlContact_t;

void AddContact(dlContact_t* contact, dlContact_t** contacts);
void DestroyAllContacts(dlContact_t** contacts);