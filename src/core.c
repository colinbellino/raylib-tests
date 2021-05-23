#include <raylib.h>

#include "core.h"

extern CORE_UPDATE(coreUpdate)
{
    if (memory->isInitialized <= 0)
    {
        if (memory->isInitialized == 0)
        {
            memory->windowWidth = 800;
            memory->windowHeight = 450;
            memory->windowTitle = "Game";
            memory->color = BLACK;
        }
        else
        {
            // memory->windowWidth += 100;
            // memory->windowHeight += 100;
            memory->color = (memory->reloadCount % 2) ? GREEN : RED;
            memory->reloadCount += 1;

            CloseWindow();
        }

        InitWindow(memory->windowWidth, memory->windowHeight, memory->windowTitle);
        // SetWindowState(FLAG_WINDOW_TOPMOST | FLAG_WINDOW_TRANSPARENT);
        SetTargetFPS(60);

        memory->isInitialized = 1;
    }

    BeginDrawing();
    ClearBackground(memory->color);
    DrawText(FormatText("Reload count: %i", memory->reloadCount), 50, 50, 24, WHITE);
    EndDrawing();

    return !WindowShouldClose();
}