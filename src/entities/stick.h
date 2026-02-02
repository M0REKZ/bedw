/*
    Brain Evil: Dark World
    Copyright (C) Benjamín Gajardo (also known as +KZ)

    This program is shared under the PLUSKAIZO LICENSE, you should have received
    a copy of the license along with the program.
*/

#ifndef STICK_H
#define STICK_H

#include "entity_base.h"

class CStick : public IEntity
{

    public:

    static ENTITY_ID(8);
    static ENTITY_CREATOR_FUNC(StickCreator);

    CSector * m_pMySector = nullptr;

    bool m_CalledLevelScript = false;

    CStick(Vector3 Pos, CSector *pSector);

    virtual void Update() override;
    virtual void Render() override;
    virtual int GetSectorID() override;
    virtual void SetSectorID(int id) override;
    virtual unsigned int GetEntityID() override { return SGetEntityID(); }
};

#endif
