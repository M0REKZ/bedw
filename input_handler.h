// Copyright (c) Benjamín Gajardo (also known as +KZ)

#ifndef INPUT_HANDLER_H
#define INPUT_HANDLER_H

struct SInput
{
    bool m_Front = false;
    bool m_Back = false;
    bool m_Left = false;
    bool m_Right = false;
    bool m_Jump = false;
    float m_Angle = 0.f;
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
