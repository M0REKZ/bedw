/*
    Brain Evil: Dark World
    Copyright (C) Benjamín Gajardo (also known as +KZ)

    This program is shared under the PLUSKAIZO LICENSE, you should have received
    a copy of the license along with the program.
*/

#include "checkpoint.h"
#include <globals.h>
#include <level_script_handler.h>
#include <config_handler.h>
#include <collision.h>

ENTITY_CREATOR_FUNC(CCheckpoint::CheckpointCreator)
{
    return new CCheckpoint(Pos, pSector);
}

CCheckpoint::CCheckpoint(Vector3 Pos, CSector * pSector)
{
    m_Pos = Pos;
    m_Vel = {0,0,0};
    m_pMySector = pSector;
    m_Health = 1;
    m_Type = ENTTYPE_PICKUP;

    m_CheckpointID = g_Game.SectorPointerToID(pSector);

    g_Game.SetNeededTexture(55); // checkpoint
    g_Game.SetNeededSound(11); //pickup

    Vector3 Offset = {0,0,0};
    DoFloorCollision(m_Pos, Offset, m_Radius, m_pMySector);
    m_Pos.y += Offset.y;
}

void CCheckpoint::Update()
{
    if(m_CheckpointID < 0) //invalid checkpoint
    {
        return;
    }

    if(g_ConfigHandler.m_GameProgress.m_CheckPoint == m_CheckpointID)
    {
        return;
    }

    if(m_Health <= 0)
    {
        StopSound(g_Game.m_Sounds[11]);
        PlaySound(g_Game.m_Sounds[11]);
        g_ConfigHandler.m_GameProgress.m_CheckPoint = m_CheckpointID;
        m_Health = 1;

        g_ConfigHandler.SaveGameProgress(); // save checkpoint
    }
}

void CCheckpoint::Render()
{
    if(m_CheckpointID < 0) //invalid checkpoint
    {
        return;
    }

    if(g_ConfigHandler.m_GameProgress.m_CheckPoint == m_CheckpointID)
    {
        return;
    }

    if(!m_pMySector->m_Active)
        return;

    BeginMode3D(g_Globals.m_RaylibCamera);
    BeginShaderMode(g_Globals.m_TransparentBillboardShader);
    DrawBillboardRec(g_Globals.m_RaylibCamera, g_Game.m_Textures[55], {0,0,32.f,32.f}, m_Pos, {m_Radius*2, m_Radius*2}, {255,255,255,255});
    EndShaderMode();
    EndMode3D();
}

int CCheckpoint::GetSectorID()
{
    return g_Game.SectorPointerToID(m_pMySector);
}

void CCheckpoint::SetSectorID(int id)
{
    m_pMySector = g_Game.GetSector(id);
}
