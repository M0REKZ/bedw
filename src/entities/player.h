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

#ifndef PLAYER_H
#define PLAYER_H

#include "entity_base.h"

class CPlayer : public IEntity
{
    void DoAttack();
    void CollectPickups();

    public:

    static ENTITY_ID(0);
    static ENTITY_CREATOR_FUNC(PlayerCreator);

    float m_Angle = 0.f;
    bool m_Grounded = false;
    bool m_LookingLeft = false;
    int m_Frame = 0;
    int m_FrameTime = 0;

    int m_PrevHealth;
    int m_AttackDelay = 0;

    int m_Walking = 0;

    Vector3 m_WantedVel = {0.f,0.f};

    CPlayer(Vector3 Pos);

    virtual void Update() override;
    virtual void Render() override;
    virtual int GetSectorID() override;
    virtual void SetSectorID(int id) override;
    virtual unsigned int GetEntityID() override { return SGetEntityID(); }
};

#endif
