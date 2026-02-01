/*
    Brain Evil: Dark World
    Copyright (C) Benjamín Gajardo (also known as +KZ)

    This program is shared under the PLUSKAIZO LICENSE, you should have received
    a copy of the license along with the program.
*/
#include "helper_fs.h"
#include <raylib.h>
#include <cstdio>
#include <cstdlib>

#if defined(__APPLE__)
    #include <limits.h>
#elif defined(__linux__)
    #include <limits.h>
#elif defined(WIN32)
    #include <platform/windows.h>
#else
    #error unsupported platform
#endif

bool CreateSavePath()
{
    const char * pPath = GetSavePath();

    if(!pPath)
        return false;

    if(!DirectoryExists(pPath))
    {
        printf("Create %s %d\n", pPath, MakeDirectory(pPath));
    }
    else
    {
        printf("EXISTS\n");
    }
    return true;
}

const char *GetSavePath()
{
    #if defined(__APPLE__)

    static char path[PATH_MAX] = {'\0'};

    const char * home = getenv("HOME"); //TODO: dont use HOME
    if(!home)
        return nullptr;

    snprintf(path, sizeof(path), "%s/Library/Application Support/BEDW", home);

    #elif defined(__linux__)

    static char path[PATH_MAX] = {'\0'};

    const char * home = getenv("HOME"); //TODO: dont use HOME
    if(!home)
        return nullptr;

    snprintf(path, sizeof(path), "%s/.local/share/BEDW", home);

    #elif defined(WIN32)

    // MAX_PATH on windows is 260, im using that number directly instead of including MAX_PATH header
    // since including both raylib.h and windows.h in a same file has conflicts............ :/
    // anyways, MAX_PATH limit is checked in platform/windows.cpp file
    static char path[260] = {'\0'};
    char home[260] = {'\0'};
    if(GetWindowsProgramFilesPath(home, 260)) 
    {
        snprintf(path, sizeof(path), "%s\\BEDW", home);
    }
    else
    {
        return nullptr;
    }

    #endif

    return path;
}
