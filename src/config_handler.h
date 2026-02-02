/*
    Brain Evil: Dark World
    Copyright (C) Benjamín Gajardo (also known as +KZ)

    This program is shared under the PLUSKAIZO LICENSE, you should have received
    a copy of the license along with the program.
*/
#ifndef CONFIG_HANDLER_H
#define CONFIG_HANDLER_H

#include <raylib.h>
#include <entities/player.h>

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
    
    //weapons
    int m_KeyWeaponHand = KEY_ONE;
    int m_KeyWeaponSaw = KEY_TWO;
    int m_KeyWeaponStick = KEY_THREE;
    int m_KeyWeaponGun = KEY_FOUR;
};

struct SGameProgress
{
    unsigned int m_CurrentLevelNumber = 0;
    unsigned int m_CheckPoint = 0;
    int m_GotStick = false;
    int m_GotGun = false;

    int m_Ammo[CPlayer::PlayerWeapon::NUM_AMMO_WEAPONS] = {0}; //Saw
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
