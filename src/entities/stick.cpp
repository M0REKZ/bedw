/*
    Brain Evil: Dark World
    Copyright (C) Benjamín Gajardo (also known as +KZ)

    This program is shared under the PLUSKAIZO LICENSE, you should have received
    a copy of the license along with the program.
*/

#include "stick.h"
#include <globals.h>
#include <level_script_handler.h>
#include <config_handler.h>

ENTITY_CREATOR_FUNC(CStick::StickCreator)
{
    return new CStick(Pos, pSector);
}

CStick::CStick(Vector3 Pos, CSector * pSector)
{
    m_Pos = Pos;
    m_Vel = {0,0,0};
    m_pMySector = pSector;
    m_Health = 1;
    m_Type = ENTTYPE_PICKUP;

    g_Game.SetNeededTexture(45); // stick
}

void CStick::Update()
{
    if(g_ConfigHandler.m_GameProgress.m_GotStick)
    {
        m_MarkedForDeletion = true;
        return;
    }

    if(m_Health <= 0)
    {
        g_ConfigHandler.m_GameProgress.m_GotStick = true;
    }
}

void CStick::Render()
{
    if(!m_pMySector->m_Active)
        return;

    if(m_Health <= 0)
        return;

    BeginMode3D(g_Globals.m_RaylibCamera);
    BeginShaderMode(g_Globals.m_TransparentBillboardShader);
    DrawBillboardRec(g_Globals.m_RaylibCamera, g_Game.m_Textures[45], {0,0,32.f,32.f}, m_Pos, {m_Radius*2, m_Radius*2}, {255,255,255,255});
    EndShaderMode();
    EndMode3D();
}

int CStick::GetSectorID()
{
    return g_Game.SectorPointerToID(m_pMySector);
}

void CStick::SetSectorID(int id)
{
    m_pMySector = g_Game.GetSector(id);
}
