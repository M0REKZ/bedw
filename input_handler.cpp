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

#include "input_handler.h"
#include "globals.h"
#include <raylib.h>
#include <cmath>
#include <cstdio>
#include <config_handler.h>

CInputHandler g_InputHandler;

void CInputHandler::UpdateInput()
{
    if(IsKeyDown(g_ConfigHandler.m_Config.m_KeyFront))
    {
        m_Inputs.m_Front = true;
    }
    else
    {
        m_Inputs.m_Front = false;
    }

    if(IsKeyDown(g_ConfigHandler.m_Config.m_KeyLeft))
    {
        m_Inputs.m_Left = true;
    }
    else
    {
        m_Inputs.m_Left = false;
    }

    if(IsKeyDown(g_ConfigHandler.m_Config.m_KeyBack))
    {
        m_Inputs.m_Back = true;
    }
    else
    {
        m_Inputs.m_Back = false;
    }

    if(IsKeyDown(g_ConfigHandler.m_Config.m_KeyRight))
    {
        m_Inputs.m_Right = true;
    }
    else
    {
        m_Inputs.m_Right = false;
    }

    if(IsKeyDown(g_ConfigHandler.m_Config.m_KeyJump))
    {
        m_Inputs.m_Jump = true;
    }
    else
    {
        m_Inputs.m_Jump = false;
    }

    if(IsKeyDown(KEY_LEFT))
    {
        m_Inputs.m_ArrowLeft = true;
        //m_Inputs.m_Angle -= 0.1f;
    }
    else
    {
        m_Inputs.m_ArrowLeft = false;
    }

    if(IsKeyDown(KEY_RIGHT))
    {
        m_Inputs.m_ArrowRight = true;
        //m_Inputs.m_Angle += 0.1f;
    }
    else
    {
        m_Inputs.m_ArrowRight = false;
    }

    Vector2 PrevMousePos = m_Inputs.m_MousePos;
    UpdateMousePos();

    if((PrevMousePos.x != m_Inputs.m_MousePos.x || PrevMousePos.y != m_Inputs.m_MousePos.y) &&
        PrevMousePos.x != 0.f && PrevMousePos.y != 0.f)
    {
        float dx = m_Inputs.m_MousePos.x - PrevMousePos.x;
        float dy = m_Inputs.m_MousePos.y - PrevMousePos.y;

        m_Inputs.m_Angle += dx * (M_PI/180);
        m_Inputs.m_AngleY += dy * (M_PI/180);

        if (m_Inputs.m_Angle > M_PI * 2)
            m_Inputs.m_Angle -= M_PI * 2;
        else if (m_Inputs.m_Angle < 0)
            m_Inputs.m_Angle += M_PI * 2;

        if (m_Inputs.m_AngleY > M_PI - 1)
            m_Inputs.m_AngleY = M_PI - 1;
        else if (m_Inputs.m_AngleY < 1)
            m_Inputs.m_AngleY = 1;

        if(IsCursorHidden())
        {
            SetMousePosition(g_Globals.m_CurrentWindowWidth/2, g_Globals.m_CurrentWindowHeight/2);
            m_Inputs.m_MousePos = {(float)g_Globals.m_CurrentWindowWidth/2, (float)g_Globals.m_CurrentWindowHeight/2};
        }
    }

    if(IsMouseButtonDown(MOUSE_BUTTON_LEFT))
    {
        m_Inputs.m_MouseClick = true;
    }
    else
    {
        m_Inputs.m_MouseClick = false;
    }

    if(IsMouseButtonDown(MOUSE_BUTTON_RIGHT))
    {
        m_Inputs.m_MouseRightClick = true;
    }
    else
    {
        m_Inputs.m_MouseRightClick = false;
    }

    if(IsKeyDown(KEY_ESCAPE))
    {
        m_Inputs.m_Escape = true;
    }
    else
    {
        m_Inputs.m_Escape = false;
    }

    if(IsKeyDown(KEY_ENTER))
    {
        m_Inputs.m_Enter = true;
    }
    else
    {
        m_Inputs.m_Enter = false;
    }

    if(IsKeyDown(KEY_BACKSPACE))
    {
        m_Inputs.m_BackSpace = true;
    }
    else
    {
        m_Inputs.m_BackSpace = false;
    }

    if(IsKeyDown(KEY_N))
    {
        m_Inputs.m_EditorNeighborKey = true;
    }
    else
    {
        m_Inputs.m_EditorNeighborKey = false;
    }

    if(IsKeyDown(KEY_E))
    {
        m_Inputs.m_EditorEditSectorKey = true;
    }
    else
    {
        m_Inputs.m_EditorEditSectorKey = false;
    }

    if(IsKeyDown(KEY_K))
    {
        m_Inputs.m_EditorSaveLevelKey = true;
    }
    else
    {
        m_Inputs.m_EditorSaveLevelKey = false;
    }

    if(IsKeyDown(KEY_L))
    {
        m_Inputs.m_EditorLoadLevelKey = true;
    }
    else
    {
        m_Inputs.m_EditorLoadLevelKey = false;
    }

    if(IsKeyDown(KEY_UP))
    {
        m_Inputs.m_ArrowUp = true;
    }
    else
    {
        m_Inputs.m_ArrowUp = false;
    }

    if(IsKeyDown(KEY_DOWN))
    {
        m_Inputs.m_ArrowDown = true;
    }
    else
    {
        m_Inputs.m_ArrowDown = false;
    }

    if(IsKeyDown(KEY_C))
    {
        m_Inputs.m_EditorCeilingKey = true;
    }
    else
    {
        m_Inputs.m_EditorCeilingKey = false;
    }

    if(IsKeyDown(KEY_F))
    {
        m_Inputs.m_EditorFloorKey = true;
    }
    else
    {
        m_Inputs.m_EditorFloorKey = false;
    }

    if(IsKeyDown(KEY_G))
    {
        m_Inputs.m_EditorCeilingSlopeKey = true;
    }
    else
    {
        m_Inputs.m_EditorCeilingSlopeKey = false;
    }

    if(IsKeyDown(KEY_V))
    {
        m_Inputs.m_EditorFloorSlopeKey = true;
    }
    else
    {
        m_Inputs.m_EditorFloorSlopeKey = false;
    }

    if(IsKeyDown(KEY_Q))
    {
        m_Inputs.m_EditorPlaceEntityKey = true;
    }
    else
    {
        m_Inputs.m_EditorPlaceEntityKey = false;
    }
}

void CInputHandler::UpdateMousePos()
{
    m_Inputs.m_MousePos = GetMousePosition();
}
