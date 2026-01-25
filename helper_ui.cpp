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
#include "helper_ui.h"
#include <globals.h>
#include <cstdio>
#include <algorithm>

void DrawTextCenterScaledToScreen(Font font, const char *pText, float x, float y, float fontsize, Color color, float spacing)
{
    fontsize *= 10; //Snowstorm font rendering is very small

    //Scale it to window
    fontsize *= std::min((float)g_Globals.m_CurrentWindowWidth / GAME_WIDTH ,(float)g_Globals.m_CurrentWindowHeight / GAME_HEIGHT);

    Vector2 Size = MeasureTextEx(font, pText, fontsize, spacing);
    DrawTextEx(font, pText, {x-Size.x/2,y-Size.y/2}, fontsize, spacing, color);
}

UIButtonState RenderUIButtonCenterScaledToScreen(const SInput *pInput, Font font, const char *pText, float x, float y, float fontsize, Color color, Color colorhover, Color colorclick, float spacing)
{
    Color c = color;

    //Check button hover
    UIButtonState st = UpdateUIButtonCenterScaledToScreen(pInput, font, pText, x, y, fontsize, spacing);

    fontsize *= 10; //Snowstorm font rendering is very small

    //Scale it to window
    fontsize *= std::min((float)g_Globals.m_CurrentWindowWidth / GAME_WIDTH ,(float)g_Globals.m_CurrentWindowHeight / GAME_HEIGHT);

    Vector2 Size = MeasureTextEx(font, pText, fontsize, spacing);

    if(st == BTNSTATE_HOVER)
        c = colorhover;
    else if(st == BTNSTATE_PRESSED)
        c = colorclick;

    DrawTextEx(font, pText, {x-Size.x/2,y-Size.y/2}, fontsize, spacing, c);

    return st;
}

UIButtonState UpdateUIButtonCenterScaledToScreen(const SInput *pInput, Font font, const char *pText, float x, float y, float fontsize, float spacing)
{
    fontsize *= 10; //Snowstorm font rendering is very small

    //Scale it to window
    fontsize *= std::min((float)g_Globals.m_CurrentWindowWidth / GAME_WIDTH ,(float)g_Globals.m_CurrentWindowHeight / GAME_HEIGHT);

    Vector2 Size = MeasureTextEx(font, pText, fontsize, spacing);

    if(pInput->m_MousePos.x >= x - Size.x/2 && pInput->m_MousePos.x <= x + Size.x/2 &&
        pInput->m_MousePos.y >= y - Size.y/2 && pInput->m_MousePos.y <= y + Size.y/2)
    {
        if(pInput->m_MouseClick)
        {
            return BTNSTATE_PRESSED;
        }
        else
        {
            return BTNSTATE_HOVER;
        }
    }
    
    return BTNSTATE_NONE;
}
