/*
    Brain Evil: Dark World
    Copyright (C) Benjamín Gajardo (also known as +KZ)

    This program is shared under the PLUSKAIZO LICENSE, you should have received
    a copy of the license along with the program.
*/

#ifndef CHECKPOINT_H
#define CHECKPOINT_H

#include "entity_list.h"
#include "entity_base.h"

class CCheckpoint : public IEntity
{

    public:

    static ENTITY_ID(ENTITYID_CHECKPOINT);
    static ENTITY_CREATOR_FUNC(CheckpointCreator);

    CSector * m_pMySector = nullptr;

    bool m_CalledLevelScript = false;
    int m_CheckpointID = -1;

    CCheckpoint(Vector3 Pos, CSector *pSector);

    virtual void Update() override;
    virtual void Render() override;
    virtual int GetSectorID() override;
    virtual void SetSectorID(int id) override;
    virtual unsigned int GetEntityID() override { return SGetEntityID(); }
};

#endif
