/*
    Brain Evil: Dark World
    Copyright (C) Benjamín Gajardo (also known as +KZ)

    This program is shared under the PLUSKAIZO LICENSE, you should have received
    a copy of the license along with the program.
*/

#ifndef MENUCAMERA_H
#define MENUCAMERA_H

#include "entity_base.h"

class CMenuCamera : public IEntity
{

    public:

    static ENTITY_ID(2);
    static ENTITY_CREATOR_FUNC(MenuCameraCreator);

    float m_Angle = 0.f;

    CMenuCamera(Vector3 Pos);

    virtual void Render() override;
    virtual int GetSectorID() override;
    virtual void SetSectorID(int id) override;
    virtual unsigned int GetEntityID() override { return SGetEntityID(); }
};

#endif
