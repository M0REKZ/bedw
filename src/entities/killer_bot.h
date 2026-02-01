/*
    Brain Evil: Dark World
    Copyright (C) Benjamín Gajardo (also known as +KZ)

    This program is shared under the PLUSKAIZO LICENSE, you should have received
    a copy of the license along with the program.
*/

#ifndef KILLERBOT_H
#define KILLERBOT_H

#include "entity_base.h"

enum KillerBotType
{
    KILLERBOT_SLICE,
    KILLERBOT_SAW,
    KILLERBOT_SHOOT,
};

class CSawBot
{
    public:
    static ENTITY_ID(5);
    static ENTITY_CREATOR_FUNC(SawBotCreator);
};

class CShootBot
{
    public:
    static ENTITY_ID(6);
    static ENTITY_CREATOR_FUNC(ShootBotCreator);
};


class CKillerBot : public IEntity
{
    public:

    static ENTITY_ID(1);
    static ENTITY_CREATOR_FUNC(KillerBotCreator);

    float m_Angle = 0.f;
    bool m_Grounded = false;
    bool m_Attack = false;
    int m_AttackDelay = 0;
    int m_AttackFrame = 0;
    int m_PrevHealth;
    bool m_PlayedDeathSound = false;
    bool m_EdgeSmart = false;
    bool m_FutureGrounded = false;

    //shootbot
    int m_ShootDelay = 0;
    Vector3 m_LaserShootedPos; //for rendering
    bool m_RenderLaser = false;

    KillerBotType m_BotType = KILLERBOT_SLICE;

    CSector * m_pMySector = nullptr;

    int m_Frame = 0;
    int m_FrameTime = 0;

    Vector3 m_WantedVel = {0.f,0.f};

    CKillerBot(Vector3 Pos, CSector * pSector, KillerBotType BotType = KILLERBOT_SLICE);

    virtual void Update() override;
    virtual void Render() override;
    virtual int GetSectorID() override;
    virtual void SetSectorID(int id) override;
    virtual unsigned int GetEntityID() override;
};

#endif
