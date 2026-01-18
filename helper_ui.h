// Copyright (c) Benjamín Gajardo (also known as +KZ)

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
