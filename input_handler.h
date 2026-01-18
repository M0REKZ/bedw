// Copyright (c) Benjamín Gajardo (also known as +KZ)

#ifndef INPUT_HANDLER_H
#define INPUT_HANDLER_H

#include <raylib.h>

struct SInput
{
    bool m_Front = false;
    bool m_Back = false;
    bool m_Left = false;
    bool m_Right = false;
    bool m_Jump = false;
    float m_Angle = 0.f;
    float m_AngleY = 0.f;
    bool m_MouseClick = false;
    bool m_MouseRightClick = false;
    Vector2 m_MousePos = {0,0};
    bool m_Escape = false;
    bool m_Enter = false;
    bool m_BackSpace = false;
    bool m_ArrowLeft = false;
    bool m_ArrowRight = false;
    bool m_ArrowUp = false;
    bool m_ArrowDown = false;
    bool m_EditorNeighborKey = false;
    bool m_EditorEditSectorKey = false;
    bool m_EditorCeilingKey = false;
    bool m_EditorFloorKey = false;
    bool m_EditorSaveLevelKey = false;
    bool m_EditorLoadLevelKey = false;
    bool m_EditorCeilingSlopeKey = false;
    bool m_EditorFloorSlopeKey = false;
    bool m_EditorPlaceEntityKey = false;
};

class CInputHandler
{
    SInput m_Inputs;
    public:
    void UpdateInput();
    const SInput * GetInputs() { return &m_Inputs; }
};

extern CInputHandler g_InputHandler;

#endif
