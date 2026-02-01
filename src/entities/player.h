/*
    Brain Evil: Dark World
    Copyright (C) Benjamín Gajardo (also known as +KZ)

    This program is shared under the PLUSKAIZO LICENSE, you should have received
    a copy of the license along with the program.
*/

#ifndef PLAYER_H
#define PLAYER_H

#include "entity_base.h"

class CPlayer : public IEntity
{
    void DoAttack();
    void CollectPickups();

    public:

    static ENTITY_ID(0);
    static ENTITY_CREATOR_FUNC(PlayerCreator);

    float m_Angle = 0.f;
    bool m_Grounded = false;
    bool m_LookingLeft = false;
    int m_Frame = 0;
    int m_FrameTime = 0;

    int m_PrevHealth;
    int m_AttackDelay = 0;

    int m_Walking = 0;

    Vector3 m_WantedVel = {0.f,0.f};

    CPlayer(Vector3 Pos);

    virtual void Update() override;
    virtual void Render() override;
    virtual int GetSectorID() override;
    virtual void SetSectorID(int id) override;
    virtual unsigned int GetEntityID() override { return SGetEntityID(); }
};

#endif
