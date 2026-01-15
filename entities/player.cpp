// Copyright (c) Benjamín Gajardo (also known as +KZ)

#include "player.h"
#include <game.h>
#include <globals.h>
#include <collision.h>
#include <input_handler.h>
#include <cmath>
#include <cstdio>

ENTITY_CREATOR_FUNC(CPlayer::PlayerCreator)
{
    g_Game.SetCurrentSector(pSector);
    return new CPlayer(Pos);
}

CPlayer::CPlayer(Vector3 Pos)
{
    m_Pos = Pos;
}

void CPlayer::Update()
{
    const SInput * pInput = g_InputHandler.GetInputs();

    Vector3 OldPos = m_Pos;

    m_Angle = pInput->m_Angle;

    m_Vel = {0,0,0};

    if(pInput->m_Front)
    {
        m_Vel.x = cosf(m_Angle);
        m_Vel.z = sinf(m_Angle);
    }
    else if(pInput->m_Back)
    {
        m_Vel.x = -cosf(m_Angle);
        m_Vel.z = -sinf(m_Angle);
    }

    if(pInput->m_Right)
    {
        m_Vel.x = cosf(m_Angle + M_PI_2);
        m_Vel.z = sinf(m_Angle + M_PI_2);
    }
    else if(pInput->m_Left)
    {
        m_Vel.x = -cosf(m_Angle + M_PI_2);
        m_Vel.z = -sinf(m_Angle + M_PI_2);
    }

    if(pInput->m_Jump)
    {
        m_Vel.y = .1f;
    }
    else
    {
        m_Vel.y = -.1f;
    }

    
    CSector *pCurrentSector = g_Game.GetCurrentSector();

    //Player collision
    DoMovement(m_Pos, m_Vel, m_Radius, &pCurrentSector);

    g_Game.SetCurrentSector(pCurrentSector);

    m_Pos.x += m_Vel.x;
    m_Pos.y += m_Vel.y;
    m_Pos.z += m_Vel.z;

    g_Globals.m_Camera.m_Target = m_Pos;
    g_Globals.m_Camera.m_Target.y = m_Pos.y + .5f;

    Vector3 CameraOffset;

    CameraOffset.x = -cosf(m_Angle) * 16;
    CameraOffset.y = 2.f;
    CameraOffset.z = -sinf(m_Angle) * 16;

    //Camera collision
    DoMovement(m_Pos, CameraOffset, 0.9f);

    g_Globals.m_Camera.m_Pos.x = m_Pos.x + CameraOffset.x;
    g_Globals.m_Camera.m_Pos.y = m_Pos.y + CameraOffset.y;
    g_Globals.m_Camera.m_Pos.z = m_Pos.z + CameraOffset.z;
}

void CPlayer::Render()
{
    BeginMode3D(g_Globals.m_RaylibCamera);
    DrawSphere(m_Pos, m_Radius, {0,255,0,255});
    EndMode3D();
}

int CPlayer::GetSectorID()
{
    return g_Game.SectorPointerToID(g_Game.GetCurrentSector());
}
