/*
    Brain Evil: Dark World
    Copyright (C) Benjamín Gajardo (also known as +KZ)

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <https://www.gnu.org/licenses/>.
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

    static char path[MAX_PATH] = {'\0'};
    char home[PATH_MAX] = {'\0'};
    if(GetProgramFilesPath(home, MAX_PATH)) 
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
