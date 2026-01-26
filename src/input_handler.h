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

#ifndef INPUT_HANDLER_H
#define INPUT_HANDLER_H

#include <raylib.h>

struct SInput
{
    bool m_Front = false;
    bool m_Back = false;
    bool m_Left = false;
    bool m_Right = false;
    bool m_Jump = false;
    float m_Angle = 0.f;
    float m_AngleY = 0.f;
    bool m_MouseClick = false;
    bool m_MouseRightClick = false;
    Vector2 m_MousePos = {0,0};
    bool m_Escape = false;
    bool m_Enter = false;
    bool m_BackSpace = false;
    bool m_ArrowLeft = false;
    bool m_ArrowRight = false;
    bool m_ArrowUp = false;
    bool m_ArrowDown = false;
    bool m_EditorNeighborKey = false;
    bool m_EditorEditSectorKey = false;
    bool m_EditorCeilingKey = false;
    bool m_EditorFloorKey = false;
    bool m_EditorSaveLevelKey = false;
    bool m_EditorLoadLevelKey = false;
    bool m_EditorCeilingSlopeKey = false;
    bool m_EditorFloorSlopeKey = false;
    bool m_EditorPlaceEntityKey = false;
};

class CInputHandler
{
    SInput m_Inputs;
    public:
    void UpdateInput();
    const SInput * GetInputs() { return &m_Inputs; }
    void UpdateMousePos();
};

extern CInputHandler g_InputHandler;

#endif
