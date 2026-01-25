/*
    Brain Evil: Dark World
    Copyright (C) Benjamín Gajardo (also known as +KZ)

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <https://www.gnu.org/licenses/>.
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
