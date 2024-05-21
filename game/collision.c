#include "collision.h"
#include "contact.h"
#include "body.h"
#include "mathf.h"
#include "raymath.h"

#include <stdlib.h>
#include <assert.h>
#include <string.h>

bool Intersects(dlBody* body1, dlBody* body2)
{
	float distance = Vector2Distance(body1->position, body2->position);
	float radius = body1->mass + body2->mass;

	return (distance <= radius);
}

void CreateContacts(dlBody* bodies, dlContact_t** contacts)
{
	for (dlBody* body1 = bodies; body1; body1 = body1->next)
	{
		for (dlBody* body2 = body1->next; body2; body2 = body2->next)
		{
			if (body1 == body2) continue;
			if (body1->type != BT_DYNAMIC && body2->type != BT_DYNAMIC) continue;

			if (Intersects(body1, body2))
			{
				dlContact_t* contact = GenerateContact(body1, body2);
				AddContact(contact, contacts);
			}
		}
	}
}

dlContact_t* GenerateContact(dlBody* body1, dlBody* body2)
{
	dlContact_t* contact = (dlContact_t*)malloc(sizeof(dlContact_t));
	assert(contact);

	memset(contact, 0, sizeof(dlContact_t));

	contact->body1 = body1;
	contact->body2 = body2;

	Vector2 direction = Vector2Subtract(body1->position, body2->position);
	float distance = Vector2Length(direction);
	if (distance == 0)
	{
		direction = (Vector2){ GetRandomFloatValue(-0.05f, 0.05f), GetRandomFloatValue(-0.05f, 0.05f) };
	}

	float radius = (body1->mass + body2->mass);

	contact->depth = radius - distance;
	contact->normal = Vector2Normalize(direction);
	contact->restitution = (body1->restitution + body2->restitution) * 0.5f;

	return contact;
}


void SeparateContacts(dlContact_t* contacts)
{
	// Iterate through each contact in the contacts array
	for (dlContact_t* contact = contacts; contact; contact = contact->next)
	{
		// Calculate the total inverse mass of both bodies involved in the contact
		float totalInverseMass = contact->body1->inverseMass + contact->body2->inverseMass;

		// Calculate the separation distance along the contact normal, scaled by the depth of the contact and the total inverse mass
		Vector2 separation = Vector2Scale(contact->normal, contact->depth / totalInverseMass);

		// Update the position of body1 by adding the scaled separation vector, adjusted by its own inverse mass
		contact->body1->position = Vector2Add(contact->body1->position, Vector2Scale(separation, contact->body1->inverseMass));

		// Update the position of body2 by subtracting the scaled separation vector, adjusted by its own inverse mass
		contact->body2->position = Vector2Add(contact->body2->position, Vector2Scale(separation, -contact->body2->inverseMass));
	}
}

void ResolveContacts(dlContact_t* contacts)
{
	// Iterate through each contact in the contacts array
	for (dlContact_t* contact = contacts; contact; contact = contact->next)
	{
		// Calculate the relative velocity vector between body1 and body2 of the contact
		Vector2 rv = Vector2Subtract(contact->body1->velocity, contact->body2->velocity);

		// Calculate the dot product of the relative velocity vector and the normal vector of the contact
		float nv = Vector2DotProduct(rv, contact->normal);

		// If the dot product is greater than zero, skip this iteration because the bodies are moving away from each other
		if (nv > 0) continue;

		// Calculate the total inverse mass of both bodies involved in the contact
		float totalInverseMass = contact->body1->inverseMass + contact->body2->inverseMass;

		// Calculate the magnitude of the impulse needed based on the restitution property of the contact and the dot product
		float impulseMagnitude = -(1 + contact->restitution) * nv / totalInverseMass;

		// Create the impulse vector scaled by the calculated magnitude and aligned with the normal vector of the contact
		Vector2 impulse = Vector2Scale(contact->normal, impulseMagnitude);

		// Apply the impulse force to body1
		ApplyForce(contact->body1, impulse, FM_IMPULSE);

		// Apply the opposite impulse force to body2
		ApplyForce(contact->body2, Vector2Negate(impulse), FM_IMPULSE);
	}
}