// Copyright (c) Benjamín Gajardo (also known as +KZ)
#include "menu_camera.h"
#include <raylib.h>
#include <collision.h>
#include <input_handler.h>
#include <globals.h>
#include <pause_handler.h>

ENTITY_CREATOR_FUNC(CMenuCamera::MenuCameraCreator)
{
    g_Game.SetCurrentSector(pSector);
    return new CMenuCamera(Pos);
}

CMenuCamera::CMenuCamera(Vector3 Pos)
{
    m_Pos = Pos;
    m_Vel = {0,0,0};
}

void CMenuCamera::Render()
{
    m_Angle += 0.005f;

    if(m_Angle >= M_PI * 2)
        m_Angle -= M_PI * 2;

    Vector3 CameraOffset;

    CameraOffset.x = -cosf(m_Angle) * 16;
    CameraOffset.y = m_Pos.y;
    CameraOffset.z = -sinf(m_Angle) * 16;

    printf("%f %f %f\n",CameraOffset.x, CameraOffset.z, m_Angle);

    //Camera collision
    DoMovement(m_Pos, CameraOffset, 0.9f);


    g_Globals.m_Camera.m_Target = m_Pos;
    g_Globals.m_Camera.m_Pos.x = m_Pos.x + CameraOffset.x;
    g_Globals.m_Camera.m_Pos.y = m_Pos.y + CameraOffset.y;
    g_Globals.m_Camera.m_Pos.z = m_Pos.z + CameraOffset.z;
}

int CMenuCamera::GetSectorID()
{
    return g_Game.SectorPointerToID(g_Game.GetCurrentSector());
}

void CMenuCamera::SetSectorID(int id)
{
    g_Game.SetCurrentSector(g_Game.GetSector(id));
}
