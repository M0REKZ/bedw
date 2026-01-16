// Copyright (c) Benjamín Gajardo (also known as +KZ)

#ifndef PLAYER_H
#define PLAYER_H

#include "entity_base.h"

class CPlayer : public IEntity
{
    public:

    static ENTITY_ID(0);
    static ENTITY_CREATOR_FUNC(PlayerCreator);

    float m_Angle = 0.f;
    float m_Radius = 1.f;
    bool m_Grounded = false;
    bool m_LookingLeft = false;
    int m_Frame = 0;
    int m_FrameTime = 0;

    Vector3 m_WantedVel = {0.f,0.f};

    CPlayer(Vector3 Pos);

    virtual void Update() override;
    virtual void Render() override;
    virtual int GetSectorID() override;
    virtual unsigned int GetEntityID() override { return SGetEntityID(); }
};

#endif
