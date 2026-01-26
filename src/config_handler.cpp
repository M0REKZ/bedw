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
#include "config_handler.h"
#include <string>
#include <cstring>
#include <fstream>
#include <helper_fs.h>

CConfigHandler g_ConfigHandler;

void CConfigHandler::Init()
{
    LoadConfig();
    LoadGameProgress();
}

void CConfigHandler::SaveConfig()
{
    char filepath[256] = {0};
    snprintf(filepath, sizeof(filepath), "%s/config.txt", GetSavePath());
    std::ofstream file(filepath);

    file << "KEY_FRONT " << m_Config.m_KeyFront << std::endl;
    file << "KEY_BACK " << m_Config.m_KeyBack << std::endl;
    file << "KEY_LEFT " << m_Config.m_KeyLeft << std::endl;
    file << "KEY_RIGHT " << m_Config.m_KeyRight << std::endl;
    file << "KEY_JUMP " << m_Config.m_KeyJump << std::endl;

    file << "MOUSE_SENS " << m_Config.m_MouseSens << std::endl;
    file << "MOUSE_INVERT_Y " << m_Config.m_MouseInvertY << std::endl;

    file << "FULLSCREEN " << m_Config.m_Fullscreen << std::endl;

    file.close();
}

void CConfigHandler::SaveGameProgress()
{
    char filepath[256] = {0};
    snprintf(filepath, sizeof(filepath), "%s/progress.txt", GetSavePath());
    std::ofstream file(filepath);

    file << "CURRENT_LEVEL_NUMBER " << m_GameProgress.m_CurrentLevelNumber << std::endl;
    file << "CURRENT_CHECKPOINT " << m_GameProgress.m_CheckPoint << std::endl;
    file << "GOT_GUN " << m_GameProgress.m_GotGun << std::endl;
    file << "GOT_STICK " << m_GameProgress.m_GotStick << std::endl;

    file.close();
}

void CConfigHandler::LoadConfig()
{
    char filepath[256] = {0};
    snprintf(filepath, sizeof(filepath), "%s/config.txt", GetSavePath());
    std::ifstream file(filepath);

    std::string line;

    while(std::getline(file, line))
    {
        if(strstr(line.c_str(), "KEY_FRONT"))
        {
            sscanf(line.c_str(), "KEY_FRONT %d", &m_Config.m_KeyFront);
        }

        if(strstr(line.c_str(), "KEY_BACK"))
        {
            sscanf(line.c_str(), "KEY_BACK %d", &m_Config.m_KeyBack);
        }

        if(strstr(line.c_str(), "KEY_LEFT"))
        {
            sscanf(line.c_str(), "KEY_LEFT %d", &m_Config.m_KeyLeft);
        }

        if(strstr(line.c_str(), "KEY_RIGHT"))
        {
            sscanf(line.c_str(), "KEY_RIGHT %d", &m_Config.m_KeyRight);
        }

        if(strstr(line.c_str(), "KEY_JUMP"))
        {
            sscanf(line.c_str(), "KEY_JUMP %d", &m_Config.m_KeyJump);
        }

        if(strstr(line.c_str(), "MOUSE_SENS"))
        {
            sscanf(line.c_str(), "MOUSE_SENS %d", &m_Config.m_MouseSens);
        }

        if(strstr(line.c_str(), "MOUSE_INVERT_Y"))
        {
            sscanf(line.c_str(), "MOUSE_INVERT_Y %d", &m_Config.m_MouseInvertY);
        }

        if(strstr(line.c_str(), "FULLSCREEN"))
        {
            sscanf(line.c_str(), "FULLSCREEN %d", &m_Config.m_Fullscreen);
        }
    }

    file.close();
}

void CConfigHandler::LoadGameProgress()
{
    char filepath[256] = {0};
    snprintf(filepath, sizeof(filepath), "%s/progress.txt", GetSavePath());
    std::ifstream file(filepath);

    std::string line;

    while(std::getline(file, line))
    {
        if(strstr(line.c_str(), "CURRENT_LEVEL_NUMBER"))
        {
            sscanf(line.c_str(), "CURRENT_LEVEL_NUMBER %u", &m_GameProgress.m_CurrentLevelNumber);
        }

        if(strstr(line.c_str(), "CURRENT_CHECKPOINT"))
        {
            sscanf(line.c_str(), "CURRENT_CHECKPOINT %u", &m_GameProgress.m_CheckPoint);
        }

        if(strstr(line.c_str(), "GOT_GUN"))
        {
            sscanf(line.c_str(), "GOT_GUN %d", &m_GameProgress.m_GotGun);
        }

        if(strstr(line.c_str(), "GOT_STICK"))
        {
            sscanf(line.c_str(), "GOT_STICK %d", &m_GameProgress.m_GotStick);
        }
    }

    file.close();
}
