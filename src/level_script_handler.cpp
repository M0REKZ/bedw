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
#include "level_script_handler.h"
#include <config_handler.h>
#include "pause_handler.h"
#include "level_handler.h"

CLevelScriptHandler g_LevelScriptHandler;

void CLevelScriptHandler::PostLoad()
{
    if(g_PauseHandler.m_IsMenu)
        return;

    if(g_ConfigHandler.m_GameProgress.m_CurrentLevelNumber == 0)
    {
        if(!m_pTargets)
        {
            m_NumTargets = 3;
            m_pTargets = new IEntity*[m_NumTargets];
            int found = 0;
            for(int i = 0; i < g_Game.NumEntities(); i++)
            {
                IEntity * pEnt = g_Game.GetEntity(i);
                if(!pEnt)
                    continue;

                if(pEnt->GetEntityID() == 3 && found < m_NumTargets)
                {
                    m_pTargets[found] = pEnt;
                    found++;
                }
            }
        }
    }
}

void CLevelScriptHandler::PreUpdate()
{
    if(g_PauseHandler.m_IsMenu)
        return;
}

void CLevelScriptHandler::PostUpdate()
{
    if(g_PauseHandler.m_IsMenu)
        return;

    if(g_ConfigHandler.m_GameProgress.m_CurrentLevelNumber == 0)
    {
        bool alive = false;
        for(int i = 0; i < m_NumTargets; i++)
        {
            if(m_pTargets[i]->m_Health > 0)
            {
                alive = true;
                break;
            }
        }

        if(!alive)
        {
            static bool setsector = false;
            if(!setsector)
            {
                CSector * pDoor = g_Game.GetSector(18);
                if(pDoor)
                {
                    pDoor->m_Ceiling = -70;
                    pDoor->m_Floor = -60;
                }
            }
        }
    }
}

void CLevelScriptHandler::Unload()
{
    if(m_pTargets)
        delete[] m_pTargets;
    m_NumTargets = 0;
    m_pTargets = nullptr;
}

void CLevelScriptHandler::OnExitTouch(IEntity * pEnt)
{
    if(g_PauseHandler.m_IsMenu)
        return;

    if(g_ConfigHandler.m_GameProgress.m_CurrentLevelNumber == 0)
    {
        g_ConfigHandler.m_GameProgress.m_CheckPoint = 0;
        g_LevelHandler.LoadLevelNum(1);
        return;
    }
}
