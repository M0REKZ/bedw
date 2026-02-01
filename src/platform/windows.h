/*
    Brain Evil: Dark World
    Copyright (C) Benjamín Gajardo (also known as +KZ)

    This program is shared under the PLUSKAIZO LICENSE, you should have received
    a copy of the license along with the program.
*/
#ifndef BEDW_WINDOWS_H
#define BEDW_WINDOWS_H

#if defined(WIN32)

//This is to avoid conflicts with raylib.h, dont #include that file

bool GetWindowsProgramFilesPath(char * pOut, int BufferSize);

#endif

#endif
