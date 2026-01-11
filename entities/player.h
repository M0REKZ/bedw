// Copyright (c) Benjamín Gajardo (also known as +KZ)

#ifndef PLAYER_H
#define PLAYER_H

#include "entity_base.h"

class CPlayer : public IEntity
{
    public:
    float m_Angle = 0.f;
    float m_Radius = 0.3f;

    CPlayer(Vector3 Pos);

    virtual void Update() override;
    virtual void Render() override;
};

#endif
