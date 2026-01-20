// Copyright (c) Benjamín Gajardo (also known as +KZ)

#include "globals.h"

CGlobals g_Globals;

void CGlobals::Init()
{
    m_RaylibCamera.up = {0.f, 1.f, 0.f};
    m_RaylibCamera.fovy = 45.f;
    m_RaylibCamera.projection = CAMERA_PERSPECTIVE;

    //screen drawing
    m_RaylibCamera2D.offset = {0,0};
    m_RaylibCamera2D.target = {0,0};
    m_RaylibCamera2D.rotation = 0;
    m_RaylibCamera2D.zoom = 1;

    m_TransparentBillboardShader = LoadShader(nullptr, "data/shaders/discard_alpha.fs");
    m_MainFont = LoadFontEx("data/fonts/MidnightLetters-Regular.otf", 64, nullptr, 0);
    GenTextureMipmaps(&m_MainFont.texture);
    SetTextureFilter(m_MainFont.texture, TEXTURE_FILTER_TRILINEAR);
}
