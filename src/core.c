#include <raylib.h>

#include "core.h"

void ResetState(CoreMemory *memory)
{
    memory->framesCounter = 0;
    memory->gameOver = false;
    memory->pause = false;

    memory->fruit = (Food){0};
    memory->fruit.color = BLUE;

    memory->counterTail = 1;
    memory->allowMove = false;

    memory->offset = (Vector2){0};
    memory->offset.x = memory->windowWidth % SQUARE_SIZE;
    memory->offset.y = memory->windowHeight % SQUARE_SIZE;

    for (int i = 0; i < SNAKE_LENGTH; i++)
    {
        memory->snake[i].position = (Vector2){memory->offset.x / 2, memory->offset.y / 2};
        memory->snake[i].size = (Vector2){SQUARE_SIZE, SQUARE_SIZE};
        memory->snake[i].speed = (Vector2){SQUARE_SIZE, 0};

        if (i == 0)
        {
            memory->snake[i].color = DARKBLUE;
        }
        else
        {
            memory->snake[i].color = BLUE;
        }
    }

    for (int i = 0; i < SNAKE_LENGTH; i++)
    {
        memory->snakePosition[i] = (Vector2){0.0f, 0.0f};
    }

    memory->fruit.size = (Vector2){SQUARE_SIZE, SQUARE_SIZE};
    memory->fruit.active = false;
}

extern CORE_UPDATE(coreUpdate)
{
    if (memory->initialized == 0)
    {
        memory->windowWidth = 800;
        memory->windowHeight = 450;
        memory->windowTitle = "Game";
        memory->initialized = 1;
        memory->fruit.color = BLUE;

        ResetState(memory);
    }

    if (memory->reloadedThisFrame)
    {
        if (memory->windowOpened == 1)
        {
            CloseWindow();
        }

        InitWindow(memory->windowWidth, memory->windowHeight, memory->windowTitle);
        SetTargetFPS(60);

        memory->reloadedThisFrame = 0;
        memory->windowOpened = 1;
    }

    // Update ------------------------------------
    if (!memory->gameOver)
    {
        if (IsKeyPressed('P'))
        {
            memory->pause = !memory->pause;
        }

        if (!memory->pause)
        {
            // Player control
            if (IsKeyPressed(KEY_RIGHT) && (memory->snake[0].speed.x == 0) && memory->allowMove)
            {
                memory->snake[0].speed = (Vector2){SQUARE_SIZE, 0};
                memory->allowMove = false;
            }
            if (IsKeyPressed(KEY_LEFT) && (memory->snake[0].speed.x == 0) && memory->allowMove)
            {
                memory->snake[0].speed = (Vector2){-SQUARE_SIZE, 0};
                memory->allowMove = false;
            }
            if (IsKeyPressed(KEY_UP) && (memory->snake[0].speed.y == 0) && memory->allowMove)
            {
                memory->snake[0].speed = (Vector2){0, -SQUARE_SIZE};
                memory->allowMove = false;
            }
            if (IsKeyPressed(KEY_DOWN) && (memory->snake[0].speed.y == 0) && memory->allowMove)
            {
                memory->snake[0].speed = (Vector2){0, SQUARE_SIZE};
                memory->allowMove = false;
            }

            // Snake movement
            for (int i = 0; i < memory->counterTail; i++)
            {
                memory->snakePosition[i] = memory->snake[i].position;
            }

            if ((memory->framesCounter % 5) == 0)
            {
                for (int i = 0; i < memory->counterTail; i++)
                {
                    if (i == 0)
                    {
                        memory->snake[0].position.x += memory->snake[0].speed.x;
                        memory->snake[0].position.y += memory->snake[0].speed.y;
                        memory->allowMove = true;
                    }
                    else
                    {
                        memory->snake[i].position = memory->snakePosition[i - 1];
                    }
                }
            }

            // Wall behaviour
            if (((memory->snake[0].position.x) > (memory->windowWidth - memory->offset.x)) ||
                ((memory->snake[0].position.y) > (memory->windowHeight - memory->offset.y)) ||
                (memory->snake[0].position.x < 0) || (memory->snake[0].position.y < 0))
            {
                memory->gameOver = true;
            }

            // Collision with yourself
            for (int i = 1; i < memory->counterTail; i++)
            {
                if ((memory->snake[0].position.x == memory->snake[i].position.x) && (memory->snake[0].position.y == memory->snake[i].position.y))
                {
                    memory->gameOver = true;
                }
            }

            // Fruit position calculation
            if (!memory->fruit.active)
            {
                memory->fruit.active = true;
                memory->fruit.position = (Vector2){GetRandomValue(0, (memory->windowWidth / SQUARE_SIZE) - 1) * SQUARE_SIZE + memory->offset.x / 2, GetRandomValue(0, (memory->windowHeight / SQUARE_SIZE) - 1) * SQUARE_SIZE + memory->offset.y / 2};

                for (int i = 0; i < memory->counterTail; i++)
                {
                    while ((memory->fruit.position.x == memory->snake[i].position.x) && (memory->fruit.position.y == memory->snake[i].position.y))
                    {
                        memory->fruit.position = (Vector2){GetRandomValue(0, (memory->windowWidth / SQUARE_SIZE) - 1) * SQUARE_SIZE + memory->offset.x / 2, GetRandomValue(0, (memory->windowHeight / SQUARE_SIZE) - 1) * SQUARE_SIZE + memory->offset.y / 2};
                        i = 0;
                    }
                }
            }

            // Collision
            if ((memory->snake[0].position.x < (memory->fruit.position.x + memory->fruit.size.x) && (memory->snake[0].position.x + memory->snake[0].size.x) > memory->fruit.position.x) &&
                (memory->snake[0].position.y < (memory->fruit.position.y + memory->fruit.size.y) && (memory->snake[0].position.y + memory->snake[0].size.y) > memory->fruit.position.y))
            {
                memory->snake[memory->counterTail].position = memory->snakePosition[memory->counterTail - 1];
                memory->counterTail += 1;
                memory->fruit.active = false;
            }

            memory->framesCounter++;
        }
    }
    else
    {
        if (IsKeyPressed(KEY_ENTER))
        {
            ResetState(memory);
            memory->gameOver = false;
        }
    }

    // Draw -------------------------------------
    BeginDrawing();

    ClearBackground(RAYWHITE);

    DrawText(FormatText("Reload count: %i", memory->reloadCount), 50, 50, 24, BLACK);

    if (!memory->gameOver)
    {
        // Draw grid lines
        for (int i = 0; i < memory->windowWidth / SQUARE_SIZE + 1; i++)
        {
            DrawLineV((Vector2){SQUARE_SIZE * i + memory->offset.x / 2, memory->offset.y / 2}, (Vector2){SQUARE_SIZE * i + memory->offset.x / 2, memory->windowHeight - memory->offset.y / 2}, LIGHTGRAY);
        }

        for (int i = 0; i < memory->windowHeight / SQUARE_SIZE + 1; i++)
        {
            DrawLineV((Vector2){memory->offset.x / 2, SQUARE_SIZE * i + memory->offset.y / 2}, (Vector2){memory->windowWidth - memory->offset.x / 2, SQUARE_SIZE * i + memory->offset.y / 2}, LIGHTGRAY);
        }

        // Draw snake
        for (int i = 0; i < memory->counterTail; i++)
        {
            DrawRectangleV(memory->snake[i].position, memory->snake[i].size, memory->snake[i].color);
        }

        // Draw fruit to pick
        DrawRectangleV(memory->fruit.position, memory->fruit.size, memory->fruit.color);

        if (memory->pause)
        {
            DrawText("GAME PAUSED", memory->windowWidth / 2 - MeasureText("GAME PAUSED", 40) / 2, memory->windowHeight / 2 - 40, 40, GRAY);
        }
    }
    else
    {
        DrawText("PRESS [ENTER] TO PLAY AGAIN", GetScreenWidth() / 2 - MeasureText("PRESS [ENTER] TO PLAY AGAIN", 20) / 2, GetScreenHeight() / 2 - 50, 20, GRAY);
    }

    EndDrawing();

    return !WindowShouldClose();
}