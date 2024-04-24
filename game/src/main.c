#include "raylib.h"

int main(void)
{
	InitWindow(800, 450, "raylib [core] example - basic window");

	while (!WindowShouldClose())
	{
		BeginDrawing();
		ClearBackground(RAYWHITE);
		DrawText("Congrats!", 190, 200, 20, MAROON);
		EndDrawing();
	}
	CloseWindow();
	return 0;
}