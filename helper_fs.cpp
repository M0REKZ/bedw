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
    #if defined(__APPLE__)
    snprintf(path, sizeof(path), "%s/Library/Application Support/BEDW", home);
    #elif defined(__linux__)
    snprintf(path, sizeof(path), "%s/.local/share/BEDW", home);
    #else
    #error unsupported platform
    #endif
    return path;
}
