/*
    Brain Evil: Dark World
    Copyright (C) Benjamín Gajardo (also known as +KZ)

    This program is shared under the PLUSKAIZO LICENSE, you should have received
    a copy of the license along with the program.
*/

#ifndef VICTIM_H
#define VICTIM_H

#include "entity_base.h"

class CSaw : public IEntity
{

    public:

    static ENTITY_ID(7);
    static ENTITY_CREATOR_FUNC(SawCreator);

    CSector * m_pMySector = nullptr;

    int m_Frame = 0;
    int m_Lifetime;

    CSaw(Vector3 Pos, CSector *pSector, bool pickup = true);

    virtual void Update() override;
    virtual void Render() override;
    virtual int GetSectorID() override;
    virtual void SetSectorID(int id) override;
    virtual unsigned int GetEntityID() override { return SGetEntityID(); }
};

#endif
