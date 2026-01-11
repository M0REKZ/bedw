// Copyright (c) Benjamín Gajardo (also known as +KZ)

#ifndef ENTITY_H
#define ENTITY_H

#include <raylib.h>

class IEntity
{
    public:
    
    Vector3 m_Pos;
    Vector3 m_Vel;

    virtual void Update() {};
    virtual void Render() {};
};
#endif
