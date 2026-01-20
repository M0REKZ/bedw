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

    g_Game.SetNeededTexture(18); // logo
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

    //Camera collision
    DoMovement(m_Pos, CameraOffset, 0.9f);


    g_Globals.m_Camera.m_Target = m_Pos;
    g_Globals.m_Camera.m_Pos.x = m_Pos.x + CameraOffset.x;
    g_Globals.m_Camera.m_Pos.y = m_Pos.y + CameraOffset.y;
    g_Globals.m_Camera.m_Pos.z = m_Pos.z + CameraOffset.z;


    BeginMode2D(g_Globals.m_RaylibCamera2D);

    float scale = std::min((float)g_Globals.m_CurrentWindowWidth / GAME_WIDTH ,(float)g_Globals.m_CurrentWindowHeight / GAME_HEIGHT);

    DrawTextureEx(g_Game.m_Textures[18], {g_Globals.m_CurrentWindowWidth / 2 - 256 * scale, 10}, 0, scale, {255,255,255,255});

    EndMode2D();
}

int CMenuCamera::GetSectorID()
{
    return g_Game.SectorPointerToID(g_Game.GetCurrentSector());
}

void CMenuCamera::SetSectorID(int id)
{
    g_Game.SetCurrentSector(g_Game.GetSector(id));
}
