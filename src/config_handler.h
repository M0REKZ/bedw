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
#ifndef CONFIG_HANDLER_H
#define CONFIG_HANDLER_H

#include <raylib.h>

struct SConfig
{
    int m_KeyFront = KEY_W;
    int m_KeyBack = KEY_S;
    int m_KeyLeft = KEY_A;
    int m_KeyRight = KEY_D;
    int m_KeyJump = KEY_SPACE;
    int m_MouseSens = 100;
    int m_MouseInvertY = false;
    int m_Fullscreen = false;
};

struct SGameProgress
{
    unsigned int m_CurrentLevelNumber = 0;
    unsigned int m_CheckPoint = 0;
    int m_GotStick = false;
    int m_GotGun = false;
};

class CConfigHandler
{

    public:

    SConfig m_Config;
    SGameProgress m_GameProgress;

    void Init();
    void SaveConfig();
    void SaveGameProgress();
    void LoadConfig();
    void LoadGameProgress();



};

extern CConfigHandler g_ConfigHandler;

#endif
