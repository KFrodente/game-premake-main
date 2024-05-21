// Include necessary header files
#include "render.h"
#include "raymath.h"

// Define screen size, zoom level, and view size
Vector2 dlScreenSize = { 1280, 720 };
float dlScreenZoom = 1.0f;
float dlViewSize = 25.0f;

// Function to convert screen coordinates to world coordinates
Vector2 ConvertScreenToWorld(Vector2 screen)
{
    // Calculate ratio of screen width to screen height
    float ratio = dlScreenSize.x / dlScreenSize.y;

    // Calculate extents of the view in world coordinates
    Vector2 extents = { dlViewSize * ratio, dlViewSize };
    extents = Vector2Scale(extents, dlScreenZoom);

    // Calculate lower and upper bounds of the view in world coordinates
    Vector2 lower = Vector2Negate(extents);
    Vector2 upper = extents;

    // Calculate normalized coordinates (0 to 1) of the screen point
    float nx = screen.x / dlScreenSize.x;
    float ny = (dlScreenSize.y - screen.y) / dlScreenSize.y;

    // Map normalized coordinates to world coordinates
    Vector2 world;
    world.x = ((1 - nx) * lower.x) + (nx * upper.x);
    world.y = ((1 - ny) * lower.y) + (ny * upper.y);

    return world;
}

// Function to convert world coordinates to screen coordinates
Vector2 ConvertWorldToScreen(Vector2 world)
{
    float ratio = dlScreenSize.x / dlScreenSize.y;
    Vector2 extents = { dlViewSize * ratio, dlViewSize };
    extents = Vector2Scale(extents, dlScreenZoom);

    Vector2 lower = Vector2Negate(extents);
    Vector2 upper = extents;

    float nx = (world.x - lower.x) / (upper.x - lower.x);
    float ny = (world.y - lower.y) / (upper.y - lower.y);

    Vector2 screen;
    screen.x = nx * dlScreenSize.x;
    screen.y = (1.0f - ny) * dlScreenSize.y;

    return screen;
}

// Function to convert world coordinates to pixel coordinates
float ConvertWorldToPixel(float world)
{
    float ratio = dlScreenSize.x / dlScreenSize.y;
    Vector2 extents = { dlViewSize * ratio, dlViewSize };
    extents = Vector2Scale(extents, dlScreenZoom);

    float nx = world / extents.x;
    float pixel = nx * dlScreenSize.x;

    return pixel;
}