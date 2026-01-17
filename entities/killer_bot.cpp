// Copyright (c) Benjamín Gajardo (also known as +KZ)
#include "killer_bot.h"
#include <globals.h>
#include <collision.h>

ENTITY_CREATOR_FUNC(CKillerBot::KillerBotCreator)
{
    return new CKillerBot(Pos, pSector);
}

CKillerBot::CKillerBot(Vector3 Pos, CSector * pSector)
{
    m_Pos = Pos;
    m_Pos.y += m_Radius;
    m_Vel = {0,0,0};
    m_Health = 3;

    m_pMySector = pSector;

    g_Game.SetNeededTexture(11); // killerbot
    g_Game.SetNeededTexture(12); // walk
    g_Game.SetNeededTexture(13); // also walk
    g_Game.SetNeededTexture(14); // attack
    g_Game.SetNeededTexture(15); // also attack
}

void CKillerBot::Update()
{
    IEntity * pPlayer;
    for(int i = 0; i < g_Game.NumEntities(); i++)
    {
        if((pPlayer = g_Game.GetEntity(i)))
        {
            if(pPlayer->GetEntityID() == 0)
                break;
        }
    }

    if(pPlayer && pPlayer->m_Health > 0)
    {
        float distance = PointDistance({pPlayer->m_Pos.x, pPlayer->m_Pos.z}, {m_Pos.x, m_Pos.z});
        if(distance < 30.f && distance > 1.5f)
        {
            m_Angle = GetAngleBetweenPoints({m_Pos.x,m_Pos.z}, {pPlayer->m_Pos.x, pPlayer->m_Pos.z});
            m_WantedVel.x = cosf(m_Angle) / 5;
            m_WantedVel.z = sinf(m_Angle) / 5;
            if(!m_Attack)
            {
                m_AttackFrame = 0;
            }
        }
        else
        {
            if(distance <= 1.5f)
            {
                if(!m_Attack)
                {
                    m_Attack = true;
                    m_AttackDelay = 20;
                }
            }
            else if(!m_Attack)
            {
                m_AttackFrame = 0;
            }
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
    }

    m_Vel.x *= 0.9f;
    if(std::abs(m_Vel.x) < std::abs(m_WantedVel.x))
        m_Vel.x += (m_WantedVel.x > 0.f ? 0.1f : -0.1f) * std::abs(cosf(m_Angle));
    m_Vel.z *= 0.9f;
    if(std::abs(m_Vel.z) < std::abs(m_WantedVel.z))
        m_Vel.z += (m_WantedVel.z > 0.f ? 0.1f : -0.1f) * std::abs(sinf(m_Angle));
    m_Vel.y += -0.05;

    if(std::abs(m_Vel.x) < 0.01f)
        m_Vel.x = 0.f;
    if(std::abs(m_Vel.z) < 0.01f)
        m_Vel.z = 0.f;

    DoMovement(m_Pos, m_Vel, m_Radius, &m_pMySector, &m_Grounded);

    m_Pos.x += m_Vel.x;
    m_Pos.y += m_Vel.y;
    m_Pos.z += m_Vel.z;
}

void CKillerBot::Render()
{
    if(m_AttackFrame)
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

    BeginMode3D(g_Globals.m_RaylibCamera);
    BeginShaderMode(g_Globals.m_TransparentBillboardShader);
    if(m_Frame == 0 || m_Frame == 3 || m_Frame == 5)
        DrawBillboardRec(g_Globals.m_RaylibCamera, g_Game.m_Textures[11], {0,0,32.f,40.f}, m_Pos, {m_Radius*2*0.8f, m_Radius*2}, {255,255,255,255});
    else if(m_Frame == 4)
        DrawBillboardRec(g_Globals.m_RaylibCamera, g_Game.m_Textures[12], {0,0,32.f,40.f}, m_Pos, {m_Radius*2*0.8f, m_Radius*2}, {255,255,255,255});
    else if(m_Frame == 6)
        DrawBillboardRec(g_Globals.m_RaylibCamera, g_Game.m_Textures[13], {0,0,32.f,40.f}, m_Pos, {m_Radius*2*0.8f, m_Radius*2}, {255,255,255,255});
    else if(m_Frame == 1)
        DrawBillboardRec(g_Globals.m_RaylibCamera, g_Game.m_Textures[14], {0,0,32.f,40.f}, m_Pos, {m_Radius*2*0.8f, m_Radius*2}, {255,255,255,255});
    else if(m_Frame == 2)
        DrawBillboardRec(g_Globals.m_RaylibCamera, g_Game.m_Textures[15], {0,0,32.f,40.f}, m_Pos, {m_Radius*2*0.8f, m_Radius*2}, {255,255,255,255});
    EndShaderMode();
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
