#if !defined(CORE_H)

#include <stdio.h>
#include <raylib.h>

#include "core_utils.h"

#define SNAKE_LENGTH 256
#define SQUARE_SIZE 10

typedef struct Snake
{
    Vector2 position;
    Vector2 size;
    Vector2 speed;
    Color color;
} Snake;

typedef struct Food
{
    Vector2 position;
    Vector2 size;
    bool active;
    Color color;
} Food;

typedef struct
{
    int initialized;
    int windowOpened;
    int windowWidth;
    int windowHeight;
    char *windowTitle;

    int framesCounter;
    bool gameOver;
    bool pause;

    Food fruit;
    Snake snake[SNAKE_LENGTH];
    Vector2 snakePosition[SNAKE_LENGTH];
    bool allowMove;
    Vector2 offset;
    int counterTail;

    int reloadedThisFrame;
    int reloadCount;
} CoreMemory;

#define CORE_UPDATE(name) int name(CoreMemory *memory)
typedef CORE_UPDATE(CoreUpdate);

CORE_UPDATE(coreUpdateStub)
{
    printf("Update stub!\n");
    return 0;
}

#define CORE_H
#endif
