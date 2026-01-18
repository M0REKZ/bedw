// Copyright (c) Benjamín Gajardo (also known as +KZ)

#ifndef GLOBALS_H
#define GLOBALS_H

#include <raylib.h>

#define GAME_WIDTH 512
#define GAME_HEIGHT 384 // 256 + 128

class CCamera
{
    public:
    Vector3 m_Pos;
    Vector3 m_Target;
};

class CGlobals
{
    public:
    CCamera m_Camera;
    Camera3D m_RaylibCamera;
    Camera2D m_RaylibCamera2D;

    int m_CurrentWindowWidth = GAME_WIDTH;
    int m_CurrentWindowHeight = GAME_HEIGHT;

    Shader m_TransparentBillboardShader;
    Font m_MainFont;

    void Init();
};

extern CGlobals g_Globals;

#endif
