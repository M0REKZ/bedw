/*
    Brain Evil: Dark World
    Copyright (C) Benjamín Gajardo (also known as +KZ)

    This program is shared under the PLUSKAIZO LICENSE, you should have received
    a copy of the license along with the program.
*/
#if defined(WIN32)

//This is to avoid conflicts with raylib.h, dont #include that file

#include "windows.h"
#include <Shlobj.h>
#include <Shlwapi.h>

bool GetWindowsProgramFilesPath(char *pOut, int BufferSize)
{
    if(BufferSize < MAX_PATH || !pOut)
        return false;

    return SUCCEEDED(SHGetFolderPath(NULL, CSIDL_PROGRAM_FILES, NULL, 0, pOut));
}

#endif
