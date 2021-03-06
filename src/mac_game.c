#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>
#include <dlfcn.h>

#include "core.h"

const char *coreLibPath = "./build/core.o";
const char *sourcePath = "./src/";
const char *buildCoreCommand = "./mac_build_core.sh";

typedef struct
{
    void *handle;
    CoreUpdate *update;
    bool isValid;
} MacGameCore;

MacGameCore loadCore()
{
    print("Loading core...\n");

    MacGameCore result = {0};

    result.handle = dlopen(coreLibPath, RTLD_LAZY);
    if (!result.handle)
    {
        print("Couldn't load core: %s\n", dlerror());
        return result;
    }

    result.update = (CoreUpdate *)dlsym(result.handle, "coreUpdate");
    result.isValid = (result.update != NULL);

    return result;
}

bool codeChanged(time_t lastReload)
{
    DIR *directory = opendir(sourcePath);
    struct dirent *entry;
    struct stat info;

    if (directory == NULL)
    {
        fprintf(stderr, "Could not open source directory.\n");
        return false;
    }

    while ((entry = readdir(directory)) != NULL)
    {
        char buffer[1024];
        strcpy(buffer, sourcePath);
        strcat(buffer, entry->d_name);

        if (stat(buffer, &info) == -1)
        {
            // perror(buffer);
            continue;
        }

        // print("%30s\n", ctime(&info.st_mtime));
        if (info.st_mtime > lastReload)
        {
            // print("%20s\n", file->d_name);
            return true;
        }
    }

    closedir(directory);
    return false;
}

void unloadCore(MacGameCore *core)
{
    if (core->handle)
    {
        dlclose(core->handle);
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
    MacGameCore core = loadCore();

    while (running != 0)
    {
        running = core.update(&memory);

        if (codeChanged(lastReload))
        {
            lastReload = time(0);

            system(buildCoreCommand);
            // usleep(1 * 1000);

            unloadCore(&core);
            core = loadCore();

            if (core.isValid == 0)
            {
                printf("Failed to reload core!\n");
            }
            else
            {
                memory.isInitialized = -1;
            }
        }
    }

    return 0;
}