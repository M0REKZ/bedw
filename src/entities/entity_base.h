/*
    Brain Evil: Dark World
    Copyright (C) Benjamín Gajardo (also known as +KZ)

    This program is shared under the PLUSKAIZO LICENSE, you should have received
    a copy of the license along with the program.
*/

#ifndef ENTITY_H
#define ENTITY_H

#include <raylib.h>
#include <game.h>
#include <map>

class IEntity
{
    public:

    enum EntType
    {
        ENTTYPE_NONE,
        ENTTYPE_PLAYER,
        ENTTYPE_ENEMY,
        ENTTYPE_PICKUP,
    };
    
    Vector3 m_Pos;
    Vector3 m_Vel;
    float m_Radius = 1.f;
    int m_Health = 0;
    enum EntType m_Type = ENTTYPE_NONE;
    bool m_CanCollide = false;

    virtual void Update() {};
    virtual void Render() {};
    virtual int GetSectorID() { return 0; }
    virtual void SetSectorID(int id) {}
    virtual unsigned int GetEntityID() { return 0; }
};

#define ENTITY_ID(id) unsigned int SGetEntityID() { return id; }
#define ENTITY_CREATOR_FUNC(name) IEntity * name(Vector3 Pos, class CSector * pSector)

extern std::unordered_map<unsigned int, ENTITY_CREATOR_FUNC((*))> g_EntityCreatorList;
void InitEntityCreatorList();

#endif
