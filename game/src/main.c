#include "body.h"
#include "mathf.h"
#include "raylib.h"
#include "raymath.h"
#include "world.h"

#include <stdlib.h>
#include <assert.h>

#define MAX_BODIES 100

int main(void)
{
	InitWindow(1280, 720, "Physics Engine");
	SetTargetFPS(120);

	//game loop
	while (!WindowShouldClose())
	{
		//update
		float dt = GetFrameTime();
		float fps = (float)GetFPS();



		Vector2 position = GetMousePosition();
		if (IsMouseButtonDown(0))
		{
			Body* body = CreateBody();
			body->velocity = CreateVector2(GetRandomFloatValue(-5, 5), GetRandomFloatValue(-5, 5));
		}

		//render
		BeginDrawing();
		ClearBackground(BLACK);
		DrawText(TextFormat("FPS: %.2f (%.2fms)", fps, 1000/fps), 10, 10, 16, GREEN);
		DrawText(TextFormat("FRAME: %.4f", dt), 10, 30, 16, LIME);

		DrawCircle((int)position.x, (int)position.y, 5, ORANGE);


		//update bodies
		Body* body = bodies;
		while (body) // do while we have a valid pointer, will be NULL at the end of the list
		{
			// update body position
			body->position = Vector2Add(body->position, body->velocity);
			// draw body
			DrawCircle((int)body->position.x, (int)body->position.y, 3, RED);

			body = body->next; // get next body
		}

		//DrawText("Congrats!", 190, 200, 20, MAROON);
		EndDrawing();
	}
	CloseWindow();
	free(bodies);

	return 0;
}