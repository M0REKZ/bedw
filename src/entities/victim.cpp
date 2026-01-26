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

#include "victim.h"
#include <globals.h>
#include <level_script_handler.h>

ENTITY_CREATOR_FUNC(CVictim::VictimCreator)
{
    return new CVictim(Pos, pSector);
}

CVictim::CVictim(Vector3 Pos, CSector * pSector)
{
    m_Pos = Pos;
    m_Vel = {0,0,0};
    m_pMySector = pSector;
    m_Health = 1;
    m_Type = ENTTYPE_PICKUP;

    g_Game.SetNeededTexture(27); // victim girl
}

void CVictim::Update()
{
    if(!m_CalledLevelScript && m_Health <= 0)
    {
        g_LevelScriptHandler.OnExitTouch(this);
    }
}

void CVictim::Render()
{
    if(!m_pMySector->m_Active)
        return;

    if(m_Health <= 0)
        return;

    BeginMode3D(g_Globals.m_RaylibCamera);
    BeginShaderMode(g_Globals.m_TransparentBillboardShader);
    DrawBillboardRec(g_Globals.m_RaylibCamera, g_Game.m_Textures[27], {0,0,32.f,32.f}, m_Pos, {m_Radius*2, m_Radius*2}, {255,255,255,255});
    EndShaderMode();
    EndMode3D();
}

int CVictim::GetSectorID()
{
    return g_Game.SectorPointerToID(m_pMySector);
}

void CVictim::SetSectorID(int id)
{
    m_pMySector = g_Game.GetSector(id);
}
