
// Copyright (c) Benjamín Gajardo (also known as +KZ)

#ifndef VICTIM_H
#define VICTIM_H

#include "entity_base.h"

class CVictim : public IEntity
{

    public:

    static ENTITY_ID(4);
    static ENTITY_CREATOR_FUNC(VictimCreator);

    CSector * m_pMySector = nullptr;

    bool m_CalledLevelScript = false;

    CVictim(Vector3 Pos, CSector *pSector);

    virtual void Update() override;
    virtual void Render() override;
    virtual int GetSectorID() override;
    virtual void SetSectorID(int id) override;
    virtual unsigned int GetEntityID() override { return SGetEntityID(); }
};

#endif
