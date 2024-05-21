#pragma once
#include "raylib.h"
#include "body.h"

typedef struct dlEditorData
{
    Vector2 anchor01;

    bool WindowBox000Active;
    bool DropdownBox002EditMode;
    int BodyType;
    float MassValue;
    float DampingValue;
    float GravityScale;
    float StiffnessValue;
    float RestitutionValue;
    float GravityValue;
    float GravitationValue;
    float TimestepValue;
    bool SimulateActive;
    bool ResetPressed;

} dlEditorData_t;

extern dlEditorData_t dlEditorData;

extern bool dlEditorActive;
extern bool dlEditorIntersect;

void InitEditor();
void UpdateEditor(Vector2 mousePosition);
void DrawEditor(Vector2 position);

struct dlBody* GetBodyIntersect(struct dlBody* bodies, Vector2 position);
void DrawLineBodyToPosition(struct dlBody* body, Vector2 position);