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

#ifndef HELPER_H
#define HELPER_H

#include <raylib.h>
#include <input_handler.h>

enum UIButtonState
{
    BTNSTATE_NONE,
    BTNSTATE_HOVER,
    BTNSTATE_PRESSED,
};

void DrawTextCenterScaledToScreen(Font font, const char * pText, float x, float y, float fontsize = 1, Color color = RAYWHITE, float spacing = 0);
UIButtonState RenderUIButtonCenterScaledToScreen(const SInput * pInput, Font font, const char * pText, float x, float y, float fontsize = 1, Color color = RAYWHITE, Color colorhover = GRAY, Color colorclick = DARKGRAY, float spacing = 0);
UIButtonState UpdateUIButtonCenterScaledToScreen(const SInput * pInput, Font font, const char * pText, float x, float y, float fontsize = 1, float spacing = 0);
#endif
