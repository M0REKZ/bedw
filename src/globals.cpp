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
