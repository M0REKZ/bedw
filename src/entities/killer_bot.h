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

#ifndef KILLERBOT_H
#define KILLERBOT_H

#include "entity_base.h"

enum KillerBotType
{
    KILLERBOT_SLICE,
    KILLERBOT_SAW,
    KILLERBOT_SHOOT,
};

class CSawBot
{
    public:
    static ENTITY_ID(5);
    static ENTITY_CREATOR_FUNC(SawBotCreator);
};

class CShootBot
{
    public:
    static ENTITY_ID(6);
    static ENTITY_CREATOR_FUNC(ShootBotCreator);
};


class CKillerBot : public IEntity
{
    public:

    static ENTITY_ID(1);
    static ENTITY_CREATOR_FUNC(KillerBotCreator);

    float m_Angle = 0.f;
    bool m_Grounded = false;
    bool m_Attack = false;
    int m_AttackDelay = 0;
    int m_AttackFrame = 0;
    int m_PrevHealth;
    bool m_PlayedDeathSound = false;
    bool m_EdgeSmart = false;
    bool m_FutureGrounded = false;

    //shootbot
    int m_ShootDelay = 0;
    Vector3 m_LaserShootedPos; //for rendering
    bool m_RenderLaser = false;

    KillerBotType m_BotType = KILLERBOT_SLICE;

    CSector * m_pMySector = nullptr;

    int m_Frame = 0;
    int m_FrameTime = 0;

    Vector3 m_WantedVel = {0.f,0.f};

    CKillerBot(Vector3 Pos, CSector * pSector, KillerBotType BotType = KILLERBOT_SLICE);

    virtual void Update() override;
    virtual void Render() override;
    virtual int GetSectorID() override;
    virtual void SetSectorID(int id) override;
    virtual unsigned int GetEntityID() override;
};

#endif
