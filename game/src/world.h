#pragma once
#include "raylib.h"
#include "body.h"

typedef struct dlBody dlBody;

extern dlBody* dlBodies;
extern int dlBodyCount;
extern Vector2 dlGravity;


dlBody* CreateBody(Vector2 position, float mass, dlBodyType bodyType);
void AddBody(dlBody* body);
void DestroyBody(dlBody* body);