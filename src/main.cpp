/*
    Brain Evil: Dark World
    Copyright (C) Benjamín Gajardo (also known as +KZ)

    This program is shared under the PLUSKAIZO LICENSE, you should have received
    a copy of the license along with the program.
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

const int FPS = 60;

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

    ClearWindowState(FLAG_VSYNC_HINT);

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

    double timedt = 0.;
    float nowtime = GetTime();
    bool can_render = true;
    
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

        timedt += GetTime() - nowtime;
        nowtime = GetTime();

        g_InputHandler.UpdateInput();

        if(timedt >= 1/(double)FPS)
        {
            g_PauseHandler.Update();
            if(!g_PauseHandler.m_IsPaused)
                g_Game.Update();

            timedt -= 1/(double)FPS;
            if(timedt <= 0.001f)
            {
                timedt = 0.f;
                can_render = true;

                WaitTime(1/(double)(FPS+10)); //we have some time for sleeping to not explode cpu
            }

            g_Globals.m_RaylibCamera.position = g_Globals.m_Camera.m_Pos;
            g_Globals.m_RaylibCamera.target = g_Globals.m_Camera.m_Target;
        }

        if(can_render)
        {
            BeginDrawing();
            ClearBackground({0,24,87,255});
            g_Game.Render();
            g_PauseHandler.Render();
            EndDrawing();
            SwapScreenBuffer();
            can_render = false;
        }

        PollInputEvents();

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