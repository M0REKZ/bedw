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

#include <cstdio>
#include <raylib.h>
#include <rlgl.h>
#include "input_handler.h"
#include "game.h"
#include "globals.h"
#include <cstring>
#include "pause_handler.h"
#include <config_handler.h>

int main(int argc, char** argv)
{
    if(argc > 1)
    {
        for(int i = 1; i < argc; i++)
        {
            if(strstr(argv[i],"editor"))
            {
                g_Game.m_EditorMode = true;
            }
        }
    }

    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(GAME_WIDTH, GAME_HEIGHT, "Brain Evil: Dark World");
    InitAudioDevice();
    SetWindowMinSize(GAME_WIDTH, GAME_HEIGHT);
    SetExitKey(KEY_NULL);

    SetTargetFPS(60);

    if(!g_Game.Init())
        return -1;

    if(g_ConfigHandler.m_Config.m_Fullscreen)
    {
        ToggleBorderlessWindowed();
        SetWindowSize(GetMonitorWidth(GetCurrentMonitor()),GetMonitorHeight(GetCurrentMonitor()));
        SetWindowPosition(0,0);
        g_Globals.m_CurrentWindowWidth = GetScreenWidth();
        g_Globals.m_CurrentWindowHeight = GetScreenHeight();
    }

    bool exit = false;
    
    g_Globals.Init();
    while(!exit)
    {
        if(g_Game.m_HasMusic)
            UpdateMusicStream(g_Game.m_Music);

        if(IsWindowResized())
        {
            g_Globals.m_CurrentWindowWidth = GetScreenWidth();
            g_Globals.m_CurrentWindowHeight = GetScreenHeight();
        }

        g_InputHandler.UpdateInput();

        g_PauseHandler.Update();
        if(!g_PauseHandler.m_IsPaused)
            g_Game.Update();

        g_Globals.m_RaylibCamera.position = g_Globals.m_Camera.m_Pos;
        g_Globals.m_RaylibCamera.target = g_Globals.m_Camera.m_Target;

        BeginDrawing();
        ClearBackground({0,24,87,255});
        g_Game.Render();
        g_PauseHandler.Render();
        EndDrawing();

        if(WindowShouldClose() || g_Game.m_Exit)
        {
            g_Game.Destroy();
            exit = true;
        }
    }

    CloseAudioDevice();
    CloseWindow();

    return 0;
}