// Copyright (c) Benjamín Gajardo (also known as +KZ)

#ifndef ENTITY_H
#define ENTITY_H

#include <raylib.h>
#include <game.h>
#include <map>

class IEntity
{
    public:
    
    Vector3 m_Pos;
    Vector3 m_Vel;

    virtual void Update() {};
    virtual void Render() {};
    virtual int GetSectorID() { return 0; }
    virtual unsigned int GetEntityID() { return 0; }
};

#define ENTITY_ID(id) unsigned int SGetEntityID() { return id; }
#define ENTITY_CREATOR_FUNC(name) IEntity * name(Vector3 Pos, class CSector * pSector)

extern std::unordered_map<unsigned int, ENTITY_CREATOR_FUNC((*))> g_EntityCreatorList;
void InitEntityCreatorList();

#endif
