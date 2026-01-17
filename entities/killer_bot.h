// Copyright (c) Benjamín Gajardo (also known as +KZ)

#ifndef KILLERBOT_H
#define KILLERBOT_H

#include "entity_base.h"

class CKillerBot : public IEntity
{
    public:

    static ENTITY_ID(1);
    static ENTITY_CREATOR_FUNC(KillerBotCreator);

    float m_Angle = 0.f;
    bool m_Grounded = false;

    CSector * m_pMySector = nullptr;

    int m_Frame = 0;
    int m_FrameTime = 0;

    Vector3 m_WantedVel = {0.f,0.f};

    CKillerBot(Vector3 Pos, CSector * pSector);

    virtual void Update() override;
    virtual void Render() override;
    virtual int GetSectorID() override;
    virtual void SetSectorID(int id) override;
    virtual unsigned int GetEntityID() override { return SGetEntityID(); }
};

#endif
