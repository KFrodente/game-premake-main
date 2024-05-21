#include "mathf.h"
#include "raylib.h"
#include "raymath.h"
#include "world.h"
#include "integrator.h"
#include "force.h"
#include "render.h"
#include "editor.h"
#include "spring.h"
#include "collision.h"
#include "contact.h"

#include <stdlib.h>
#include <assert.h>

#define MAX_BODIES 100

int main(void)
{
	dlBody* selectedBody = NULL;
	dlBody* connectBody = NULL;
	dlContact_t* contacts = NULL;

		float fixedTimestep = 1.0f / 50;//dlEditorData.TimestepValue;
		float timeAccumulator = 0;

	InitWindow(1920, 1080, "Physics Engine");
	InitEditor();
	SetTargetFPS(60);

	//initialize world
	dlGravity = (Vector2){ 0, -1 };


	//game loop
	while (!WindowShouldClose())
	{
		fixedTimestep = 1.0f / dlEditorData.TimestepValue;

		float dt = GetFrameTime();
		float fps = (float)GetFPS();
		dlGravity = (Vector2){ 0, dlEditorData.GravityValue };

		Vector2 position = GetMousePosition();
		dlScreenZoom += GetMouseWheelMove() * 0.2f;
		dlScreenZoom = Clamp(dlScreenZoom, 0.1, 10);

		UpdateEditor(position);

		if (dlEditorData.ResetPressed)
		{
			DestroyAllBodies();
			DestroyAllSprings();
		}


		selectedBody = GetBodyIntersect(dlBodies, position);
		if (selectedBody)
		{
			Vector2 screen = ConvertWorldToScreen(selectedBody->position);
			DrawCircleLines(screen.x, screen.y, ConvertWorldToPixel(selectedBody->mass * 0.5f) + 5, YELLOW);
		}

		if (!dlEditorIntersect)
		{
			// Create body
			if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) || (IsMouseButtonDown(MOUSE_BUTTON_LEFT) && IsKeyDown(KEY_LEFT_CONTROL)))
			{
				int bodyCount = GetRandomValue(100, 200);
				bodyCount = 1;
				for (int i = 0; i < bodyCount; i++)
				{
					dlBody* body = CreateBody(ConvertScreenToWorld(position), dlEditorData.MassValue, dlEditorData.BodyType);
					body->damping = dlEditorData.DampingValue;
					body->gravityScale = dlEditorData.GravityScale;
					body->HSV = (Vector3){ GetRandomFloatValue(1, 255), GetRandomFloatValue(200, 255) , 1.0f };

					body->restitution = dlEditorData.RestitutionValue;
					AddBody(body);
				}
			}

			// connect spring
			if (IsMouseButtonPressed(MOUSE_BUTTON_RIGHT) && selectedBody)
			{
				connectBody = selectedBody;
			}
			if (IsMouseButtonDown(MOUSE_BUTTON_RIGHT) && connectBody)
			{
				DrawLineBodyToPosition(connectBody, position);
				if (IsKeyDown(KEY_LEFT_SHIFT) && dlEditorData.SimulateActive)
				{
					Vector2 direction = Vector2Subtract(ConvertScreenToWorld(position), connectBody->position);
					if (direction.x != 0 || direction.y != 0)
					{

						float length = Vector2Length(direction);
						float x = length - 1;
						float force = x * dlEditorData.StiffnessValue;

						Vector2 ndirection = Vector2Normalize(direction);

						ApplyForce(connectBody, Vector2Scale(ndirection, force), FM_FORCE);
						connectBody->velocity =  Vector2Scale(connectBody->velocity, 0.97f);

					}
				}
			}
			if (IsMouseButtonReleased(MOUSE_BUTTON_RIGHT) && connectBody)
			{
				if (selectedBody && selectedBody != connectBody)
				{
					dlSpring_t* spring = CreateSpring(selectedBody, connectBody, Vector2Distance(selectedBody->position, connectBody->position), 5);
					AddSpring(spring);
				}
			}
		}





		if (dlEditorData.SimulateActive)
		{
			timeAccumulator += dt;

			while (timeAccumulator >= fixedTimestep)
			{
				timeAccumulator -= fixedTimestep;
				// update bodies
				for (dlBody* body = dlBodies; body; body = body->next)
				{
					Step(body, fixedTimestep);
				}
				ApplyGravitation(dlBodies,  dlEditorData.GravitationValue);
				ApplySpringForce(dlSprings);
				// collision
				contacts = NULL;
				CreateContacts(dlBodies, &contacts);
				SeparateContacts(contacts);
				ResolveContacts(contacts);
			}
		}
		// render
		BeginDrawing();
		ClearBackground(BLACK);



		// Stats
		DrawText(TextFormat("FPS: %.2f (%.2fms)", fps, fps / 1000), 10, 10, 20, LIME);
		DrawText(TextFormat("FRAME: %.2f", dt), 10, 30, 20, LIME);


		//DrawCircle(position.x, position.y, 10, RED);

		// draw bodies
		for (dlBody* body = dlBodies; body; body = body->next)
		{
			Vector2 screen = ConvertWorldToScreen(body->position);
			DrawCircle(screen.x, screen.y, ConvertWorldToPixel(body->mass * 0.5f), ColorFromHSV(body->HSV.x, body->HSV.y, body->HSV.z));
		}
		// draw springs
		for (dlSpring_t* spring = dlSprings; spring; spring = spring->next)
		{
			Vector2 screen1 = ConvertWorldToScreen(spring->body1->position);
			Vector2 screen2 = ConvertWorldToScreen(spring->body2->position);
			DrawLine(screen1.x, screen1.y, screen2.x, screen2.y, YELLOW);
		}
		// draw contacts
		for (dlContact_t* contact = contacts; contact; contact = contact->next)
		{
			Vector2 screen = ConvertWorldToScreen(contact->body1->position);
			DrawCircle(screen.x, screen.y, ConvertWorldToPixel(contact->body1->mass * 0.5f), RED);
		}

		DrawEditor(position);

		EndDrawing();
	}
	CloseWindow();

	return 0;
}