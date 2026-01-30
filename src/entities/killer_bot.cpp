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
#include "killer_bot.h"
#include <globals.h>
#include <collision.h>
#include <cmath>

ENTITY_CREATOR_FUNC(CKillerBot::KillerBotCreator)
{
    return new CKillerBot(Pos, pSector, KILLERBOT_SLICE);
}

ENTITY_CREATOR_FUNC(CSawBot::SawBotCreator)
{
    return new CKillerBot(Pos, pSector, KILLERBOT_SAW);
}

ENTITY_CREATOR_FUNC(CShootBot::ShootBotCreator)
{
    return new CKillerBot(Pos, pSector, KILLERBOT_SHOOT);
}

CKillerBot::CKillerBot(Vector3 Pos, CSector * pSector, KillerBotType BotType)
{
    m_BotType = BotType;

    m_Type = EntType::ENTTYPE_ENEMY;
    m_Pos = Pos;
    m_Radius = 1.2f;
    m_Pos.y += m_Radius;
    m_Vel = {0,0,0};
    m_CanCollide = true;

    m_pMySector = pSector;

    g_Game.SetNeededSound(2); // metal hit
    g_Game.SetNeededSound(4); // death
    g_Game.SetNeededSound(5); // death 2

    switch (m_BotType)
    {
    case KILLERBOT_SAW:
        m_PrevHealth = m_Health = 6;
        m_EdgeSmart = true;

        g_Game.SetNeededTexture(28); // sawbot
        g_Game.SetNeededTexture(29); // walk
        g_Game.SetNeededTexture(30); // also walk
        g_Game.SetNeededTexture(31); // attack
        g_Game.SetNeededTexture(32); // also attack

        g_Game.SetNeededSound(8); // saw
        break;
    case KILLERBOT_SHOOT:
        m_PrevHealth = m_Health = 16;
        m_EdgeSmart = true;

        g_Game.SetNeededTexture(38); // sawbot
        g_Game.SetNeededTexture(37); // walk
        g_Game.SetNeededTexture(36); // also walk
        g_Game.SetNeededTexture(34); // attack
        g_Game.SetNeededTexture(33); // also attack
        g_Game.SetNeededTexture(35); // shoot!

        g_Game.SetNeededSound(0); // slice
        g_Game.SetNeededSound(9); // shoot!

        m_ShootDelay = 60 * 4;
        break;
    
    default:
        g_Game.SetNeededTexture(11); // killerbot
        g_Game.SetNeededTexture(12); // walk
        g_Game.SetNeededTexture(13); // also walk
        g_Game.SetNeededTexture(14); // attack
        g_Game.SetNeededTexture(15); // also attack

        g_Game.SetNeededSound(0); // slice
        m_PrevHealth = m_Health = 3;
        break;
    }
}

void CKillerBot::Update()
{
    if(m_Pos.y < g_Game.m_DeathLineY)
    {
        m_Health = 0;
    }

    if(m_PrevHealth > m_Health)
    {
        m_PrevHealth = m_Health;
        StopSound(g_Game.m_Sounds[2]);
        PlaySound(g_Game.m_Sounds[2]);
    }

    if(m_Health <= 0)
    {
        if(!m_PlayedDeathSound)
        {
            StopSound(g_Game.m_Sounds[4]);
            StopSound(g_Game.m_Sounds[5]);
            PlaySound(g_Game.m_Sounds[4]);
            PlaySound(g_Game.m_Sounds[5]);
            m_PlayedDeathSound = true;
        }
        m_CanCollide = false;
        return;
    }

    IEntity * pPlayer = nullptr;
    for(int i = 0; i < g_Game.NumEntities(); i++)
    {
        if((pPlayer = g_Game.GetEntity(i)))
        {
            if(pPlayer->m_Type == EntType::ENTTYPE_PLAYER)
                break;
        }
    }

    if(pPlayer && pPlayer->m_Health > 0)
    {
        float distance = PointDistance3D(pPlayer->m_Pos, m_Pos);
        if(distance < 50.f && distance > 2.f)
        {
            m_Angle = GetAngleBetweenPoints({m_Pos.x,m_Pos.z}, {pPlayer->m_Pos.x, pPlayer->m_Pos.z});
            m_WantedVel.x = cosf(m_Angle) / 2.f;
            m_WantedVel.z = sinf(m_Angle) / 2.f;

            if(m_BotType == KILLERBOT_SHOOT)
            {
                m_WantedVel.x /= 4.f;
                m_WantedVel.z /= 4.f;

                //shooting
                m_ShootDelay--;
                if(m_ShootDelay < 60)
                {
                    m_AttackFrame = 7;
                    m_WantedVel.x = 0.f;
                    m_WantedVel.z = 0.f;
                }

                if(m_ShootDelay <= 0)
                {
                    StopSound(g_Game.m_Sounds[9]);
                    PlaySound(g_Game.m_Sounds[9]);

                    m_LaserShootedPos = pPlayer->m_Pos;
                    m_LaserShootedPos.y = m_Pos.y;
                    Vector2 TempPos = {m_LaserShootedPos.x, m_LaserShootedPos.z};
                    CSector * pTempInOutSector = m_pMySector;
                    float Step = (pPlayer->m_Pos.y - m_Pos.y) / PointDistance({m_Pos.x, m_Pos.z}, TempPos);
                    if(!DoRaycast({m_Pos.x, m_Pos.z}, TempPos, m_LaserShootedPos.y, Step, &pTempInOutSector))
                    {
                        pPlayer->m_Health -= 50;
                    }
                    m_LaserShootedPos.x = TempPos.x;
                    m_LaserShootedPos.z = TempPos.y;
                    m_RenderLaser = true;
                    m_ShootDelay = 60 * 4;
                }
            }

            if(!m_Attack &&
                !(m_BotType == KILLERBOT_SHOOT && m_ShootDelay < 60))
            {
                m_AttackFrame = 0;
            }
        }
        else
        {
            if(distance <= 2.f)
            {
                if(!m_Attack)
                {
                    m_Attack = true;
                    if(m_BotType == KILLERBOT_SLICE)
                    {
                        m_AttackDelay = 20;
                    }
                    else if(m_BotType == KILLERBOT_SAW)
                    {
                        if(m_AttackFrame == 0)
                            m_AttackDelay = 0;
                        else
                            m_AttackDelay = 8;
                    }
                    else if(m_BotType == KILLERBOT_SHOOT)
                    {
                        m_AttackDelay = 10;
                    }
                }
            }
            else if(!m_Attack)
            {
                m_AttackFrame = 0;
            }
            m_ShootDelay = 60 * 4;
            m_WantedVel = {0,0,0};
        }

        if(m_Attack)
        {
            m_WantedVel = {0,0,0};
            m_AttackDelay--;
            if(m_AttackDelay <= 0)
            {
                if(distance <= 2.f)
                {
                    if(m_BotType == KILLERBOT_SLICE || m_BotType == KILLERBOT_SHOOT)
                    {
                        StopSound(g_Game.m_Sounds[0]);
                        PlaySound(g_Game.m_Sounds[0]);
                    }
                    else if(m_BotType == KILLERBOT_SAW)
                    {
                        StopSound(g_Game.m_Sounds[8]);
                        PlaySound(g_Game.m_Sounds[8]);
                    }
                    pPlayer->m_Health -= 5;
                }
                m_Attack = false;
                m_AttackFrame++;
                if(m_AttackFrame >= 3)
                {
                    m_AttackFrame = 1;
                }
            }
        }
    }
    else
    {
        m_WantedVel = {0,0,0};
        m_Attack = false;
        m_AttackFrame = 0;
        m_ShootDelay = 60 * 4;
    }

    if(m_EdgeSmart && !m_FutureGrounded)
    {
        m_Vel.x = 0;
        m_Vel.z = 0;
        m_WantedVel.x = 0;
        m_WantedVel.z = 0;
    }

    m_Vel.x *= 0.9f;
    if(std::abs(m_WantedVel.x) > 0.001f && std::abs(m_Vel.x) < std::abs(m_WantedVel.x))
        m_Vel.x += (m_WantedVel.x > 0.f ? 0.1f : -0.1f) * std::abs(cosf(m_Angle));
    m_Vel.z *= 0.9f;
    if(std::abs(m_WantedVel.z) > 0.001f && std::abs(m_Vel.z) < std::abs(m_WantedVel.z))
        m_Vel.z += (m_WantedVel.z > 0.f ? 0.1f : -0.1f) * std::abs(sinf(m_Angle));
    m_Vel.y += -0.025;

    if(std::abs(m_Vel.x) < 0.01f)
        m_Vel.x = 0.f;
    if(std::abs(m_Vel.z) < 0.01f)
        m_Vel.z = 0.f;

    IEntity * pEntity = nullptr;
    for(int i = 0; i < g_Game.NumEntities(); i++)
    {
        if((pEntity = g_Game.GetEntity(i)))
        {
            if(pEntity == this)
                continue;

            if(!pEntity->m_CanCollide)
                continue;
            
            DoEntityCollision(m_Pos, m_Vel, m_Radius, pEntity->m_Pos, pEntity->m_Radius);
        }
    }

    if(m_EdgeSmart)
    {
        Vector2 FuturePos = {m_Pos.x, m_Pos.z};
        FuturePos.x += cosf(m_Angle);
        FuturePos.y += sinf(m_Angle);

        CSector * pStanding = m_pMySector;
        CSector * pFutureSec = m_pMySector;

        for(int i = 0; i < m_pMySector->m_NumVertices; i++)
        {
            if(!m_pMySector->m_pNeighbors[i])
                continue;

            //for reversed sectors
            if(IsPointInsideSector(m_pMySector->m_pNeighbors[i], {m_Pos.x, m_Pos.z}))
            {
                if(m_pMySector->m_pNeighbors[i]->m_Floor > m_pMySector->m_pNeighbors[i]->m_Ceiling && pStanding->m_Floor < m_pMySector->m_pNeighbors[i]->m_Floor)
                {
                    pStanding = m_pMySector->m_pNeighbors[i];
                }
            }
            
            if(IsPointInsideSector(m_pMySector->m_pNeighbors[i], FuturePos))
            {
                pFutureSec = m_pMySector->m_pNeighbors[i];
                break;
            }
        }

        if(pFutureSec == pStanding || pFutureSec->m_Floor >= pStanding->m_Floor)
        {
            m_FutureGrounded = true;
        }
        else
        {
            m_FutureGrounded = false;
        }

        if(!m_FutureGrounded)
        {
            m_Vel.x = 0;
            m_Vel.z = 0;
            m_WantedVel.x = 0;
            m_WantedVel.z = 0;
        }
    }

    DoMovement(m_Pos, m_Vel, m_Radius, &m_pMySector, &m_Grounded);

    m_Pos.x += m_Vel.x;
    m_Pos.y += m_Vel.y;
    m_Pos.z += m_Vel.z;
}

void CKillerBot::Render()
{
    if(!m_pMySector->m_Active)
        return;

    if(m_AttackFrame)
    {
        if(m_AttackFrame == 1 || m_AttackFrame == 2 || m_AttackFrame == 7)
            m_Frame = m_AttackFrame;
    }
    else if(std::abs(m_Vel.x) > 0.001f || std::abs(m_Vel.z) > 0.001f)
    {
        m_FrameTime++;
            if(m_FrameTime >= 8)
            {
                m_FrameTime = 0;
                m_Frame++;
            }
            if(m_Frame < 3 || m_Frame > 6)
            {
                m_Frame = 3;
            }
    }
    else
    {
        m_Frame = 0;
        m_FrameTime = 0;
    }

    BeginMode3D(g_Globals.m_RaylibCamera);
    BeginShaderMode(g_Globals.m_TransparentBillboardShader);

    if(m_BotType == KILLERBOT_SLICE)
    {
        if(m_Health <= 0)
        { /*TODO: dead sprite*/}
        else if(m_Frame == 0 || m_Frame == 3 || m_Frame == 5)
            DrawBillboardRec(g_Globals.m_RaylibCamera, g_Game.m_Textures[11], {0,0,32.f,40.f}, m_Pos, {m_Radius*2*0.8f, m_Radius*2}, {255,255,255,255});
        else if(m_Frame == 4)
            DrawBillboardRec(g_Globals.m_RaylibCamera, g_Game.m_Textures[12], {0,0,32.f,40.f}, m_Pos, {m_Radius*2*0.8f, m_Radius*2}, {255,255,255,255});
        else if(m_Frame == 6)
            DrawBillboardRec(g_Globals.m_RaylibCamera, g_Game.m_Textures[13], {0,0,32.f,40.f}, m_Pos, {m_Radius*2*0.8f, m_Radius*2}, {255,255,255,255});
        else if(m_Frame == 1)
            DrawBillboardRec(g_Globals.m_RaylibCamera, g_Game.m_Textures[14], {0,0,32.f,40.f}, m_Pos, {m_Radius*2*0.8f, m_Radius*2}, {255,255,255,255});
        else if(m_Frame == 2)
            DrawBillboardRec(g_Globals.m_RaylibCamera, g_Game.m_Textures[15], {0,0,32.f,40.f}, m_Pos, {m_Radius*2*0.8f, m_Radius*2}, {255,255,255,255});
    }
    else if(m_BotType == KILLERBOT_SAW)
    {
        if(m_Health <= 0)
        { /*TODO: dead sprite*/}
        else if(m_Frame == 0 || m_Frame == 3 || m_Frame == 5)
            DrawBillboardRec(g_Globals.m_RaylibCamera, g_Game.m_Textures[28], {0,0,32.f,40.f}, m_Pos, {m_Radius*2*0.8f, m_Radius*2}, {255,255,255,255});
        else if(m_Frame == 4)
            DrawBillboardRec(g_Globals.m_RaylibCamera, g_Game.m_Textures[29], {0,0,32.f,40.f}, m_Pos, {m_Radius*2*0.8f, m_Radius*2}, {255,255,255,255});
        else if(m_Frame == 6)
            DrawBillboardRec(g_Globals.m_RaylibCamera, g_Game.m_Textures[30], {0,0,32.f,40.f}, m_Pos, {m_Radius*2*0.8f, m_Radius*2}, {255,255,255,255});
        else if(m_Frame == 1)
            DrawBillboardRec(g_Globals.m_RaylibCamera, g_Game.m_Textures[31], {0,0,32.f,40.f}, m_Pos, {m_Radius*2*0.8f, m_Radius*2}, {255,255,255,255});
        else if(m_Frame == 2)
            DrawBillboardRec(g_Globals.m_RaylibCamera, g_Game.m_Textures[32], {0,0,32.f,40.f}, m_Pos, {m_Radius*2*0.8f, m_Radius*2}, {255,255,255,255});
    }
    else if(m_BotType == KILLERBOT_SHOOT)
    {
        if(m_Health <= 0)
        { /*TODO: dead sprite*/}
        else if(m_Frame == 0 || m_Frame == 3 || m_Frame == 5)
            DrawBillboardRec(g_Globals.m_RaylibCamera, g_Game.m_Textures[38], {0,0,32.f,40.f}, m_Pos, {m_Radius*2*0.8f, m_Radius*2}, {255,255,255,255});
        else if(m_Frame == 4)
            DrawBillboardRec(g_Globals.m_RaylibCamera, g_Game.m_Textures[37], {0,0,32.f,40.f}, m_Pos, {m_Radius*2*0.8f, m_Radius*2}, {255,255,255,255});
        else if(m_Frame == 6)
            DrawBillboardRec(g_Globals.m_RaylibCamera, g_Game.m_Textures[36], {0,0,32.f,40.f}, m_Pos, {m_Radius*2*0.8f, m_Radius*2}, {255,255,255,255});
        else if(m_Frame == 1)
            DrawBillboardRec(g_Globals.m_RaylibCamera, g_Game.m_Textures[34], {0,0,32.f,40.f}, m_Pos, {m_Radius*2*0.8f, m_Radius*2}, {255,255,255,255});
        else if(m_Frame == 2)
            DrawBillboardRec(g_Globals.m_RaylibCamera, g_Game.m_Textures[33], {0,0,32.f,40.f}, m_Pos, {m_Radius*2*0.8f, m_Radius*2}, {255,255,255,255});
        else if(m_Frame == 7) //shoot!
            DrawBillboardRec(g_Globals.m_RaylibCamera, g_Game.m_Textures[35], {0,0,32.f,40.f}, m_Pos, {m_Radius*2*0.8f, m_Radius*2}, {255,255,255,255});
    }
    EndShaderMode();

    if(m_RenderLaser)
    {
        DrawCylinderEx(m_Pos, m_LaserShootedPos, 0.1f, 0.1f, 3, RED);
        m_RenderLaser = false;
    }

    EndMode3D();
}

int CKillerBot::GetSectorID()
{
    return g_Game.SectorPointerToID(m_pMySector);
}

void CKillerBot::SetSectorID(int id)
{
    m_pMySector = g_Game.GetSector(id);
}

unsigned int CKillerBot::GetEntityID()
{
    switch (m_BotType)
    {
    case KILLERBOT_SAW:
        return CSawBot::SGetEntityID();
        break;
    case KILLERBOT_SHOOT:
        return CShootBot::SGetEntityID();
        break;
    default:
        return CKillerBot::SGetEntityID();
        break;
    }
}
