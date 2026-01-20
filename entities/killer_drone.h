// Copyright (c) Benjamín Gajardo (also known as +KZ)

#ifndef KILLERDRONE_H
#define KILLERDRONE_H

#include "entity_base.h"

class CKillerDrone : public IEntity
{
    public:

    static ENTITY_ID(3);
    static ENTITY_CREATOR_FUNC(KillerDroneCreator);

    float m_Angle = 0.f;
    bool m_Grounded = false;
    bool m_Attack = false;
    bool m_AirAttacked = false;
    int m_AttackDelay = 0;
    int m_AttackFrame = 0;
    int m_JumpTimer = 60;
    int m_PrevHealth;
    bool m_PlayedDeathSound = false;

    CSector * m_pMySector = nullptr;

    int m_Frame = 0;
    int m_FrameTime = 0;

    Vector3 m_WantedVel = {0.f,0.f};

    CKillerDrone(Vector3 Pos, CSector * pSector);

    virtual void Update() override;
    virtual void Render() override;
    virtual int GetSectorID() override;
    virtual void SetSectorID(int id) override;
    virtual unsigned int GetEntityID() override { return SGetEntityID(); }
};

#endif
