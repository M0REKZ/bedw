// Copyright (c) Benjamín Gajardo (also known as +KZ)

#include <cstdio>
#include <raylib.h>
#include <rlgl.h>
#include "input_handler.h"
#include "game.h"
#include "globals.h"
#include <cstring>

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

    bool exit = false;
    
    g_Globals.Init();
    while(!exit)
    {
        if(IsWindowResized())
        {
            g_Globals.m_CurrentWindowWidth = GetScreenWidth();
            g_Globals.m_CurrentWindowHeight = GetScreenHeight();
        }

        g_InputHandler.UpdateInput();

        g_Game.Update();

        g_Globals.m_RaylibCamera.position = g_Globals.m_Camera.m_Pos;
        g_Globals.m_RaylibCamera.target = g_Globals.m_Camera.m_Target;

        BeginDrawing();
        ClearBackground({0,0,0,255});
        g_Game.Render();
        EndDrawing();

        if(WindowShouldClose())
            exit = true;
    }

    CloseAudioDevice();
    CloseWindow();

    return 0;
}