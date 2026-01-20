// Copyright (c) Benjamín Gajardo (also known as +KZ)
#include "helper_fs.h"
#include <raylib.h>
#include <cstdio>
#include <cstdlib>

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
    static char path[1024] = {'\0'};

    const char * home = getenv("HOME");
    if(!home)
        return nullptr;
    snprintf(path, sizeof(path), "%s/Library/Application Support/BEDW", home);

    return path;
}
