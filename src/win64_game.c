#include <WinDef.h>
#include <libloaderapi.h>
#include <winbase.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <time.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h>
#include <unistd.h>

#include "core.h"

const char *coreLibPath = "./build/core.o";
const char *coreLibCopyPath = "./build/core-copy.o";
const char *sourcePath = "./src/";
const char *buildCoreCommand = "win64_build_core.bat";

typedef struct
{
    HMODULE handle;
    CoreUpdate *update;
    bool isValid;
} Win64CoreCode;

Win64CoreCode loadCore()
{
    print("Loading core...\n");

    Win64CoreCode result = {};

    // Copy the dll just in case it's loaded by something else
    CopyFile(coreLibPath, coreLibCopyPath, FALSE);

    result.handle = LoadLibraryA(coreLibCopyPath);
    if (result.handle == 0)
    {
        fprintf(stderr, "Error while loading game library.\n");
        return result;
    }

    result.update = (CoreUpdate *)GetProcAddress(result.handle, "coreUpdate");
    result.isValid = (result.update != NULL);

    return result;
}

bool codeChanged(time_t lastReload)
{
    struct dirent *file;
    struct stat info;

    DIR *directory = opendir(sourcePath);
    if (directory == 0)
    {
        perror("The directory couldn't be accessed or does not exist.\n");
        return false;
    }

    while ((file = readdir(directory)))
    {
        char buffer[1024];
        strcpy(buffer, sourcePath);
        strcat(buffer, file->d_name);
        if (stat(buffer, &info) == -1)
        {
            perror(buffer);
            continue;
        }

        // print("%30s\n", ctime(&info.st_mtime));
        if (info.st_mtime > lastReload)
        {
            // print("%20s\n", file->d_name);
            return true;
        }
    }

    return false;
}

void unloadCore(Win64CoreCode *core)
{
    if (core->handle)
    {
        FreeLibrary(core->handle);
        core->handle = NULL;
    }

    core->update = coreUpdateStub;
    core->isValid = false;
}

int main(void)
{
    int running = 1;
    time_t lastReload = time(0);
    CoreMemory memory = {0};
    memory.reloadedThisFrame = 1;
    memory.color = BLUE;
    Win64CoreCode core = loadCore();

    while (running != 0)
    {
        running = core.update(&memory);

        if (codeChanged(lastReload))
        {
            lastReload = time(0);

            system(buildCoreCommand);

            unloadCore(&core);
            // usleep(1000 * 1000);
            core = loadCore();

            if (core.isValid == 0)
            {
                printf("Failed to reload core!\n");
            }
            else
            {
                // CloseWindow();

                memory.reloadedThisFrame = 1;
                memory.reloadCount += 1;

                memory.color = (memory.reloadCount % 2) ? GREEN : RED;
            }
        }
    }

    return 0;
}