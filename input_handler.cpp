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
        m_Inputs.m_Angle -= 0.1f;
    }

    if(IsKeyDown(KEY_RIGHT))
    {
        m_Inputs.m_Angle += 0.1f;
    }
}