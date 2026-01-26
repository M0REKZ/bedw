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

#ifndef GLOBALS_H
#define GLOBALS_H

#include <raylib.h>

#define GAME_WIDTH 512
#define GAME_HEIGHT 384 // 256 + 128

class CCamera
{
    public:
    Vector3 m_Pos;
    Vector3 m_Target;
};

class CGlobals
{
    public:
    CCamera m_Camera;
    Camera3D m_RaylibCamera;
    Camera2D m_RaylibCamera2D;

    int m_CurrentWindowWidth = GAME_WIDTH;
    int m_CurrentWindowHeight = GAME_HEIGHT;

    Shader m_TransparentBillboardShader;
    Font m_MainFont;

    void Init();
};

extern CGlobals g_Globals;

#endif
