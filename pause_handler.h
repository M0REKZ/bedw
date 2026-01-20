// Copyright (c) Benjamín Gajardo (also known as +KZ)
#ifndef PAUSE_HANDLER_H
#define PAUSE_HANDLER_H

class CPauseHandler
{
    void HandleMenuOption(unsigned int Page, int Option);

    const char * GetOptionTextLabel(unsigned int Page, int Option);

    public:
    bool m_IsPaused = false;
    bool m_IsMenu = false;
    void Update();
    void Render();

    unsigned int m_CurrentPage = 0;
    int m_HoverOption = false;
    bool m_Selected = false;
};

extern CPauseHandler g_PauseHandler;

#endif
