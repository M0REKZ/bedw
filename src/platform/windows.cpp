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
