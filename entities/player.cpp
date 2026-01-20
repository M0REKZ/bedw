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

void CPlayer::DoAttack()
{
    IEntity * pClosestTarget = nullptr;
    IEntity * pEntity = nullptr;
    float closestdistance = 9999.f;
    for(int i = 0; i < g_Game.NumEntities(); i++)
    {
        if((pEntity = g_Game.GetEntity(i)))
        {
            if(pEntity->m_Type == EntType::ENTTYPE_ENEMY)
            {
                if(pEntity->m_Health <= 0)
                    continue;

                float distance = PointDistance3D(pEntity->m_Pos, m_Pos);

                if(distance < closestdistance)
                {
                    closestdistance = distance;
                    pClosestTarget = pEntity;
                }
            }
        }
    }

    if(pClosestTarget)
    {
        if(closestdistance < 2.f)
        {
            pClosestTarget->m_Health--;
        }
    }

    PlaySound(g_Game.m_Sounds[3]);
    m_AttackDelay = 20;
    m_Frame = 10;
}

CPlayer::CPlayer(Vector3 Pos)
{
    m_Type = EntType::ENTTYPE_PLAYER;
    m_PrevHealth = m_Health = 100;
    m_Pos = Pos;
    m_Pos.y += m_Radius;
    m_Vel = {0,0,0};
    m_CanCollide = true;
    g_Game.SetNeededTexture(4); // chydia image
    g_Game.SetNeededTexture(5); // run
    g_Game.SetNeededTexture(6); // also run
    g_Game.SetNeededTexture(7); // walk
    g_Game.SetNeededTexture(8); // also walk
    g_Game.SetNeededTexture(9); // fall
    g_Game.SetNeededTexture(10); // jump
    g_Game.SetNeededTexture(16); // hurt
    g_Game.SetNeededTexture(17); // attack

    g_Game.SetNeededSound(1); // hurt
    g_Game.SetNeededSound(3); // hit miss
    g_Game.SetNeededSound(6); // jump
    g_Game.SetNeededSound(7); // walk
}

void CPlayer::Update()
{
    if(m_PrevHealth > m_Health)
    {
        m_Frame = 9;
        m_PrevHealth = m_Health;
        StopSound(g_Game.m_Sounds[1]);
        PlaySound(g_Game.m_Sounds[1]);
    }

    if(m_Health <=0)
    {
        m_CanCollide = false;
        return;
    }

    if(m_AttackDelay > 0)
    {
        m_AttackDelay--;
    }

    const SInput * pInput = g_InputHandler.GetInputs();

    Vector3 OldPos = m_Pos;

    m_WantedVel = {0,0,0};

    bool walk = false;

    m_Angle = pInput->m_Angle;

    if(pInput->m_Front)
    {
        if(pInput->m_Right)
            m_Angle += M_PI_4;
        else if(pInput->m_Left)
            m_Angle -= M_PI_4;
        walk = true;
    }
    else if(pInput->m_Back)
    {
        m_Angle = pInput->m_Angle + M_PI;
        if(pInput->m_Right)
            m_Angle -= M_PI_4;
        else if(pInput->m_Left)
            m_Angle += M_PI_4;
        walk = true;
    }
    else if(pInput->m_Right)
    {
        m_Angle = pInput->m_Angle + M_PI_2;
        walk = true;
    }
    else if(pInput->m_Left)
    {
        m_Angle = pInput->m_Angle - M_PI_2;
        walk = true;
    }

    if(pInput->m_Left)
    {
        m_LookingLeft = true;
    }
    else if(pInput->m_Right)
    {
        m_LookingLeft = false;
    }

    if(walk)
    {
        m_WantedVel.x = cosf(m_Angle)/2;
        m_WantedVel.z = sinf(m_Angle)/2;
    }

    if(m_Grounded && pInput->m_Jump)
    {
        m_Vel.y = 0.65f;
        PlaySound(g_Game.m_Sounds[6]);
    }

    if(m_AttackDelay <= 0 && pInput->m_MouseClick)
    {
        DoAttack();
    }

    m_Vel.x *= 0.9f;
    if(std::abs(m_Vel.x) < std::abs(m_WantedVel.x))
        m_Vel.x += (m_WantedVel.x > 0.f ? 0.1f : -0.1f) * std::abs(cosf(m_Angle));
    m_Vel.z *= 0.9f;
    if(std::abs(m_Vel.z) < std::abs(m_WantedVel.z))
        m_Vel.z += (m_WantedVel.z > 0.f ? 0.1f : -0.1f) * std::abs(sinf(m_Angle));
    m_Vel.y += -0.025;

    if(std::abs(m_Vel.x) < 0.01f)
        m_Vel.x = 0.f;
    if(std::abs(m_Vel.z) < 0.01f)
        m_Vel.z = 0.f;

    
    CSector *pCurrentSector = g_Game.GetCurrentSector();

    //Player collision
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
    DoMovement(m_Pos, m_Vel, m_Radius, &pCurrentSector, &m_Grounded);

    g_Game.SetCurrentSector(pCurrentSector);

    m_Pos.x += m_Vel.x;
    m_Pos.y += m_Vel.y;
    m_Pos.z += m_Vel.z;

    g_Globals.m_Camera.m_Target = m_Pos;
    g_Globals.m_Camera.m_Target.y = m_Pos.y + .5f;

    Vector3 CameraOffset;

    CameraOffset.x = -cosf(pInput->m_Angle) * 16;
    CameraOffset.y = cosf(pInput->m_AngleY) * 16;
    CameraOffset.z = -sinf(pInput->m_Angle) * 16;

    //Camera collision
    DoMovement(m_Pos, CameraOffset, 0.9f);

    g_Globals.m_Camera.m_Pos.x = m_Pos.x + CameraOffset.x;
    g_Globals.m_Camera.m_Pos.y = m_Pos.y + CameraOffset.y;
    g_Globals.m_Camera.m_Pos.z = m_Pos.z + CameraOffset.z;

    if(m_Grounded && (std::abs(m_Vel.x) > 0.001f || std::abs(m_Vel.z) > 0.001f))
    {
        m_Walking--;
        if(m_Walking <= 0)
        {
            StopSound(g_Game.m_Sounds[7]);
            PlaySound(g_Game.m_Sounds[7]);
            if(std::abs(m_Vel.x) < 0.35f && std::abs(m_Vel.z) < 0.35f)
            {
                m_Walking = 20;
            }
            else
            {
                m_Walking = 10;
            }
        }

        if(m_Walking > 10 && (std::abs(m_Vel.x) >= 0.35f || std::abs(m_Vel.z) >= 0.35f))
        {
            m_Walking = 9;
        }
    }
    else
    {
        m_Walking = 0;
    }
}

void CPlayer::Render()
{
    if(m_Frame == 9 || m_Frame == 10)
    {
        m_FrameTime++;
        if(m_FrameTime >= 10)
        {
            m_FrameTime = 0;
            m_Frame = 0;
        }
    }
    else if(!m_Grounded)
    {
        if(m_Vel.y < 0.f)
        {
            m_Frame = 7;
        }
        else
        {
            m_Frame = 8;
        }
    }
    else if(std::abs(m_Vel.x) > 0.001f || std::abs(m_Vel.z) > 0.001f)
    {
        m_FrameTime++;
        if(std::abs(m_Vel.x) < 0.35f && std::abs(m_Vel.z) < 0.35f)
        {
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
            if(m_FrameTime >= 5)
            {
                m_FrameTime = 0;
                m_Frame++;
            }
            if(m_Frame < 1 || m_Frame > 2)
            {
                m_Frame = 1;
            }
        }
    }
    else
    {
        m_Frame = 0;
        m_FrameTime = 0;
    }

    BeginMode3D(g_Globals.m_RaylibCamera);
    BeginShaderMode(g_Globals.m_TransparentBillboardShader);
    if(m_Health <= 0)
    { /* TODO: need dead sprite */ }
    else if(m_Frame == 0 || m_Frame == 3 || m_Frame == 5)
        DrawBillboardRec(g_Globals.m_RaylibCamera, g_Game.m_Textures[4], {0,0,m_LookingLeft ? -22.f : 22.f,32.f}, m_Pos, {m_Radius*2*0.6875f, m_Radius*2}, {255,255,255,255});
    else if(m_Frame == 1)
        DrawBillboardRec(g_Globals.m_RaylibCamera, g_Game.m_Textures[5], {0,0,m_LookingLeft ? -25.f : 25.f,30.f}, m_Pos, {m_Radius*2*0.83f, m_Radius*2}, {255,255,255,255});
    else if(m_Frame == 2)
        DrawBillboardRec(g_Globals.m_RaylibCamera, g_Game.m_Textures[6], {0,0,m_LookingLeft ? -25.f : 25.f,30.f}, m_Pos, {m_Radius*2*0.83f, m_Radius*2}, {255,255,255,255});
    else if(m_Frame == 4)
        DrawBillboardRec(g_Globals.m_RaylibCamera, g_Game.m_Textures[7], {0,0,m_LookingLeft ? -22.f : 22.f,32.f}, m_Pos, {m_Radius*2*0.6875f, m_Radius*2}, {255,255,255,255});
    else if(m_Frame == 6)
        DrawBillboardRec(g_Globals.m_RaylibCamera, g_Game.m_Textures[8], {0,0,m_LookingLeft ? -22.f : 22.f,32.f}, m_Pos, {m_Radius*2*0.6875f, m_Radius*2}, {255,255,255,255});
    else if(m_Frame == 7)
        DrawBillboardRec(g_Globals.m_RaylibCamera, g_Game.m_Textures[9], {0,0,m_LookingLeft ? -25.f : 25.f,30.f}, m_Pos, {m_Radius*2*0.83f, m_Radius*2}, {255,255,255,255});
    else if(m_Frame == 8)
        DrawBillboardRec(g_Globals.m_RaylibCamera, g_Game.m_Textures[10], {0,0,m_LookingLeft ? -22.f : 22.f,32.f}, m_Pos, {m_Radius*2*0.6875f, m_Radius*2}, {255,255,255,255});
    else if(m_Frame == 9)
        DrawBillboardRec(g_Globals.m_RaylibCamera, g_Game.m_Textures[16], {0,0,m_LookingLeft ? -25.f : 25.f,32.f}, m_Pos, {m_Radius*2*0.83f, m_Radius*2}, {255,255,255,255});
    else if(m_Frame == 10)
        DrawBillboardRec(g_Globals.m_RaylibCamera, g_Game.m_Textures[17], {0,0,m_LookingLeft ? -25.f : 25.f,32.f}, m_Pos, {m_Radius*2*0.83f, m_Radius*2}, {255,255,255,255});
    EndShaderMode();
    EndMode3D();
}

int CPlayer::GetSectorID()
{
    return g_Game.SectorPointerToID(g_Game.GetCurrentSector());
}

void CPlayer::SetSectorID(int id)
{
    g_Game.SetCurrentSector(g_Game.GetSector(id));
}
