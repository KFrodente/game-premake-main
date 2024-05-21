#include "editor.h"
#define RAYGUI_IMPLEMENTATION
#include "../../raygui/src/raygui.h"
#include "render.h"

#define EDITOR_DATA(data) TextFormat("%0.2f", data), &data
bool dlEditorActive = true;
bool dlEditorIntersect = false;

dlEditorData_t dlEditorData;

Rectangle editorRect;

Vector2 anchor01 = { 1600, 50 };
Texture2D cursorTexture;

bool EditorBoxActive = true;


void InitEditor()
{
    GuiLoadStyle("raygui/styles/lavanda/style_lavanda.rgs");

    Image image = LoadImage("resources/reticle.png");
    cursorTexture = LoadTextureFromImage(image);
    UnloadImage(image);

    dlEditorData.anchor01 = (Vector2){ 1600, 50 };

    dlEditorData.WindowBox000Active = true;
    dlEditorData.DropdownBox002EditMode = false;
    dlEditorData.BodyType = 0;
    dlEditorData.MassValue = 0.0f;
    dlEditorData.DampingValue = 0.0f;
    dlEditorData.GravityScale = 0.0f;
    dlEditorData.StiffnessValue = 0.0f;
    dlEditorData.RestitutionValue = 0.0f;
    dlEditorData.GravityValue = 0.0f;
    dlEditorData.GravitationValue = 0.0f;
    dlEditorData.TimestepValue = 0.0f;
    dlEditorData.SimulateActive = true;
    dlEditorData.ResetPressed = false;

    editorRect = (Rectangle){ dlEditorData.anchor01.x + 0, dlEditorData.anchor01.y + 0, 300, 600 };

    HideCursor();
}

void UpdateEditor(Vector2 position)
{
    //
}

void DrawEditor(Vector2 position)
{
    if (dlEditorData.DropdownBox002EditMode) GuiLock();

    if (dlEditorData.WindowBox000Active)
    {
        dlEditorData.WindowBox000Active = !GuiWindowBox((Rectangle){ dlEditorData.anchor01.x + 0, dlEditorData.anchor01.y + 0, 248, 416 }, "Editor");
        GuiGroupBox((Rectangle){ dlEditorData.anchor01.x + 8, dlEditorData.anchor01.y + 40, 232, 160 }, "Body");
        GuiSliderBar((Rectangle){ dlEditorData.anchor01.x + 88, dlEditorData.anchor01.y + 80, 120, 16 }, "Mass", EDITOR_DATA(dlEditorData.MassValue), 0, 20);
        GuiSliderBar((Rectangle){ dlEditorData.anchor01.x + 88, dlEditorData.anchor01.y + 104, 120, 16 }, "Damping", EDITOR_DATA(dlEditorData.DampingValue), 0, 1);
        GuiSliderBar((Rectangle){ dlEditorData.anchor01.x + 88, dlEditorData.anchor01.y + 128, 120, 16 }, "GravityScale", EDITOR_DATA(dlEditorData.GravityScale), 0, 10);
        GuiSliderBar((Rectangle){ dlEditorData.anchor01.x + 88, dlEditorData.anchor01.y + 152, 120, 16 }, "Stiffness(k)", EDITOR_DATA(dlEditorData.StiffnessValue), 0, 10);
        GuiSliderBar((Rectangle){ dlEditorData.anchor01.x + 88, dlEditorData.anchor01.y + 176, 120, 16 }, "Restitution", EDITOR_DATA(dlEditorData.RestitutionValue), 0, 1);
        GuiGroupBox((Rectangle){ dlEditorData.anchor01.x + 8, dlEditorData.anchor01.y + 216, 232, 152 }, "World");
        GuiSlider((Rectangle){ dlEditorData.anchor01.x + 88, dlEditorData.anchor01.y + 232, 120, 16 }, "Gravity", EDITOR_DATA(dlEditorData.GravityValue), -15, 15);
        GuiSlider((Rectangle){ dlEditorData.anchor01.x + 88, dlEditorData.anchor01.y + 256, 120, 16 }, "Gravitation", EDITOR_DATA(dlEditorData.GravitationValue), -15, 15);
        GuiSliderBar((Rectangle){ dlEditorData.anchor01.x + 88, dlEditorData.anchor01.y + 280, 120, 16 }, "TimeStep", EDITOR_DATA(dlEditorData.TimestepValue), 0, 60);
        GuiToggle((Rectangle){ dlEditorData.anchor01.x + 136, dlEditorData.anchor01.y + 376, 88, 24 }, "Simulate", &dlEditorData.SimulateActive);
        dlEditorData.ResetPressed = GuiButton((Rectangle){ dlEditorData.anchor01.x + 24, dlEditorData.anchor01.y + 376, 88, 24 }, "Reset"); 
        if (GuiDropdownBox((Rectangle){ dlEditorData.anchor01.x + 16, dlEditorData.anchor01.y + 48, 216, 24 }, "Dynamic;Kinematic;Static", &dlEditorData.BodyType, dlEditorData.DropdownBox002EditMode)) dlEditorData.DropdownBox002EditMode = !dlEditorData.DropdownBox002EditMode;
    }

    DrawTexture(cursorTexture, (int)position.x - cursorTexture.width * 0.5f, (int)position.y - cursorTexture.height * 0.5f, WHITE);
    GuiUnlock();
}


dlBody* GetBodyIntersect(dlBody* bodies, Vector2 position)
{
    for (dlBody* body = bodies; body; body = body->next)
    {
        Vector2 screen = ConvertWorldToScreen(body->position);
        if (CheckCollisionPointCircle(position, screen, ConvertWorldToPixel(body->mass)))
        {
            return body;
        }
    }

    return NULL;
}

void DrawLineBodyToPosition(dlBody* body, Vector2 position)
{
    Vector2 screen = ConvertWorldToScreen(body->position);
    DrawLine((int)screen.x, (int)screen.y, (int)position.x - cursorTexture.width / 2, (int)position.y - cursorTexture.height / 2, YELLOW);
}
