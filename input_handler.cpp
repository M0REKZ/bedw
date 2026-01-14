// Copyright (c) Benjamín Gajardo (also known as +KZ)

#include "input_handler.h"
#include <raylib.h>

CInputHandler g_InputHandler;

void CInputHandler::UpdateInput()
{
    if(IsKeyDown(KEY_W))
    {
        m_Inputs.m_Front = true;
    }
    else
    {
        m_Inputs.m_Front = false;
    }

    if(IsKeyDown(KEY_A))
    {
        m_Inputs.m_Left = true;
    }
    else
    {
        m_Inputs.m_Left = false;
    }

    if(IsKeyDown(KEY_S))
    {
        m_Inputs.m_Back = true;
    }
    else
    {
        m_Inputs.m_Back = false;
    }

    if(IsKeyDown(KEY_D))
    {
        m_Inputs.m_Right = true;
    }
    else
    {
        m_Inputs.m_Right = false;
    }

    if(IsKeyDown(KEY_SPACE))
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
        m_Inputs.m_Angle -= 0.1f;
    }
    else
    {
        m_Inputs.m_ArrowLeft = false;
    }

    if(IsKeyDown(KEY_RIGHT))
    {
        m_Inputs.m_ArrowRight = true;
        m_Inputs.m_Angle += 0.1f;
    }
    else
    {
        m_Inputs.m_ArrowRight = false;
    }

    m_Inputs.m_MousePos = GetMousePosition();

    if(IsMouseButtonDown(MOUSE_BUTTON_LEFT))
    {
        m_Inputs.m_MouseClick = true;
    }
    else
    {
        m_Inputs.m_MouseClick = false;
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
}