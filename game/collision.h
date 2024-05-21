#pragma once
#include "raylib.h"

typedef struct dlContact dlContact_t;
typedef struct dlBody dlBody;

void CreateContacts(dlBody* bodies, dlContact_t** contacts);
dlContact_t* GenerateContact(dlBody* body1, dlBody* body2);

void SeparateContacts(dlContact_t* contacts);
void ResolveContacts(dlContact_t* contacts);