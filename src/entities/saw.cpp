/*
    Brain Evil: Dark World
    Copyright (C) Benjamín Gajardo (also known as +KZ)

    This program is shared under the PLUSKAIZO LICENSE, you should have received
    a copy of the license along with the program.
*/

#include "saw.h"
#include <globals.h>
#include <collision.h>
#include "player.h"
#include <config_handler.h>

ENTITY_CREATOR_FUNC(CSaw::SawCreator)
{
    return new CSaw(Pos, pSector);
}

CSaw::CSaw(Vector3 Pos, CSector * pSector, bool pickup)
{
    m_Pos = Pos;
    m_Vel = {0,0,0};
    m_pMySector = pSector;
    m_Health = 1;
    m_Type = pickup ? ENTTYPE_PICKUP : ENTTYPE_PROJECTILE;

    m_Radius = 0.5f;

    g_Game.SetNeededTexture(39); // saw
    g_Game.SetNeededTexture(40); // saw
    g_Game.SetNeededTexture(41); // saw
    g_Game.SetNeededTexture(42); // saw

    g_Game.SetNeededSound(0); //slice
    g_Game.SetNeededSound(10); //pickup

    m_Lifetime = 60 * 10;
}

void CSaw::Update()
{
    m_Lifetime--;
    if(m_Lifetime <= 0)
    {
        m_MarkedForDeletion = true;
        return;
    }

    if(m_Health <= 0)
    {
        g_ConfigHandler.m_GameProgress.m_Ammo[0]++;
        m_MarkedForDeletion = true;
        StopSound(g_Game.m_Sounds[10]);
        PlaySound(g_Game.m_Sounds[10]);
        return;
    }

    bool collided = DoMovement(m_Pos, m_Vel, m_Radius, &m_pMySector);

    m_Pos.x += m_Vel.x;
    m_Pos.y += m_Vel.y;
    m_Pos.z += m_Vel.z;

    if(collided)
    {
        m_Vel = {0,0,0};
        m_Type = ENTTYPE_PICKUP;
    }

    if(m_Type == ENTTYPE_PROJECTILE)
    {
        IEntity * pEnemy = nullptr;
        for(int i = 0; i < g_Game.NumEntities(); i++)
        {
            if((pEnemy = g_Game.GetEntity(i)))
            {
                if((pEnemy->m_Type == EntType::ENTTYPE_ENEMY || pEnemy->m_Type == EntType::ENTTYPE_PLAYER))
                {
                    if(pEnemy->m_Health > 0)
                    {
                        float distance = PointDistance3D(pEnemy->m_Pos, m_Pos);

                        if(distance <= pEnemy->m_Radius + m_Radius)
                            break;
                    }
                }
            }
            pEnemy = nullptr;
        }

        if(pEnemy)
        {
            pEnemy->m_Health -= 5;
            StopSound(g_Game.m_Sounds[0]);
            PlaySound(g_Game.m_Sounds[0]);
            m_MarkedForDeletion = true;
        }
    }
}

void CSaw::Render()
{
    if(m_Type == ENTTYPE_PROJECTILE)
    {
        m_Frame++;
        if(m_Frame >= 4)
            m_Frame = 0;
    }

    if(!m_pMySector->m_Active)
        return;

    if(m_Health <= 0)
        return;

    BeginMode3D(g_Globals.m_RaylibCamera);
    BeginShaderMode(g_Globals.m_TransparentBillboardShader);
    DrawBillboardRec(g_Globals.m_RaylibCamera, g_Game.m_Textures[39 + m_Frame], {0,0,15.f,15.f}, m_Pos, {m_Radius*2, m_Radius*2}, {255,255,255,255});
    EndShaderMode();
    EndMode3D();
}

int CSaw::GetSectorID()
{
    return g_Game.SectorPointerToID(m_pMySector);
}

void CSaw::SetSectorID(int id)
{
    m_pMySector = g_Game.GetSector(id);
}
