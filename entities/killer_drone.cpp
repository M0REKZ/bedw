// Copyright (c) Benjamín Gajardo (also known as +KZ)
#include "killer_drone.h"
#include <globals.h>
#include <collision.h>
#include <cmath>

ENTITY_CREATOR_FUNC(CKillerDrone::KillerDroneCreator)
{
    return new CKillerDrone(Pos, pSector);
}

CKillerDrone::CKillerDrone(Vector3 Pos, CSector * pSector)
{
    m_Type = EntType::ENTTYPE_ENEMY;
    m_Pos = Pos;
    m_Radius = 1.5f;
    m_Pos.y += m_Radius;
    m_Vel = {0,0,0};
    m_PrevHealth = m_Health = 1;
    m_CanCollide = false;

    m_pMySector = pSector;

    g_Game.SetNeededTexture(19); // drone
    g_Game.SetNeededTexture(20); // walk
    g_Game.SetNeededTexture(21); // also walk
    g_Game.SetNeededTexture(22); // also walk
    g_Game.SetNeededTexture(23); // attack
    g_Game.SetNeededTexture(24); // also attack
    g_Game.SetNeededTexture(25); // also attack

    g_Game.SetNeededSound(0); // slice
    g_Game.SetNeededSound(2); // metal hit
    g_Game.SetNeededSound(5); // death 2
}

void CKillerDrone::Update()
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
            StopSound(g_Game.m_Sounds[5]);
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
        if(distance < 50.f && distance > 2.5f)
        {
            m_Angle = GetAngleBetweenPoints({m_Pos.x,m_Pos.z}, {pPlayer->m_Pos.x, pPlayer->m_Pos.z});
            m_WantedVel.x = cosf(m_Angle) / 1.5f;
            m_WantedVel.z = sinf(m_Angle) / 1.5f;
            if(!m_Grounded)
            {
                m_WantedVel.x *= 1.5;
                m_WantedVel.z *= 1.5;
            }
            if(!m_Attack)
            {
                m_AttackFrame = 0;
            }
        }
        else
        {
            if(distance <= 2.5f)
            {
                if(!m_Attack)
                {
                    m_Attack = true;
                    m_AttackDelay = 10;
                }
            }
            else if(!m_Attack)
            {
                m_AttackFrame = 0;
            }
            m_WantedVel = {0,0,0};
        }

        if(distance < 50.f)
        {
            m_JumpTimer--;
            if(m_JumpTimer <= 0)
            {
                if(m_Grounded)
                    m_Vel.y = 0.125f;
                m_JumpTimer = 120;
            }
        }

        if(m_Attack && m_Grounded)
        {
            m_WantedVel = {0,0,0};
            m_AttackDelay--;
            if(m_AttackDelay <= 0)
            {
                if(distance <= 2.f)
                {
                    PlaySound(g_Game.m_Sounds[0]);
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
        else if(!m_Grounded && !m_AirAttacked)
        {
            if(distance <= 2.f)
            {
                m_AirAttacked = true;
                PlaySound(g_Game.m_Sounds[0]);
                pPlayer->m_Health -= 5;
            }
        }
    }
    else
    {
        m_WantedVel = {0,0,0};
        m_Attack = false;
        m_AttackFrame = 0;
    }

    float stepvel = 0.05f;
    float slide = 0.95;
    if(!m_Grounded)
    {
        stepvel = 0.1f;
        slide = 0.978f;
    }
    else
    {
        m_AirAttacked = false;
    }

    m_Vel.x *= slide;
    if(std::abs(m_Vel.x) < std::abs(m_WantedVel.x))
        m_Vel.x += (m_WantedVel.x > 0.f ? stepvel : -stepvel) * std::abs(cosf(m_Angle));
    m_Vel.z *= slide;
    if(std::abs(m_Vel.z) < std::abs(m_WantedVel.z))
        m_Vel.z += (m_WantedVel.z > 0.f ? stepvel : -stepvel) * std::abs(sinf(m_Angle));
    m_Vel.y += -0.01;

    if(std::abs(m_Vel.x) < 0.01f)
        m_Vel.x = 0.f;
    if(std::abs(m_Vel.z) < 0.01f)
        m_Vel.z = 0.f;

    DoMovement(m_Pos, m_Vel, m_Radius, &m_pMySector, &m_Grounded);

    m_Pos.x += m_Vel.x;
    m_Pos.y += m_Vel.y;
    m_Pos.z += m_Vel.z;
}

void CKillerDrone::Render()
{
    if(!m_pMySector->m_Active)
        return;

    if(!m_Grounded)
    {
        m_Frame = 7;
    }
    else if(m_AttackFrame)
    {
        if(m_AttackFrame == 1 || m_AttackFrame == 2)
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

    Vector3 RenderPos;

    RenderPos.x = m_Pos.x;
    RenderPos.z = m_Pos.z;
    RenderPos.y = m_Pos.y - m_Radius/2;

    BeginMode3D(g_Globals.m_RaylibCamera);
    BeginShaderMode(g_Globals.m_TransparentBillboardShader);
    if(m_Health <= 0)
    { /*TODO: dead sprite*/}
    else if(m_Frame == 0)
        DrawBillboardRec(g_Globals.m_RaylibCamera, g_Game.m_Textures[19], {0,0,64.f,32.f}, RenderPos, {m_Radius*2, m_Radius}, {255,255,255,255});
    else if(m_Frame == 3 || m_Frame == 5)
        DrawBillboardRec(g_Globals.m_RaylibCamera, g_Game.m_Textures[20], {0,0,64.f,32.f}, RenderPos, {m_Radius*2, m_Radius}, {255,255,255,255});
    else if(m_Frame == 4)
        DrawBillboardRec(g_Globals.m_RaylibCamera, g_Game.m_Textures[21], {0,0,64.f,32.f}, RenderPos, {m_Radius*2, m_Radius}, {255,255,255,255});
    else if(m_Frame == 6)
        DrawBillboardRec(g_Globals.m_RaylibCamera, g_Game.m_Textures[22], {0,0,64.f,32.f}, RenderPos, {m_Radius*2, m_Radius}, {255,255,255,255});
    else if(m_Frame == 1)
        DrawBillboardRec(g_Globals.m_RaylibCamera, g_Game.m_Textures[24], {0,0,64.f,32.f}, RenderPos, {m_Radius*2, m_Radius}, {255,255,255,255});
    else if(m_Frame == 2)
        DrawBillboardRec(g_Globals.m_RaylibCamera, g_Game.m_Textures[25], {0,0,64.f,32.f}, RenderPos, {m_Radius*2, m_Radius}, {255,255,255,255});
    else if(m_Frame == 7)
        DrawBillboardRec(g_Globals.m_RaylibCamera, g_Game.m_Textures[23], {0,0,64.f,32.f}, RenderPos, {m_Radius*2, m_Radius}, {255,255,255,255});
    EndShaderMode();
    EndMode3D();
}

int CKillerDrone::GetSectorID()
{
    return g_Game.SectorPointerToID(m_pMySector);
}

void CKillerDrone::SetSectorID(int id)
{
    m_pMySector = g_Game.GetSector(id);
}

