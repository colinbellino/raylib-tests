#if !defined(CORE_H)

#include <stdio.h>
#include <raylib.h>

#include "core_utils.h"

typedef struct
{
    int isInitialized;
    int windowWidth;
    int windowHeight;
    char *windowTitle;
    Color color;
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
