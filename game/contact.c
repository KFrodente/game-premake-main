#include "contact.h"

#include <stdlib.h>
#include <assert.h>
#include <string.h>

void AddContact(dlContact_t* contact, dlContact_t** contacts)
{
	assert(contact);

	// add element to linked list
	contact->next = *contacts;

	// set head of elements to new element
	*contacts = contact;
}

void DestroyAllContacts(dlContact_t** contacts)
{
	if (!*contacts) return;

	dlContact_t* contact = *contacts;
	while (contact)
	{
		dlContact_t* next = contact->next;
		free(contact);
		contact = next;
	}

	*contacts = NULL;
}