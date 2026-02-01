/*
    Brain Evil: Dark World
    Copyright (C) Benjamín Gajardo (also known as +KZ)

    This program is shared under the PLUSKAIZO LICENSE, you should have received
    a copy of the license along with the program.
*/
#ifndef LEVEL_SCRIPT_HANDLER_H
#define LEVEL_SCRIPT_HANDLER_H

#include <entities/entity_base.h>

class CLevelScriptHandler
{
    IEntity ** m_pTargets = nullptr;
    unsigned int m_NumTargets;

    public:

    void PostLoad();
    void PreUpdate();
    void PostUpdate();
    void Unload();

    void OnExitTouch(IEntity * pEnt);
};

extern CLevelScriptHandler g_LevelScriptHandler;

#endif
